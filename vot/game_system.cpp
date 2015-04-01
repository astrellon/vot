#include "game_system.h"

#include <random>
#include <iostream>

#include "texture_manager.h"
#include "utils/utils.h"
#include "common.h"
#include "enemy_fighter.h"
#include "ui/main_menu.h"
#include "ui/level_select.h"
#include "levels/level.h"

namespace vot
{
    sf::RenderWindow *GameSystem::s_window = nullptr;

    BulletManager GameSystem::s_bullet_manager;
    EnemyManager GameSystem::s_enemy_manager;
    ParticleSystemManager GameSystem::s_particle_manager;
    PowerupManager GameSystem::s_powerup_manager;
    BeamManager GameSystem::s_beam_manager;
    SoundManager GameSystem::s_sound_manager;

    sf::View GameSystem::s_hud_camera;
    std::unique_ptr<Game> GameSystem::s_game;

    float GameSystem::s_time_since_start = 0.0f;;

    uint32_t GameSystem::s_update_counter = 0;
    uint32_t GameSystem::s_keys_pressed[sf::Keyboard::KeyCount];
    uint32_t GameSystem::s_keys_released[sf::Keyboard::KeyCount];

    bool GameSystem::init(sf::RenderWindow &window)
    {
        s_window = &window;
        s_hud_camera = window.getDefaultView();

        auto size = static_cast<int>(sf::Keyboard::KeyCount);
        for (auto i = 0; i < size; i++)
        {
            s_keys_pressed[i] = 0u;
            s_keys_released[i] = 0u;
        }
        
        create_default_bullets();
        create_default_enemies();
        create_default_powerups();
        create_default_beams();

        if (!s_sound_manager.init())
        {
            return false;
        }

        return true;
    }
    void GameSystem::deinit()
    {
        s_game = nullptr;
        s_sound_manager.clear();
    }

    sf::RenderWindow &GameSystem::window()
    {
        return *s_window;
    }

    void GameSystem::update(float dt)
    {
        s_update_counter++;
        if (s_game.get() != nullptr)
        {
            s_game->update(dt);
        }
        s_sound_manager.update(dt);

        s_time_since_start += dt;
    }

    void GameSystem::draw(sf::RenderTarget &target, sf::RenderStates states)
    {
        if (s_game.get() != nullptr)
        {
            s_game->draw(target, states);
        }
    }

    BulletManager *GameSystem::bullet_manager()
    {
        return &s_bullet_manager;
    }
    void GameSystem::create_default_bullets()
    {
        auto bullet_blue_circle = TextureManager::texture("bullet_blue_circle");
        auto bullet_red_circle = TextureManager::texture("bullet_red_circle");
        auto bullet_blue = TextureManager::texture("bullet_blue");

        TextureManager::display("Create bullets");
        
        auto pattern_bullet = new PatternBullet(*bullet_blue_circle, 1.0f);
        pattern_bullet->pattern_type(0u);
        pattern_bullet->hitbox().radius(5.0f);
        pattern_bullet->scale(0.5f);
        s_bullet_manager.add_src_pattern_bullet("straight_blue_circle", pattern_bullet);
        
        pattern_bullet = new PatternBullet(*bullet_blue, 1.0f);
        pattern_bullet->pattern_type(0u);
        pattern_bullet->hitbox().radius(5.0f);
        pattern_bullet->scale(0.5f);
        s_bullet_manager.add_src_pattern_bullet("player_bullet_small", pattern_bullet);
        
        pattern_bullet = new PatternBullet(*bullet_blue, 1.5f);
        pattern_bullet->pattern_type(0u);
        pattern_bullet->hitbox().radius(5.0f);
        pattern_bullet->scale(0.75f);
        s_bullet_manager.add_src_pattern_bullet("player_bullet_medium", pattern_bullet);
        
        pattern_bullet = new PatternBullet(*bullet_red_circle, 1.0f);
        pattern_bullet->pattern_type(0u);
        pattern_bullet->hitbox().radius(5.0f);
        s_bullet_manager.add_src_pattern_bullet("straight_red_circle", pattern_bullet);

        pattern_bullet = new PatternBullet(*bullet_blue_circle, 1.0f);
        pattern_bullet->pattern_type(1u);
        pattern_bullet->hitbox().radius(5.0f);
        s_bullet_manager.add_src_pattern_bullet("arena_blue", pattern_bullet);

        pattern_bullet = new PatternBullet(*bullet_blue_circle, 1.0f);
        pattern_bullet->pattern_type(10u);
        pattern_bullet->hitbox().radius(5.0f);
        s_bullet_manager.add_src_pattern_bullet("test", pattern_bullet);
        
        auto homing_bullet = new HomingBullet(*bullet_blue_circle, 2.0f);
        homing_bullet->total_lifetime(5.0f);
        homing_bullet->hitbox().radius(5.0f);
        homing_bullet->scale(0.75f);
        s_bullet_manager.add_src_homing_bullet("homing_blue", homing_bullet);
    }

