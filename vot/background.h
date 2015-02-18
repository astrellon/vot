#pragma once

#include <SFML/Graphics.hpp>

namespace vot
{
    class Background : public sf::Drawable, public sf::Transformable
    {
        public:
            Background();

            void create();

            void speed(float value);
            float speed() const;

            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
            void update(float dt);

        private:
            sf::Sprite _sprite;
            sf::RenderTexture _target;
            float _speed;
    };
}
