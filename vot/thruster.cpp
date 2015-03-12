#include "thruster.h"

#include "character.h"
#include "utils.h"

#include <stdint.h>
#include <iostream>

namespace vot
{
    Thruster::Thruster() :
        _parent(nullptr),
        _thrust_size(0.5f),
        _thrust_amount(0.0f)
    {

    }

    void Thruster::parent(Character *character)
    {
        _parent = character;
    }
    Character *Thruster::parent() const
    {
        return _parent;
    }

    void Thruster::thrust_size(float size)
    {
        _thrust_size = size;
    }
    float Thruster::thrust_size() const
    {
        return _thrust_size;
    }

    void Thruster::calc_thrust(const sf::Vector2f &acc)
    {
        auto direction = Utils::transform_direction(_parent->getTransform() * getTransform(), sf::Vector2f(0, 1));
        auto dot = Utils::vector_dot(acc, direction);
        if (dot > 0)
        {
            thrust_amount(dot);
        }
        else
        {
            thrust_amount(0.0f);
        }
    }
    void Thruster::thrust_amount(float amount)
    {
        _thrust_amount = amount;
        update_sprite();
    }
    float Thruster::thrust_amount() const
    {
        return _thrust_amount;
    }

    void Thruster::update_sprite()
    {
        auto scale = _thrust_amount * _thrust_size + _thrust_size;
        auto alpha = static_cast<uint8_t>(_thrust_amount * 255.0f);

        setScale(scale, scale);
        setColor(sf::Color(255u, 255u, 255u, alpha));
    }
}
