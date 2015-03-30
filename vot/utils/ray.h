#pragma once

#include <SFML/Graphics.hpp>

namespace vot
{
    namespace utils
    {
        class Ray
        {
            public:
                Ray();
                Ray(const sf::Vector2f &origin);
                Ray(const sf::Vector2f &origin, const sf::Vector2f &direction);
                Ray(const sf::Vector2f &origin, float);

                inline void origin(const sf::Vector2f &value)
                {
                    _origin = value;
                }
                inline sf::Vector2f origin() const
                {
                    return _origin;
                }

                void direction(const sf::Vector2f &value);
                inline sf::Vector2f direction() const
                {
                    return _direction;
                }

                void rotation(float value);
                inline float rotation() const
                {
                    return _rotation;
                }

            private:
                sf::Vector2f _origin;
                sf::Vector2f _direction;
                float _rotation;
        };
    }
}
