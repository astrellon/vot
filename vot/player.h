#pragma once

#include <SFML/Graphics.hpp>

#include "character.h"

namespace vot
{
    class PatternBullet;

    class Player : public Character
    {
        public:
            Player(const sf::Texture &texture);
            virtual void update(float dt);

        private:
            float _cooldown;

            PatternBullet *spawn_bullet();
    };
}
