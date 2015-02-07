#include "player.h"

namespace vot
{
    Player::Player(const sf::Texture &texture) :
        _sprite(texture),
        _hitbox(5.0f),
        _life(10.0f),
        _max_life(10.0f),
        _is_dead(false)
    {
        auto size = texture.getSize();
        _sprite.setOrigin(size.x * 0.5f, size.y * 0.5f);        
    }

    void Player::translate(const sf::Vector2f &vector)
    {
        _sprite.move(vector);
        _hitbox.location(_sprite.getPosition());
    }
    void Player::location(const sf::Vector2f &vector)
    {
        _sprite.setPosition(vector);
        _hitbox.location(vector);
    }
    sf::Vector2f Player::location() const
    {
        return _sprite.getPosition();
    }

    void Player::draw(sf::RenderTarget &target, sf::RenderStates state) const
    {
        target.draw(_sprite, state);
        //target.draw(_sprite, state);
        //sf::Sprite::draw(target, state);
        //sf::Drawable::draw(target, state);
    }

    Circle &Player::hitbox()
    {
        return _hitbox;
    }

    void Player::is_dead(bool value)
    {
        _is_dead = value;
    }
    bool Player::is_dead() const
    {
        return _is_dead;
    }

    void Player::take_damage(float damage)
    {
        _life -= damage;
        if (_life < 0.0f)
        {
            _life = 0.0f;
            _is_dead = true;
        }
    }

    void Player::life(float value)
    {
        _life = value;
    }
    float Player::life() const
    {
        return _life;
    }
    
    void Player::max_life(float value)
    {
        _max_life = value;
    }
    float Player::max_life() const
    {
        return _max_life;
    }
}
