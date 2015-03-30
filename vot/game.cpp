#include "game.h"

#include "texture_manager.h"
#include "game_system.h"
#include "enemy_fighter.h"

#include <iostream>

namespace vot
{
    Game::Game() :
        _spawn_timer(0.0f),
        _paused(false)
    {
        auto player = new vot::Player(*TextureManager::texture("player"));
        player->sprite().setScale(0.5f, 0.5f);
        player->hitbox().radius(5.0f);
        player->location(sf::Vector2f(0.0f, 100.0f));
        player->init();

        this->player(player);
    
        auto window_size = GameSystem::window().getSize();
        _camera.setSize(static_cast<float>(window_size.x), static_cast<float>(window_size.y));
        _background.speed(0.1f);
        _background.create();
        _background2.speed(0.05f);
        _background2.create();
        _background3.speed(0.025f);
        _background3.create();
    
        _hud.create();
        _world_hud.create();
    }

    void Game::update(float dt)
    {
        // TODO Why is the dt coming from main not constant.
        dt = 1.0f / 60.0f;
        std::cout << "Update dt: " << dt << "\n";
        if (_player == nullptr || _paused)
        {
            return;
        }

        auto enemy_manager = GameSystem::enemy_manager();
        auto powerup_manager = GameSystem::powerup_manager();
        auto bullet_manager = GameSystem::bullet_manager();
        auto beam_manager = GameSystem::beam_manager();
        auto particle_manager = GameSystem::particle_manager();

        if (true && enemy_manager->num_enemies() < 3)
        {
            _spawn_timer += dt;
            if (_spawn_timer > 3.0f)
            {
                auto enemy = enemy_manager->spawn_enemy("enemy1");
                enemy->controller(new EnemyFighter(enemy));
                enemy->translate(Utils::rand_vec(-50.0f, 50.0f));
                _spawn_timer = 0.0f;

                if (_player->target() == nullptr && _player->auto_target())
                {
                    _player->target(enemy);
                }
            }
        }

        auto enemies = enemy_manager->objects();
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

        powerup_manager->update(dt);

        auto bullets = bullet_manager->objects();
        for (auto i = 0u; i < bullets->size(); i++)
        {
            auto bullet = bullets->at(i).get();
            if (bullet != nullptr)
            {
                bullet->update(dt);
                // Remove dead (old) bullets.
                if (bullet->dead())
                {
                    bullet_manager->remove_bullet(bullet);
                    continue;
                }
                
                auto group = bullet->group();
                // Player bullet
                if (group == Group::PLAYER)
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
                                kill_enemy(enemy);
                            }

                            bullet_hit_particles(bullet, enemy, "bullet_blue_circle");

                            bullet_manager->remove_bullet(bullet);
                            break;
                        }
                    }
                }
                // Enemy bullet
                else if (group == Group::ENEMY && 
                        _player->hitbox().intersects(bullet->hitbox()))
                {
                    _player->take_damage(bullet->damage());
                            
                    bullet_hit_particles(bullet, _player.get(), "bullet_red_circle");
                    bullet_manager->remove_bullet(bullet);
                } 
            }
        }

        auto beams = beam_manager->objects();
        for (auto i = 0u; i < beams->size(); i++)
        {
            auto beam = beams->at(i).get();
            if (beam == nullptr || !beam->is_active())
            {
                continue;
            }

            beam->hitting_target_length(-1.0f);
            if (beam->group() == Group::PLAYER)
            {
                Enemy *hitting_target = nullptr;
                auto hitting_target_length = -1.0f;
                sf::Vector2f hitting_normal;
                sf::Vector2f hitting_point;
                    
                sf::Vector2f points[2];
                sf::Vector2f normals[2];

                for (auto i = 0u; i < enemies->size(); i++)
                {
                    auto enemy = (*enemies)[i].get();

                    if (enemy != nullptr && Utils::ray_circle_intersect(beam->hitbox(), enemy->hitbox(), points, normals))
                    {
                        auto dpos = beam->hitbox().origin() - points[0];
                        auto distance = sqrtf(dpos.x * dpos.x + dpos.y * dpos.y);
                        if (distance > beam->max_length())
                        {
                            continue;
                        }

                        if (hitting_target == nullptr || hitting_target_length > distance)
                        {
                            beam->hitting_target_length(distance);
                            hitting_target_length = distance;
                            hitting_target = enemy;
                            hitting_point = points[0];
                            hitting_normal = normals[0];
                        }
                    }
                }

                if (hitting_target != nullptr)
                {
                    auto damage = beam->dps() * dt;
                    hitting_target->take_damage(damage);
                            
                    if (beam->hit_particle_cooldown() > 0.05f)
                    {
                        beam_hit_particles(hitting_point, hitting_normal, "bullet_blue_circle");
                        beam->hit_particle_cooldown(0.0f);
                    }

                    if (hitting_target->is_dead())
                    {
                        kill_enemy(hitting_target);
                    }
                }
            }
            beam->update(dt);
        }

        auto powerups = powerup_manager->active_powerups();
        for (auto i = 0u; i < powerups->size(); i++)
        {
            auto powerup = powerups->at(i).get();
            auto intersects = _player->powerup_hitbox().intersects(powerup->hitbox());
            if (intersects)
            {
                _player->add_powerup(*powerup);
                powerup_manager->remove_powerup(powerup);
                i--;
            }
        }

        _background.update(dt);
        _background2.update(dt);
        _background3.update(dt);

        _hud.update(dt); 
        _world_hud.update(dt);

        particle_manager->update(dt);
    }

    void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
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

        target.draw(*GameSystem::powerup_manager(), states);
        target.draw(*GameSystem::enemy_manager(), states);
        target.draw(*GameSystem::bullet_manager(), states);
        target.draw(*GameSystem::beam_manager(), states);

        if (_player != nullptr)
        {
            target.draw(*_player.get(), states);
        }
        target.draw(*GameSystem::particle_manager(), states);

        target.draw(_world_hud);
        
        target.setView(GameSystem::hud_camera());
        target.draw(_hud);
    }
    
    void Game::player(Player *value)
    {
        _player = std::unique_ptr<Player>(value);
        _player->id(1u);
    }
    Player *Game::player() const
    {
        return _player.get();
    }

    void Game::player_info(PlayerInfo *value)
    {
        _player_info = std::unique_ptr<PlayerInfo>(value);
    }
    PlayerInfo *Game::player_info() const
    {
        return _player_info.get();
    }

    sf::View &Game::camera()
    {
        return _camera;
    }

    void Game::paused(bool value)
    {
        _paused = value;
    }
    bool Game::paused() const
    {
        return _paused;
    }

    void Game::on_resize(uint32_t width, uint32_t height)
    {
        auto fwidth = static_cast<float>(width);
        auto fheight = static_cast<float>(height);
        _camera.setSize(fwidth, fheight);
    }

    Enemy *Game::next_target(Enemy *current)
    {
        auto start_index = 0u;
        auto enemies = GameSystem::enemy_manager()->objects();
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

    void Game::bullet_hit_particles(Bullet *bullet, Character *hit, const std::string &texture)
    {
        auto system = GameSystem::particle_manager()->spawn_system(*TextureManager::texture(texture), 10);
        system->setPosition(bullet->location());

        auto dpos = bullet->location() - hit->location();
        auto angle = Utils::vector_degrees(dpos);
        system->setRotation(angle);
        system->init();
    }
    void Game::beam_hit_particles(const sf::Vector2f &point, const sf::Vector2f &normal, const std::string &texture)
    {
        auto system = GameSystem::particle_manager()->spawn_system(*TextureManager::texture(texture), 1);
        system->setPosition(point);

        auto angle = Utils::vector_degrees(normal);
        system->setRotation(angle);
        system->init();
    }

    void Game::kill_enemy(Enemy *enemy)
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

        auto explosion = GameSystem::sound_manager()->spawn_sound("explosion2");
        explosion->play();

        auto rand = Utils::randf();

        auto type = rand > 0.66f ? "bullet" : (rand < 0.33f ? "beam" : "homing");
        auto powerup = GameSystem::powerup_manager()->spawn_powerup(type);
        powerup->location(enemy->location());

        GameSystem::enemy_manager()->remove_enemy(enemy);
    }
}
