#include "game_system.h"

#include "texture_manager.h"

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
    }

    sf::RenderWindow &GameSystem::window() const
    {
        return _window;
    }

    void GameSystem::update(float dt)
    {
        if (_player == NULL)
        {
            return;
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

        auto bullets = _bullet_manager.bullets();
        for (auto i = 0u; i < bullets->size(); i++)
        {
            auto bullet = (*bullets)[i].get();
            if (bullet != nullptr)
            {
                bullet->update(dt);
                // TODO
                auto group = bullet->group();
                if (group == Bullet::PLAYER)
                {
                    // Search for enemies
                    for (auto i = 0u; i < enemies->size(); i++)
                    {
                        auto enemy = (*enemies)[i].get();
                        if (enemy != nullptr && enemy->hitbox().intersects(bullet->hitbox()))
                        {
                            _enemy_manager.remove_enemy(enemy);
                        }
                    }
                }
                else if (group == Bullet::ENEMY && 
                        _player->hitbox().intersects(bullet->hitbox()))
                {
                    _player->take_damage(bullet->damage());
                    _bullet_manager.remove_bullet(bullet);
                } 
                else if (bullet->dead())
                {
                    _bullet_manager.remove_bullet(bullet);
                }
            }
        }
    }
    void GameSystem::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        //_bullet_manager.draw(window);
        target.draw(_bullet_manager, states);
        target.draw(_enemy_manager, states);
        if (_player != nullptr)
        {
            target.draw(*_player.get(), states);
        }
    }

    BulletManager &GameSystem::bullet_manager()
    {
        return _bullet_manager;
    }
    void GameSystem::create_default_bullets()
    {
        auto texture_manager = TextureManager::main();

        auto bullet_blue_circle = texture_manager->texture("bullet_blue_circle");
        auto bullet_blue = texture_manager->texture("bullet_blue");
        
        auto pattern_bullet = new PatternBullet(*bullet_blue_circle, 1.0f);
        pattern_bullet->pattern_type(0u);
        pattern_bullet->hitbox().radius(5.0f);
        _bullet_manager.add_src_pattern_bullet(pattern_bullet, "straight_blue_circle");
        
        pattern_bullet = new PatternBullet(*bullet_blue, 1.0f);
        pattern_bullet->pattern_type(0u);
        pattern_bullet->hitbox().radius(5.0f);
        _bullet_manager.add_src_pattern_bullet(pattern_bullet, "straight_blue");

        pattern_bullet = new PatternBullet(*bullet_blue_circle, 1.0f);
        pattern_bullet->pattern_type(1u);
        pattern_bullet->hitbox().radius(5.0f);
        _bullet_manager.add_src_pattern_bullet(pattern_bullet, "arena_blue");
    }

    EnemyManager &GameSystem::enemy_manager()
    {
        return _enemy_manager;
    }
    void GameSystem::create_default_enemieS()
    {
        auto texture_manager = TextureManager::main();

        auto texture = texture_manager->texture("enemy");
        auto enemy= new Enemy(*texture);
        enemy->hitbox().radius(5.0f);
        _enemy_manager.add_src_enemy(enemy, "enemy1");
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
}
