#include "game_system.h"

#include <random>
#include <iostream>

#include "texture_manager.h"
#include "utils.h"

namespace vot
{
    GameSystem *GameSystem::s_main = nullptr;

    void GameSystem::main(GameSystem *main)
    {
        s_main = main;
    }
    GameSystem *GameSystem::main()
    {
        return s_main;
    }

    GameSystem::GameSystem(sf::RenderWindow &window) :
        _window(window)
    {
        auto size = static_cast<int>(sf::Keyboard::KeyCount);
        for (auto i = 0; i < size; i++)
        {
            _keys_pressed[i] = 0u;
            _keys_released[i] = 0u;
        }
    }

    void GameSystem::init()
    {
        TextureManager::main()->display("Init GS");
        create_default_bullets();
        create_default_enemieS();
        create_default_powerups();

        auto player = new vot::Player(*TextureManager::texture("player"));
        player->sprite().setScale(0.5f, 0.5f);
        player->hitbox().radius(5.0f);
        player->location(sf::Vector2f(0.0f, 100.0f));

        this->player(player);
    
        auto window_size = _window.getSize();
        _camera.setSize(window_size.x, window_size.y);
        _hud_camera = _window.getDefaultView();

        _background.speed(0.1f);
        _background.create();
        _background2.speed(0.05f);
        _background2.create();
        _background3.speed(0.025f);
        _background3.create();
    
        _hud.create();
        _world_hud.create();
    }

    sf::RenderWindow &GameSystem::window() const
    {
        return _window;
    }

    void GameSystem::update(float dt)
    {
        _update_counter++;
        if (_player == NULL)
        {
            return;
        }

        if (true && _enemy_manager.num_enemies() < 3)
        {
            _spawn_timer += dt;
            if (_spawn_timer > 3.0f)
            {
                auto enemy = enemy_manager().spawn_enemy("enemy1");
                enemy->translate(Utils::rand_vec(-500.0f, 500.0f));
                _spawn_timer = 0.0f;

                if (_player->target() == nullptr && _player->auto_target())
                {
                    _player->target(enemy);
                }
            }
        }

        auto enemies = _enemy_manager.enemies();
        for (auto i = 0u; i < enemies->size(); i++)
        {
            auto enemy = (*enemies)[i].get();
            if (enemy != nullptr)
            {
                enemy->update(dt);
            }
        }
        
        _player->update(dt);
        _camera.setCenter(_player->location());
        _camera.setRotation(_player->rotation());

        _powerup_manager.update(dt);

        auto bullets = _bullet_manager.bullets();
        for (auto i = 0u; i < bullets->size(); i++)
        {
            auto bullet = (*bullets)[i].get();
            if (bullet != nullptr)
            {
                bullet->update(dt);
                // Remove dead (old) bullets.
                if (bullet->dead())
                {
                    _bullet_manager.remove_bullet(bullet);
                    continue;
                }
                
                auto group = bullet->group();
                // Player bullet
                if (group == Bullet::PLAYER)
                {
                    // Search for enemies
                    for (auto i = 0u; i < enemies->size(); i++)
                    {
                        auto enemy = (*enemies)[i].get();
                        if (enemy != nullptr && enemy->hitbox().intersects(bullet->hitbox()))
                        {
                            enemy->take_damage(bullet->damage());

                            if (enemy->is_dead())
                            {
                                if (enemy == _player->target())
                                {
                                    if (_player->auto_target())
                                    {
                                        _player->target(next_target(enemy));
                                    }
                                    else
                                    {
                                        _player->target(nullptr);
                                    }
                                }

                                auto rand = Utils::randf();

                                auto powerup = _powerup_manager.spawn_powerup(
                                        rand > 0.5f ? "bullet" : "homing");
                                powerup->location(enemy->location());
                                powerup->init_location(enemy->location());
                            
                                _enemy_manager.remove_enemy(enemy);
                            }

                            bullet_hit_particles(bullet, enemy, "bullet_blue_circle");

                            _bullet_manager.remove_bullet(bullet);
                        }
                    }
                }
                // Enemy bullet
                else if (group == Bullet::ENEMY && 
                        _player->hitbox().intersects(bullet->hitbox()))
                {
                    _player->take_damage(bullet->damage());
                            
                    bullet_hit_particles(bullet, _player.get(), "bullet_red_circle");
                    _bullet_manager.remove_bullet(bullet);
                } 
            }
        }

        auto powerups = _powerup_manager.active_powerups();
        for (auto i = 0u; i < powerups->size(); i++)
        {
            auto powerup = powerups->at(i).get();
            if (_player->hitbox().intersects(powerup->hitbox()))
            {
                _player->add_powerup(*powerup);
                _powerup_manager.remove_powerup(powerup);
                i--;
            }
        }

        _background.update(dt);
        _background2.update(dt);
        _background3.update(dt);

        _hud.update(dt); 
        _world_hud.update(dt);

        _particle_manager.update(dt);
    }

    void GameSystem::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.setView(_camera);

        target.draw(_background3, states);
        target.draw(_background2, states);
        target.draw(_background, states);

        if (false)
        {
            for (auto y = -5; y <= 5; y++)
            {
                sf::RectangleShape line;
                line.setSize(sf::Vector2f(200.0, 1.0f));
                line.setPosition(-100.0f, y * 20.0f);
                target.draw(line, states);
            }
            for (auto x = -5; x <= 5; x++)
            {
                sf::RectangleShape line;
                line.setSize(sf::Vector2f(1.0, 200.0f));
                line.setPosition(x * 20.0f, -100.0f);
                target.draw(line, states);
            }
        }

