#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>

#include "bullet.h"
#include "enemy.h"
#include "powerup.h"
#include "particles.h"
#include "beam.h"
#include "game.h"

namespace vot
{
    class GameSystem
    {
        public:
            static bool init(sf::RenderWindow &window);
            static void deinit();

            static sf::RenderWindow &window();

            static BulletManager *bullet_manager();
            static EnemyManager *enemy_manager();
            static ParticleSystemManager *particle_manager();
            static PowerupManager *powerup_manager();
            static BeamManager *beam_manager();

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

        private:
            static sf::RenderWindow *s_window;

            static BulletManager s_bullet_manager;
            static EnemyManager s_enemy_manager;
            static ParticleSystemManager s_particle_manager;
            static PowerupManager s_powerup_manager; 
            static BeamManager s_beam_manager;

            static std::unique_ptr<Game> s_game;

            static uint32_t s_update_counter;
            static uint32_t s_keys_pressed[sf::Keyboard::KeyCount];
            static uint32_t s_keys_released[sf::Keyboard::KeyCount];
            
            static void create_default_bullets();
            static void create_default_enemies();
            static void create_default_powerups();
            static void create_default_beams();
            
            static void on_resize(uint32_t width, uint32_t height);
            static void key_pressed(sf::Keyboard::Key key);
            static void key_released(sf::Keyboard::Key key);

    };
}
