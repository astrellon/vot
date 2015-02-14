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
        _target(nullptr)
    {
        auto size = texture.getSize();
        _sprite.setOrigin(size.x * 0.5f, size.y * 0.5f);        
    }

    void Character::translate(const sf::Vector2f &vector)
    {
        // Transform move vector by the sprites rotation matrix.
        auto matrix = _sprite.getTransform().getMatrix();
        auto x = vector.x * matrix[0] - vector.y * matrix[1];
        auto y = -vector.x * matrix[4] + vector.y * matrix[5];
        _sprite.move(x, y);
        _hitbox.location(_sprite.getPosition());
    }
    void Character::location(const sf::Vector2f &vector)
    {
        _sprite.setPosition(vector);
        _hitbox.location(vector);
    }
    sf::Vector2f Character::location() const
    {
        return _sprite.getPosition();
    }

    void Character::rotateBy(float angle)
    {
        _sprite.rotate(angle);
    }
    void Character::rotation(float angle)
    {
        _sprite.setRotation(angle);
    }
    float Character::rotation() const
    {
        return _sprite.getRotation();
    }

    void Character::draw(sf::RenderTarget &target, sf::RenderStates state) const
    {
        target.draw(_sprite, state);
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
        if (_life < 0.0f)
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
    
    sf::Transform Character::forward_center_trans() const
    {
        auto trans = sprite().getTransform();
        trans.rotate(-90);
        auto size = sprite().getTexture()->getSize();
        trans.translate(size.x * -0.5f, size.y * 0.5f);
        return trans;
    }

    void Character::target(Character *value)
    {
        _target = value;
    }
    Character *Character::target() const
    {
        return _target;
    }
}
