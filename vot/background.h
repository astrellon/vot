#pragma once

#include <SFML/Graphics.hpp>

namespace vot
{
    class Background : public sf::Drawable, public sf::Transformable
    {
        public:
            Background(float speed, const sf::View &camera);

            const sf::View &camera() const;

            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
            void update(float dt);

        private:
            const sf::View &_camera;
            sf::Sprite _sprite;
            sf::RenderTexture _target;
            float _speed;
    };
}
