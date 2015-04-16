#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <memory>

#include "player.h"
#include "profile.h"
#include "game_hud.h"
#include "background.h"

namespace vot
{
    class Level;

    class Game : public sf::Drawable
    {
        public:
            Game();

            void player(Player *value);
            Player *player() const;

            void profile(Profile *value);
            Profile *profile() const;

            sf::View &camera();

            void level(Level *lvl);
            Level *level() const;

            void paused(bool value);
            bool paused() const;

            void time_scale(float value);
            float time_scale() const;

            Enemy *next_target(Enemy *current);

            void on_resize(uint32_t width, uint32_t height);

            void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            sf::View _camera;
            
            std::unique_ptr<Player> _player;
            std::unique_ptr<Profile> _profile;
            
            HudMain _hud;
            HudWorld _world_hud;

            Background _background;
            Background _background2;
            Background _background3;

            Level *_current_level;

            bool _paused;
            float _time_scale;

            void kill_enemy(Enemy *enemy);
            void bullet_hit_particles(Bullet *bullet, Character *hit, const std::string &texture);
            void beam_hit_particles(const sf::Vector2f &point, const sf::Vector2f &normal, const std::string &texture);
    };
}
