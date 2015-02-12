#pragma once

#include <SFML/Graphics.hpp>

#include "character.h"

namespace vot
{
    class Enemy : public Character
    {
        public:
            Enemy(const sf::Texture &texture);
            void update(float dt);

            void enemy_type(uint32_t value);
            uint32_t enemy_type() const;

        private:
            uint32_t _enemy_type;
            uint32_t _bullet_count;
            float _firing_angle;
            float _cooldown;
    };
}
