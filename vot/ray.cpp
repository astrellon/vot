#include "ray.h"

namespace vot
{
    Ray::Ray(const sf::Vector2f &origin, const sf::Vector2f &direction, float max_length) :
        _origin(origin),
        _direction(direction),
        _max_length(max_length)
    {

    }
}