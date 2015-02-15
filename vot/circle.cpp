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

    void Circle::location(const sf::Vector2f &value)
    {
        _location = value;
    }
    sf::Vector2f &Circle::location()
    {
        return _location;
    }
    const sf::Vector2f &Circle::location() const
    {
        return _location;
    }

    void Circle::radius(float value)
    {
        _radius = value;
        _radius_squared = value * value;
    }
    float Circle::radius() const
    {
        return _radius;
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
