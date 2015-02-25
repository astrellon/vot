#pragma once

#include <SFML/Graphics.hpp>

#include "character.h"
#include "powerup.h"

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
            
            void target(Enemy *value);
            Enemy *target() const;

            bool looking_at_target() const;
            bool auto_target() const;

            void add_powerup(const Powerup &powerup);

        private:
            float _cooldown;
            float _homing_cooldown;

            PatternBullet *spawn_pattern_bullet();
            HomingBullet *spawn_homing_bullet();
            Enemy *_target;

            std::map<Powerup::Type, int32_t> _powerups;

            bool _look_at_target;
            bool _auto_target;
    };
}
