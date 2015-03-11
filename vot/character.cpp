#include "character.h"

namespace vot
{
    Character::Character(const sf::Texture &texture) :
        _sprite(texture),
        _hitbox(5.0f),
        _life(10.0f),
        _max_life(10.0f),
        _is_dead(false),
        _id(0u)
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
        _id(0u)
    {

    }

    void Character::translate(const sf::Vector2f &vector)
    {
        // Transform move vector by the sprites rotation matrix.
        auto matrix = getTransform().getMatrix();
        auto x = vector.x * matrix[0] - vector.y * matrix[1];
        auto y = -vector.x * matrix[4] + vector.y * matrix[5];
        move(x, y);
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

        _hitbox.location(getPosition());
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
}
