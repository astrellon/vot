#pragma once

#include <SFML/Graphics.hpp>

#include "character.h"

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

        private:
            float _cooldown;
            float _homing_cooldown;

            PatternBullet *spawn_pattern_bullet();
            HomingBullet *spawn_homing_bullet();
            Enemy *_target;

            bool _look_at_target;
            bool _auto_target;
    };
}