    EnemyManager *GameSystem::enemy_manager()
    {
        return &s_enemy_manager;
    }
    void GameSystem::create_default_enemies()
    {
        auto texture = TextureManager::texture("enemy");
        auto enemy = new Enemy(*texture);
        enemy->sprite().setScale(0.5f, 0.5f);
        enemy->hitbox().radius(25.0f);
        s_enemy_manager.add_src_enemy("enemy1", enemy);
    }

    ParticleSystemManager *GameSystem::particle_manager()
    {
        return &s_particle_manager;
    }

    PowerupManager *GameSystem::powerup_manager()
    {
        return &s_powerup_manager;
    }
    void GameSystem::create_default_powerups()
    {
        auto bullet_powerup = TextureManager::texture("powerup_bullet");
        auto homing_powerup = TextureManager::texture("powerup_homing");
        auto beam_powerup = TextureManager::texture("powerup_beam");

        auto powerup = new Powerup(*bullet_powerup, Powerup::BULLET, 1);
        s_powerup_manager.add_src_powerup("bullet", powerup);

        powerup = new Powerup(*homing_powerup, Powerup::HOMING, 1);
        s_powerup_manager.add_src_powerup("homing", powerup);
        
        powerup = new Powerup(*beam_powerup, Powerup::BEAM, 1);
        s_powerup_manager.add_src_powerup("beam", powerup);
    }

    BeamManager *GameSystem::beam_manager()
    {
        return &s_beam_manager;
    }
    void GameSystem::create_default_beams()
    {
        auto beam = new Beam();
        beam->width(12.0f);
        beam->max_length(400.0f);
        beam->dps(2.0f);

        s_beam_manager.add_src_beam("beam1", beam);
    }

    SoundManager *GameSystem::sound_manager()
    {
        return &s_sound_manager;
    }

    void GameSystem::process_event(const sf::Event &event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            key_pressed(event.key.code);

            // Only toggle main menu when a game is in progress.
            if (event.key.code == sf::Keyboard::Escape)
            {
                if (s_game.get() != nullptr)
                {
                    ui::MainMenu::visible(!ui::MainMenu::visible());
                }
            }
        }
        if (event.type == sf::Event::KeyReleased)
        {
            key_released(event.key.code);
        }

        if (event.type == sf::Event::Resized)
        {
            on_resize(event.size.width, event.size.height);
            ui::MainMenu::on_resize(event.size.width, event.size.height);
            ui::LevelSelect::on_resize(event.size.width, event.size.height);
        }
    }

    void GameSystem::game(Game *value)
    {
        s_game = std::unique_ptr<Game>(value);
    }
    Game *GameSystem::game()
    {
        return s_game.get();
    }

    void GameSystem::start_game(const std::string &level)
    {
        auto g = new Game();
        g->level(LevelManager::level(level));
        game(g);
    }
    void GameSystem::close_game()
    {
        s_window->close();
    }

    float GameSystem::time_since_start()
    {
        return s_time_since_start;
    }

    void GameSystem::key_pressed(sf::Keyboard::Key key)
    {
        s_keys_pressed[key] = s_update_counter + 1;
    }
    bool GameSystem::is_key_pressed(sf::Keyboard::Key key)
    {
        return s_keys_pressed[key] == s_update_counter;
    }
    void GameSystem::key_released(sf::Keyboard::Key key)
    {
        s_keys_released[key] = s_update_counter + 1;
    }
    bool GameSystem::is_key_released(sf::Keyboard::Key key)
    {
        return s_keys_released[key] == s_update_counter;
    }

    void GameSystem::on_resize(uint32_t width, uint32_t height)
    {
        s_window->setSize(sf::Vector2u(width, height));
        
        auto fwidth = static_cast<float>(width);
        auto fheight = static_cast<float>(height);
        s_hud_camera.setSize(fwidth, fheight);
        s_hud_camera.setCenter(fwidth * 0.5f, fheight * 0.5f);

        if (s_game.get() != nullptr)
        {
            s_game->on_resize(width, height);
        }
    }

    sf::View &GameSystem::hud_camera()
    {
        return s_hud_camera;
    }
    sf::Vector2i GameSystem::mouse_position()
    {
        return sf::Mouse::getPosition(*s_window);
    }
    sf::Vector2u GameSystem::window_size()
    {
        return s_window->getSize();
    }
}
