#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <memory>

#include "player.h"
#include "player_info.h"
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

            void player_info(PlayerInfo *value);
            PlayerInfo *player_info() const;

            sf::View &camera();

            void level(Level *lvl);
            Level *level() const;

            void paused(bool value);
            bool paused() const;

            Enemy *next_target(Enemy *current);

            void on_resize(uint32_t width, uint32_t height);

            void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            sf::View _camera;
            
            std::unique_ptr<Player> _player;
            std::unique_ptr<PlayerInfo> _player_info;
            
            HudMain _hud;
            HudWorld _world_hud;

            Background _background;
            Background _background2;
            Background _background3;

            Level *_current_level;

            bool _paused;

            void kill_enemy(Enemy *enemy);
            void bullet_hit_particles(Bullet *bullet, Character *hit, const std::string &texture);
            void beam_hit_particles(const sf::Vector2f &point, const sf::Vector2f &normal, const std::string &texture);
    };
}
