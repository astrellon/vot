#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>

#include "bullet.h"
#include "enemy.h"
#include "powerup.h"
#include "particles.h"
#include "beam.h"
#include "game.h"
#include "sound.h"
#include "hardpoint.h"

namespace vot
{
    class GameSystem
    {
        public:
            static bool init(sf::RenderWindow &window);
            static void deinit();

            static sf::RenderWindow *window();

            static BulletManager *bullet_manager();
            static EnemyManager *enemy_manager();
            static ParticleSystemManager *particle_manager();
            static PowerupManager *powerup_manager();
            static BeamManager *beam_manager();
            static SoundManager *sound_manager();
            static HardpointManager *hardpoint_manager();

            static void update(float dt);
            static void draw(sf::RenderTarget &target, sf::RenderStates states);

            enum KeyState
            {
                NO_CHANGE,
                PRESSED,
                RELEASED
            };

            static void process_event(const sf::Event &event);

            static bool is_key_pressed(sf::Keyboard::Key key);
            static bool is_key_released(sf::Keyboard::Key key);

            static void game(Game *value);
            static Game *game();

            static void start_game(const std::string &level);
            static void close_game();

            static float time_since_start();
            
            static sf::View &hud_camera();
            static sf::Vector2i mouse_position();
            static sf::Vector2u window_size();

        private:
            static sf::RenderWindow *s_window;

            static BulletManager s_bullet_manager;
            static EnemyManager s_enemy_manager;
            static ParticleSystemManager s_particle_manager;
            static PowerupManager s_powerup_manager; 
            static BeamManager s_beam_manager;
            static SoundManager s_sound_manager;
            static HardpointManager s_hardpoint_manager;

            static sf::View s_hud_camera;
            static std::unique_ptr<Game> s_game;

            static float s_time_since_start;

            static uint32_t s_update_counter;
            static uint32_t s_keys_pressed[sf::Keyboard::KeyCount];
            static uint32_t s_keys_released[sf::Keyboard::KeyCount];
            
            static void create_default_bullets();
            static void create_default_enemies();
            static void create_default_powerups();
            static void create_default_beams();
            static void create_default_hardpoints();
            
            static void on_resize(uint32_t width, uint32_t height);
            static void key_pressed(sf::Keyboard::Key key);
            static void key_released(sf::Keyboard::Key key);

    };
}
