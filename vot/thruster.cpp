#include "thruster.h"

#include "character.h"
#include "utils.h"
#include "game_system.h"
#include "texture_manager.h"

#include <stdint.h>
#include <iostream>

namespace vot
{
    Thruster::Thruster() :
        _parent(nullptr),
        _thrust_size(0.5f),
        _thrust_amount(0.0f),
        _max_thrust(1000.0f)
    {
        auto thrust = TextureManager::texture("bullet_blue_circle");
        _system = GameSystem::main()->particle_manager().spawn_system(*thrust, 20u);
        _system->system_type(2u);
        _system->loop_system(true);
        _system->spawn_rate(0.025f);
        _system->spawning_active(false);
        _system->init();
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

    void Thruster::max_thrust(float size)
    {
        _max_thrust = size;
    }
    float Thruster::max_thrust() const
    {
        return _max_thrust;
    }

    sf::Vector2f Thruster::forwards() const
    {
        auto direction = Utils::transform_direction(getTransform(), sf::Vector2f(0, 1));
        return Utils::vector_unit(direction);
    }

    void Thruster::calc_thrust(const sf::Vector2f &acc, float rot_acc)
    {
        auto direction = forwards();
        auto dot = Utils::vector_dot(acc, direction);

        auto amount = 0.0f;
        if (dot > 0)
        {
            amount += dot / _max_thrust;
        }

        auto to_parent = Utils::vector_unit(getPosition());
        auto cross = Utils::vector_cross_amount(to_parent, direction);
        if ((cross < 0 && rot_acc < 0) || (cross > 0 && rot_acc > 0))
        {
            amount += Utils::abs(cross);
        }

        thrust_amount(amount);
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
        auto trans = parent()->getTransform() * getTransform();
        auto global_position = trans.transformPoint(sf::Vector2f(0, 0));
        auto global_rotation = getRotation() + parent()->getRotation();
        _system->spawn_rotation_offset(Utils::degrees_to_radians * (global_rotation - 90.0f));
        _system->scale_factor(scale);
        _system->spawning_active(_thrust_amount > 0.40f);
        _system->setPosition(global_position);
    }
}
