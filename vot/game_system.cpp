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
                if (_player != nullptr && _player->hitbox().intersects(bullet->hitbox()))
                {
                    _player->take_damage(bullet->damage());
                    _bullet_manager.remove_bullet(bullet);
                }
                if (bullet->dead())
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
        auto pattern_bullet = new PatternBullet(*bullet_blue_circle, 1.0f);
        _bullet_manager.add_src_pattern_bullet(pattern_bullet, "bullet_blue_circle");
    }

    void GameSystem::player(Player *value)
    {
        _player = std::unique_ptr<Player>(value);
    }
    Player *GameSystem::player() const
    {
        return _player.get();
    }
}
