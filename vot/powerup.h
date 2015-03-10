#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <vector>
#include <memory>

#include "circle.h"

namespace vot
{
    // Powerup {{{
    class Powerup : public sf::Sprite
    {
        public:
            enum Type
            {
                NONE,
                BULLET,
                HOMING,
                BEAM
            };

            Powerup(const sf::Texture &texture, Type type, int32_t value);
            Powerup(const Powerup &clone);

            Type type() const;
            int32_t value() const;

            void location(const sf::Vector2f &location); 

            Circle &hitbox();
            const Circle &hitbox() const;

            void update(float dt);

        private:
            Type _type;
            int32_t _value;
            Circle _hitbox;

            float _lifetime; 
    };
    // }}}
    
    // PowerupManager {{{
    class PowerupManager : public sf::Drawable
    {
        public:
            Powerup *spawn_powerup(const std::string &name);
            void add_src_powerup(const std::string &name, Powerup *powerup);
            void remove_powerup(Powerup *powerup);

            typedef std::vector<std::unique_ptr<Powerup> > PowerupList; 
            const PowerupList *active_powerups() const;

            void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
        private:
            PowerupList  _powerups;
            std::map<std::string, std::unique_ptr<Powerup> > _src_powerups;
    };
    // }}}
}
