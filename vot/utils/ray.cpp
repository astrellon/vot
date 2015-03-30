#include "ray.h"

#include "utils.h"

namespace vot
{
    namespace utils
    {
        Ray::Ray() :
            _direction(1.0f, 0.0f),
            _rotation(0.0f)
        {
        }
        Ray::Ray(const sf::Vector2f &origin) :
            _origin(origin),
            _direction(1.0f, 0.0f),
            _rotation(0.0f)
        {
        }
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

        void Ray::direction(const sf::Vector2f &value)
        {
            _direction = value;
            _rotation = Utils::vector_degrees(value);
        }
        void Ray::rotation(float value)
        {
            _rotation = value;
            _direction = Utils::degrees_vector(value);
        }
    }
}
