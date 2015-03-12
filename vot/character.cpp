#include "character.h"

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
        _rot_acceleration(0.0f),
        _rot_velocity(0.0f),
        _max_rot_speed(90.0f)
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
        _rot_acceleration(0.0f),
        _rot_velocity(0.0f),
        _max_rot_speed(clone._max_rot_speed)
    {

    }

    void Character::translate(const sf::Vector2f &vector)
    {
        // Transform move vector by the sprites rotation matrix.
        auto local_direction = Utils::transform_direction(getTransform(), vector);
        move(local_direction);
        //_hitbox.location(getPosition());
    }
    void Character::location(const sf::Vector2f &vector)
    {
        setPosition(vector);
        //_hitbox.location(vector);
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
        for (auto i = 0u; i < _hardpoints.size(); i++)
        {
            auto hardpoint = _hardpoints[i].get(); 
            hardpoint->update(dt);
        }

        auto world_acceleration = Utils::transform_direction(getTransform(), _acceleration);
        _velocity += world_acceleration * dt;
        auto length = Utils::vector_length(_velocity);
        if (length > _max_speed)
        {
            _velocity /= length;
            _velocity *= _max_speed;
        }
        move(_velocity * dt);

        _hitbox.location(getPosition());

        auto unit_acc = _acceleration;
        auto len_acc = Utils::vector_length(world_acceleration);
        if (len_acc > 0.0f)
        {
            unit_acc /= len_acc;
        }

        _rot_velocity += _rot_acceleration * dt;
        if (_rot_velocity > _max_rot_speed)  { _rot_velocity = _max_rot_speed; }
        if (_rot_velocity < -_max_rot_speed) { _rot_velocity = -_max_rot_speed; }

        rotateBy(_rot_velocity * dt);

        for (auto i = 0u; i < _thrusters.size(); i++)
        {
            auto thruster = _thrusters[i].get();
            thruster->calc_thrust(unit_acc, _rot_acceleration);
        }
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

        for (auto i = 0u; i < _hardpoints.size(); i++)
        {
            target.draw(*_hardpoints[i].get(), states);
        }
        for (auto i = 0u; i < _thrusters.size(); i++)
        {
            auto thruster = _thrusters[i].get();
            if (thruster->thrust_amount() > 0.0f)
            {
                target.draw(*thruster, states);
            }
        }
    }

    Circle &Character::hitbox()
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
    
    const Character::HardpointList *Character::hardpoints() const
    {
        return &_hardpoints;
    }
    void Character::add_hardpoint(Hardpoint *point)
    {
        point->parent(this);
        _hardpoints.push_back(std::unique_ptr<Hardpoint>(point));
    }

    const Character::ThrusterList *Character::thrusters() const
    {
        return &_thrusters;
    }
    void Character::add_thruster(Thruster *thruster)
    {
        thruster->parent(this);
        _thrusters.push_back(std::unique_ptr<Thruster>(thruster));
    }

    void Character::acceleration(const sf::Vector2f &acc)
    {
        //_acceleration = Utils::transform_direction(getTransform(), acc);
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

    void Character::max_rot_speed(float speed)
    {
        _max_rot_speed = speed;
    }
    float Character::max_rot_speed() const
    {
        return _max_rot_speed;
    }
}
