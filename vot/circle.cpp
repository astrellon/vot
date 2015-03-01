#include "circle.h"

namespace vot
{
    Circle::Circle() :
        _radius(1.0f),
        _radius_squared(1.0f)
    {

    }
    Circle::Circle(float radius) :
        _radius(radius),
        _radius_squared(radius * radius)
    {

    }
    Circle::Circle(const sf::Vector2f &location, float radius) :
        _location(location),
        _radius(radius),
        _radius_squared(radius * radius)
    {

    }
    Circle::Circle(const Circle &clone) :
        _location(clone._location),
        _radius(clone._radius),
        _radius_squared(clone._radius_squared)
    {

    }

    bool Circle::intersects(const Circle &circle)
    {
        auto dx = _location.x - circle._location.x;
        auto dy = _location.y - circle._location.y;
        auto distance = dx * dx + dy * dy;

        return distance - _radius_squared - circle._radius_squared < 0.0f;
    }

    void Circle::draw(sf::RenderTarget &target, sf::RenderStates state) const
    {
        sf::CircleShape shape(_radius);
        shape.setPosition(_location);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(_radius, _radius);
        target.draw(shape, state);
    }
}
