#pragma once

#include <SFML/Graphics.hpp>

#include "circle.h"

namespace vot
{
    class Player : public sf::Drawable
    {
        public:
            Player(const sf::Texture &texture);

            virtual void draw(sf::RenderTarget &target, sf::RenderStates state) const;

            void translate(const sf::Vector2f &vector);
            void location(const sf::Vector2f &vector);
            sf::Vector2f location() const;

            void rotateBy(float angle);
            void rotation(float angle);
            float rotation() const;

            Circle &hitbox();
            void take_damage(float damage);

            void is_dead(bool value);
            bool is_dead() const;

            void life(float value);
            float life() const;

            void max_life(float value);
            float max_life() const;

        private:
            sf::Sprite _sprite;
            Circle _hitbox;
            float _life;
            float _max_life;
            bool _is_dead;

    };
}
