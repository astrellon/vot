#pragma once

#include <SFML/Graphics.hpp>

namespace vot
{
    class Ray
    {
    public:
        Ray(const sf::Vector2f &origin = sf::Vector2f(0.0f, 0.0f), 
            const sf::Vector2f &direction = sf::Vector2f(1.0f, 0.0), 
            float max_length = 100.0f);

        inline void origin(const sf::Vector2f &value)
        {
            _origin = value;
        }
        inline sf::Vector2f origin() const
        {
            return _origin;
        }

        inline void direction(const sf::Vector2f &value)
        {
            _direction = value;
        }
        inline sf::Vector2f direction() const
        {
            return _direction;
        }
        
        inline void max_length(float value)
        {
            _max_length = value;
            _max_length_squared = value * value;
        }
        inline float max_length() const
        {
            return _max_length;
        }
        inline float max_length_squared() const
        {
            return _max_length_squared;
        }

    private:
        sf::Vector2f _origin;
        sf::Vector2f _direction;
        float _max_length;
        float _max_length_squared;
    };
}