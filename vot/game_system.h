#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <memory>
#include <random>

#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include "game_hud.h"
#include "background.h"
#include "particles.h"
#include "powerup.h"
#include "beam.h"

namespace vot
{
    class GameSystem : public sf::Drawable
    {
        public:
            GameSystem(sf::RenderWindow &window);

            void init();

            sf::RenderWindow &window() const;

            BulletManager &bullet_manager();
            void create_default_bullets();

            EnemyManager &enemy_manager();
            void create_default_enemies();

            ParticleSystemManager &particle_manager();

            PowerupManager &powerup_manager();
            void create_default_powerups();

            BeamManager &beam_manager();
            void create_default_beams();

            void player(Player *value);
            Player *player() const;

            sf::View &camera();

            Enemy *next_target(Enemy *current);

            void on_resize(uint32_t width, uint32_t height);

            void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

            enum KeyState
            {
                NO_CHANGE,
                PRESSED,
                RELEASED
            };

            void key_pressed(sf::Keyboard::Key key);
            bool is_key_pressed(sf::Keyboard::Key key) const;
            void key_released(sf::Keyboard::Key key);
            bool is_key_released(sf::Keyboard::Key key) const;

            static void main(GameSystem *main);
            static GameSystem *main();

        private:
            sf::RenderWindow &_window;
            sf::View _camera;
            sf::View _hud_camera;

            BulletManager _bullet_manager;
            std::unique_ptr<Player> _player;
            EnemyManager _enemy_manager;
            ParticleSystemManager _particle_manager;
            PowerupManager _powerup_manager; 
            BeamManager _beam_manager;

            HudMain _hud;
            HudWorld _world_hud;

            Background _background;
            Background _background2;
            Background _background3;

            float _spawn_timer;

            uint32_t _update_counter;

            uint32_t _keys_pressed[sf::Keyboard::KeyCount];
            uint32_t _keys_released[sf::Keyboard::KeyCount];

            static GameSystem *s_main;

            void kill_enemy(Enemy *enemy);
            void bullet_hit_particles(Bullet *bullet, Character *hit, const std::string &texture);
            void beam_hit_particles(const sf::Vector2f &point, const sf::Vector2f &normal, const std::string &texture);

    };
}
