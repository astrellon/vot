#pragma once

#include <SFML/Graphics.hpp>

#include "character.h"
#include "powerup.h"
#include "beam.h"
#include "hardpoint.h"
#include "thruster.h"

#include "utils/circle.h"

#include <map>

namespace vot
{
    class PatternBullet;
    class HomingBullet;
    class Enemy;

    class Player : public Character
    {
        public:
            Player(const sf::Texture &texture);
            virtual void update(float dt);

            void init();
            
            void target(Enemy *value);
            Enemy *target() const;

            bool looking_at_target() const;
            bool auto_target() const;

            utils::Circle &powerup_hitbox();

            void add_powerup(const Powerup &powerup);

            void add_thruster_placement(float x, float y, float rotation, float size);

            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            float _cooldown;
            float _homing_cooldown;
            bool _look_at_target;
            bool _auto_target;
            Enemy *_target;

            std::map<Powerup::Type, int32_t> _powerups;
            utils::Circle _powerup_hitbox;
            sf::Shader _shader;

            void create_new_hardpoint(Powerup::Type type);
    };
}
