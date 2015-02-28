#pragma once

#include <SFML/Graphics.hpp>

#include "utils.h"

namespace vot
{
    class Ray
    {
    public:
        Ray(const sf::Vector2f &origin = sf::Vector2f(0.0f, 0.0f), 
            const sf::Vector2f &direction = sf::Vector2f(1.0f, 0.0));
        Ray(const sf::Vector2f &origin = sf::Vector2f(0.0f, 0.0f),
            float angle = 0.0f);

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
            _rotation = Utils::vector_degrees(value);
        }
        inline sf::Vector2f direction() const
        {
            return _direction;
        }

        inline void rotation(float value)
        {
            _rotation = value;
            _direction = Utils::degrees_vector(value);
        }
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