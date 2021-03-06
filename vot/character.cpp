#include "character.h"

#include <iostream>

#include "utils/utils.h"
#include "hardpoint.h"

namespace vot
{
    Character::Character(const sf::Texture &texture) :
        _sprite(texture),
        _hitbox(5.0f),
        _life(10.0f),
        _max_life(10.0f),
        _is_dead(false),
        _id(0u),
        _max_speed(250.0f),
        _strafe_speed(800.0f),
        _forwards_speed(2000.0f),
        _backwards_speed(1200.0f),
        _rot_acceleration(0.0f),
        _rot_velocity(0.0f),
        _rot_speed(360.0f),
        _max_rot_speed(180.0f),
        _translate_assist(true),
        _rotation_assist(true),
        _group(Group::ENEMY)
    {
        auto size = texture.getSize();
        _sprite.setOrigin(size.x * 0.5f, size.y * 0.5f);
    }
    Character::Character(const Character &clone) :
        _sprite(clone._sprite),
        _hitbox(clone._hitbox),
        _life(clone._life),
        _max_life(clone._max_life),
        _is_dead(clone._is_dead),
        _id(0u),
        _max_speed(clone._max_speed),
        _strafe_speed(clone._strafe_speed),
        _forwards_speed(clone._forwards_speed),
        _backwards_speed(clone._backwards_speed),
        _rot_acceleration(0.0f),
        _rot_velocity(0.0f),
        _rot_speed(clone._rot_speed),
        _max_rot_speed(clone._max_rot_speed),
        _translate_assist(clone._translate_assist),
        _rotation_assist(clone._rotation_assist),
        _group(clone._group)
    {

    }

    void Character::translate(const sf::Vector2f &vector)
    {
        // Transform move vector by the sprites rotation matrix.
        auto local_direction = utils::Utils::transform_direction(getTransform(), vector);
        move(local_direction);
    }
    void Character::location(const sf::Vector2f &vector)
    {
        setPosition(vector);
    }
    sf::Vector2f Character::location() const
    {
        return getPosition();
    }
    sf::Vector2f Character::center() const
    {
        auto pos = getPosition();
        auto size = _sprite.getTexture()->getSize();
        return sf::Vector2f(pos.x - size.x * 0.5f, pos.y - size.y * 0.5f);
    }

    void Character::rotateBy(float angle)
    {
        rotate(angle);
    }
    void Character::rotation(float angle)
    {
        setRotation(angle);
    }
    float Character::rotation() const
    {
        return getRotation();
    }

    void Character::update(float dt)
    {
        // Update placements {{{
        for (auto &iter : _placements)
        {
            if (iter.second.get()->hardpoint() != nullptr)
            {
                iter.second.get()->hardpoint()->update(dt);
            }
        }
        // }}}

        // Velocity {{{
        if (_translate_assist && _acceleration.x == 0.0f && _acceleration.y == 0.0f)
        {
            if (utils::Utils::vector_dot(_velocity, _velocity) < 2.0f)
            {
                _velocity.x = _velocity.y = 0.0f;
            }
            else
            {
                auto local_velocity = utils::Utils::transform_direction(getInverseTransform(), _velocity); 
                _acceleration = local_velocity / -dt;
                if (_acceleration.x > _strafe_speed)
                {
                    _acceleration.x = _strafe_speed;
                }
                if (_acceleration.x < -_strafe_speed)
                {
                    _acceleration.x = -_strafe_speed;
                }
                if (_acceleration.y > _backwards_speed)
                {
                    _acceleration.y = _backwards_speed;
                }
                if (_acceleration.y < -_forwards_speed)
                {
                    _acceleration.y = -_forwards_speed;
                }
            }
        }
        auto world_acceleration = utils::Utils::transform_direction(getTransform(), _acceleration);

        _velocity += world_acceleration * dt;

        auto length = utils::Utils::vector_length(_velocity);
        if (length > _max_speed)
        {
            _velocity /= length;
            _velocity *= _max_speed;
        }

        auto dt_velocity = _velocity * dt;
        if (_acceleration.x == 0.0f && _acceleration.y == 0.0f && 
                _translate_assist && utils::Utils::vector_dot(_velocity, _velocity) < 10.0f)
        {
            _velocity.x = _velocity.y = 0.0f;
            dt_velocity.x = dt_velocity.y = 0.0f;
        }
        move(dt_velocity);

        _hitbox.location(getPosition());

        /*
        auto unit_acc = _acceleration;
        auto len_acc = utils::Utils::vector_length(world_acceleration);
        if (len_acc > 0.0f)
        {
            unit_acc /= len_acc;
        }
        */
        // }}}

        // Rotational velocity {{{
        if (_rot_acceleration == 0.0f && _rotation_assist)
        {
            if (utils::Utils::abs(_rot_velocity) < 0.5f)
            {
                _rot_velocity = 0.0f;
            }
            else
            {
                _rot_acceleration = _rot_velocity > 0.0f ? -_rot_speed : (_rot_velocity < 0.0f ? _rot_speed : 0.0f);  
            }
        }

        _rot_velocity += _rot_acceleration * dt;
        if (_rot_velocity > _max_rot_speed)  { _rot_velocity = _max_rot_speed; }
        if (_rot_velocity < -_max_rot_speed) { _rot_velocity = -_max_rot_speed; }

        auto dt_rot_velocity = _rot_velocity * dt;
        rotateBy(dt_rot_velocity);
        // }}}

        // Update thrusters {{{
        for (auto i = 0u; i < _thrusters.size(); i++)
        {
            auto thruster = _thrusters[i].get();
            thruster->calc_thrust(_acceleration, _rot_acceleration);
        }
        // }}}
    }
    void Character::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(_sprite, states);
        
