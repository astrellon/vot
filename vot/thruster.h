#pragma once

#include <SFML/Graphics.hpp>

namespace vot
{
    class Character;

    class Thruster : public sf::Sprite
    {
        public:
            Thruster();

            void parent(Character *character);
            Character *parent() const;

            void thrust_size(float size);
            float thrust_size() const;

            void calc_thrust(const sf::Vector2f &acc, float rot_acc);
            void thrust_amount(float amount);
            float thrust_amount() const;

        private:
            Character *_parent;
            float _thrust_size;
            float _thrust_amount;

            void update_sprite();
    };
}
