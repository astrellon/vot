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
            Circle(const Circle &clone);

            void translate(const sf::Vector2f &value);
            
            inline void location(const sf::Vector2f &value)
            {
                _location = value;
            }
            inline sf::Vector2f location() const
            {
                return _location;
            }

            inline void radius(float value)
            {
                _radius = value;
                _radius_squared = value * value;
                _radius_inv = 1.0f / value;
            }
            inline float radius() const
            {
                return _radius;
            }
            inline float radius_squared() const
            {
                return _radius_squared;
            }
            inline float radius_inv() const
            {
                return _radius_inv;
            }

            bool intersects(const Circle &circle);

            virtual void draw(sf::RenderTarget &target, sf::RenderStates state) const;

        private:
            sf::Vector2f _location;
            float _radius;
            float _radius_squared;
            float _radius_inv;
    };
}