        target.draw(_powerup_manager, states);
        target.draw(_enemy_manager, states);
        target.draw(_particle_manager, states);
        target.draw(_bullet_manager, states);

        if (_player != nullptr)
        {
            target.draw(*_player.get(), states);
        }

        target.draw(_world_hud);
        
        target.setView(_hud_camera);
        target.draw(_hud);
    }

    BulletManager &GameSystem::bullet_manager()
    {
        return _bullet_manager;
    }
    void GameSystem::create_default_bullets()
    {
        auto tm = TextureManager::main();

        auto bullet_blue_circle = tm->find_texture("bullet_blue_circle");
        auto bullet_red_circle = tm->find_texture("bullet_red_circle");
        auto bullet_blue = tm->find_texture("bullet_blue");

        tm->display("Create bullets");
        
        auto pattern_bullet = new PatternBullet(*bullet_blue_circle, 1.0f);
        pattern_bullet->pattern_type(0u);
        pattern_bullet->hitbox().radius(5.0f);
        _bullet_manager.add_src_pattern_bullet(pattern_bullet, "straight_blue_circle");
        
        pattern_bullet = new PatternBullet(*bullet_blue, 1.0f);
        pattern_bullet->pattern_type(0u);
        pattern_bullet->hitbox().radius(5.0f);
        _bullet_manager.add_src_pattern_bullet(pattern_bullet, "straight_blue");
        
        pattern_bullet = new PatternBullet(*bullet_red_circle, 1.0f);
        pattern_bullet->pattern_type(0u);
        pattern_bullet->hitbox().radius(5.0f);
        _bullet_manager.add_src_pattern_bullet(pattern_bullet, "straight_red_circle");

        pattern_bullet = new PatternBullet(*bullet_blue_circle, 1.0f);
        pattern_bullet->pattern_type(1u);
        pattern_bullet->hitbox().radius(5.0f);
        _bullet_manager.add_src_pattern_bullet(pattern_bullet, "arena_blue");

        pattern_bullet = new PatternBullet(*bullet_blue_circle, 1.0f);
        pattern_bullet->pattern_type(10u);
        pattern_bullet->hitbox().radius(5.0f);
        _bullet_manager.add_src_pattern_bullet(pattern_bullet, "test");
        
        auto homing_bullet = new HomingBullet(*bullet_blue_circle, 2.0f);
        homing_bullet->total_lifetime(5.0f);
        homing_bullet->hitbox().radius(5.0f);
        _bullet_manager.add_src_homing_bullet(homing_bullet, "homing_blue");
    }

    EnemyManager &GameSystem::enemy_manager()
    {
        return _enemy_manager;
    }
    void GameSystem::create_default_enemieS()
    {
        auto texture = TextureManager::texture("enemy");
        auto enemy = new Enemy(*texture);
        enemy->sprite().setScale(0.5f, 0.5f);
        enemy->hitbox().radius(25.0f);
        _enemy_manager.add_src_enemy(enemy, "enemy1");
    }

    ParticleSystemManager &GameSystem::particle_manager()
    {
        return _particle_manager;
    }

    PowerupManager &GameSystem::powerup_manager()
    {
        return _powerup_manager;
    }
    void GameSystem::create_default_powerups()
    {
        auto bullet_powerup = TextureManager::texture("powerup_bullet");
        auto homing_powerup = TextureManager::texture("powerup_homing");
        auto powerup = new Powerup(*bullet_powerup, Powerup::BULLET, 1);
        _powerup_manager.add_src_powerup("bullet", powerup);

        powerup = new Powerup(*homing_powerup, Powerup::HOMING, 1);
        _powerup_manager.add_src_powerup("homing", powerup);
    }

    void GameSystem::player(Player *value)
    {
        _player = std::unique_ptr<Player>(value);
        _player->id(1u);
    }
    Player *GameSystem::player() const
    {
        return _player.get();
    }

    sf::View &GameSystem::camera()
    {
        return _camera;
    }

    Enemy *GameSystem::next_target(Enemy *current)
    {
        auto start_index = 0u;
        auto enemies = _enemy_manager.enemies();
        if (current != nullptr)
        {
            start_index = current->index();
        }

        auto end_index = start_index;
        auto i = start_index + 1;
        while (i != end_index)
        {
            if (i >= enemies->size())
            {
                i = 0u;
                continue;
            }
            auto enemy = enemies->at(i).get(); 
            if (enemy != nullptr && enemy != current)
            {
                return enemy;
            }
            ++i;
        }

        return nullptr;
    }

    void GameSystem::key_pressed(sf::Keyboard::Key key)
    {
        _keys_pressed[key] = _update_counter + 1;
    }
    bool GameSystem::is_key_pressed(sf::Keyboard::Key key) const
    {
        return _keys_pressed[key] == _update_counter;
    }
    void GameSystem::key_released(sf::Keyboard::Key key)
    {
        _keys_released[key] = _update_counter + 1;
    }
    bool GameSystem::is_key_released(sf::Keyboard::Key key) const
    {
        return _keys_released[key] == _update_counter;
    }

    void GameSystem::on_resize(uint32_t width, uint32_t height)
    {
        _camera.setSize(width, height);
        _hud_camera.setSize(width, height);
        _window.setSize(sf::Vector2u(width, height));
    }

    void GameSystem::bullet_hit_particles(Bullet *bullet, Character *hit, const std::string &texture)
    {
        auto system = _particle_manager.spawn_system(*TextureManager::texture(texture), 10);
        system->setPosition(bullet->getPosition());

        auto dpos = bullet->getPosition() - hit->location();
        auto angle = Utils::vector_angle(dpos);
        system->setRotation(angle);
    }
}