        /*
        sf::RectangleShape line;
        line.setSize(sf::Vector2f(_sprite.getTexture()->getSize()));
        line.setFillColor(sf::Color::Transparent);
        line.setOutlineThickness(2.0f);
        line.setOutlineColor(sf::Color::Red);

        state.transform *= _sprite.getTransform();
        target.draw(line, state);
        */

        for (const auto &iter : _placements)
        {
            if (iter.second.get()->hardpoint() != nullptr)
            {
                target.draw(*iter.second.get()->hardpoint(), states);
            }
        }
    }

    utils::Circle &Character::hitbox()
    {
        return _hitbox;
    }

    void Character::is_dead(bool value)
    {
        _is_dead = value;
    }
    bool Character::is_dead() const
    {
        return _is_dead;
    }

    void Character::take_damage(float damage)
    {
        _life -= damage;
        if (_life <= 0.0f)
        {
            _life = 0.0f;
            _is_dead = true;
        }
    }

    void Character::life(float value)
    {
        _life = value;
    }
    float Character::life() const
    {
        return _life;
    }
    
    void Character::max_life(float value)
    {
        _max_life = value;
    }
    float Character::max_life() const
    {
        return _max_life;
    }

    void Character::id(uint16_t value)
    {
        _id = value;
    }
    uint16_t Character::id() const
    {
        return _id;
    }

    sf::Sprite &Character::sprite()
    {
        return _sprite;
    }
    const sf::Sprite &Character::sprite() const
    {
        return _sprite;
    }
    
    const Character::HardpointPlacementMap *Character::placements() const
    {
        return &_placements;
    }
    void Character::add_placement(HardpointPlacement *point)
    {
        point->parent(this);
        _placements[point->name()] = std::unique_ptr<HardpointPlacement>(point);
    }
    void Character::remove_placement(HardpointPlacement *point)
    {
        point->parent(nullptr);
        for (auto iter = _placements.cbegin(); iter != _placements.cend(); ++iter)
        {
            if (iter->second.get() == point)
            {
                _placements.erase(iter);
                return;
            }
        }
    }
    void Character::clear_placements()
    {
        _placements.clear();
    }
    void Character::clear_placement_hardpoints()
    {
        for (auto &iter : _placements)
        {
            iter.second.get()->hardpoint(nullptr);
        }
    }
    
    void Character::add_hardpoint_to_placement( const std::string &name, vot::Hardpoint *point )
    {
        auto find = _placements.find(name);
        if (find != _placements.end())
        {
            find->second->hardpoint(point);
        }
    }

    const Character::ThrusterList *Character::thrusters() const
    {
        return &_thrusters;
    }
    void Character::add_thruster(Thruster *thruster)
    {
        thruster->parent(this);
        auto direction = thruster->forwards();
        auto dot = utils::Utils::vector_dot(direction, sf::Vector2f(0, 1));
        auto max_thrust = 0.0f;
        if (dot > 0.0f)
        {
            max_thrust += dot * backwards_speed();
        }
        else if (dot < 0.0f)
        {
            max_thrust += -dot * forwards_speed();
        }

        dot = utils::Utils::vector_dot(direction, sf::Vector2f(1, 0));
        max_thrust += utils::Utils::abs(dot) * strafe_speed();

        thruster->max_thrust(max_thrust);

        _thrusters.push_back(std::unique_ptr<Thruster>(thruster));
    }

    void Character::acceleration(const sf::Vector2f &acc)
    {
        //_acceleration = utils::Utils::transform_direction(getTransform(), acc);
        _acceleration = acc;
    }
    sf::Vector2f Character::acceleration() const
    {
        return _acceleration;
    }

    sf::Vector2f Character::velocity() const
    {
        return _velocity;
    }

    void Character::max_speed(float speed)
    {
        _max_speed = speed;
    }
    float Character::max_speed() const
    {
        return _max_speed;
    }

    void Character::strafe_speed(float speed)
    {
        _strafe_speed = speed;
    }
    float Character::strafe_speed() const
    {
        return _strafe_speed;
    }

    void Character::forwards_speed(float speed)
    {
        _forwards_speed = speed;
    }
    float Character::forwards_speed() const
    {
        return _forwards_speed;
    }

    void Character::backwards_speed(float speed)
    {
        _backwards_speed = speed;
    }
    float Character::backwards_speed() const
    {
        return _backwards_speed;
    }

    void Character::rot_acceleration(float acc)
    {
        _rot_acceleration = acc;
    }
    float Character::rot_acceleration() const
    {
        return _rot_acceleration;
    }

    float Character::rot_velocity() const
    {
        return _rot_velocity;
    }

    void Character::rot_speed(float speed)
    {
        _rot_speed = speed;
    }
    float Character::rot_speed() const
    {
        return _rot_speed;
    }

    void Character::max_rot_speed(float speed)
    {
        _max_rot_speed = speed;
    }
    float Character::max_rot_speed() const
    {
        return _max_rot_speed;
    }

    void Character::translate_assist(bool assist)
    {
        _translate_assist = assist;
    }
    bool Character::translate_assist() const
    {
        return _translate_assist;
    }

    void Character::rotation_assist(bool assist)
    {
        _rotation_assist = assist;
    }
    bool Character::rotation_assist() const
    {
        return _rotation_assist;
    }

    void Character::group(Group::Type value)
    {
        _group = value;
    }
    Group::Type Character::group() const
    {
        return _group;
    }
}
