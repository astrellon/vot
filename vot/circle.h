#pragma once

#include <SFML/Graphics.hpp>

namespace vot
{
    class Circle : public sf::Drawable
    {
        public:
            Circle();
            Circle(float radius);
            Circle(const sf::Vector2f &location, float radius);

            void translate(const sf::Vector2f &value);
            void location(const sf::Vector2f &value);
            sf::Vector2f &location();
            const sf::Vector2f &location() const;

            void radius(float value);
            float radius() const;

            bool intersects(const Circle &circle);

            virtual void draw(sf::RenderTarget &target, sf::RenderStates state) const;

        private:
            sf::Vector2f _location;
            float _radius;
            float _radius_squared;
    };
}
