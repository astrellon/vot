#pragma once

#include <SFML/Graphics.hpp>

#include "character.h"

namespace vot
{
    class Player : public Character
    {
        public:
            Player(const sf::Texture &texture);
            virtual void update(float dt);

        private:
            float _cooldown;
    };
}
