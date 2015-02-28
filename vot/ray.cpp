#include "ray.h"

namespace vot
{
    Ray::Ray(const sf::Vector2f &origin, const sf::Vector2f &direction) :
        _origin(origin),
        _direction(direction),
        _rotation(Utils::vector_degrees(direction))
    {
    }

    Ray::Ray(const sf::Vector2f &origin, float angle) :
        _origin(origin),
        _direction(Utils::degrees_vector(angle)),
        _rotation(angle)
    {
    }
}