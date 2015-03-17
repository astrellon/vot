#pragma once

#include <SFML/Graphics.hpp>

#include "particles.h"

namespace vot
{
    class Character;
    class ParticleSystem;

    class Thruster : public sf::Transformable
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

            void max_thrust(float amount);
            float max_thrust() const;

            sf::Vector2f forwards() const;

        private:
            Character *_parent;
            float _thrust_size;
            float _thrust_amount;
            float _max_thrust;
            ParticleSystem *_system;

            void update_sprite();
    };
}
