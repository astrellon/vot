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
        auto bullets = _bullet_manager.bullets();
        for (auto i = 0u; i < bullets->size(); i++)
        {
            auto bullet = (*bullets)[i].get();
            if (bullet != nullptr)
            {
                bullet->update(dt);
                // TODO
                if (_player != nullptr && bullet->owner() != _player->id() && _player->hitbox().intersects(bullet->hitbox()))
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
    void GameSystem::draw(sf::RenderWindow &window)
    {
        _bullet_manager.draw(window);
        if (_player != nullptr)
        {
            window.draw(*_player.get());
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
        _bullet_manager.add_src_pattern_bullet(pattern_bullet, "straight_blue_circle");
        
        pattern_bullet = new PatternBullet(*bullet_blue, 1.0f);
        pattern_bullet->pattern_type(0u);
        _bullet_manager.add_src_pattern_bullet(pattern_bullet, "straight_blue");

        pattern_bullet = new PatternBullet(*bullet_blue_circle, 1.0f);

        pattern_bullet = new PatternBullet(*bullet_blue_circle, 1.0f);
        pattern_bullet->pattern_type(1u);
        _bullet_manager.add_src_pattern_bullet(pattern_bullet, "arena_blue");
    }

    void GameSystem::player(Player *value)
    {
        _player = std::unique_ptr<Player>(value);
        _player->id(++_id_counter);
    }
    Player *GameSystem::player() const
    {
        return _player.get();
    }
}
