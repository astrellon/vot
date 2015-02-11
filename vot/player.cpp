#include "player.h"

#include "game_system.h"
#include "bullet.h"

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
        // Transform move vector by the sprites rotation matrix.
        auto matrix = _sprite.getTransform().getMatrix();
        auto x = vector.x * matrix[0] - vector.y * matrix[1];
        auto y = -vector.x * matrix[4] + vector.y * matrix[5];
        _sprite.move(x, y);
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

    void Player::rotateBy(float angle)
    {
        _sprite.rotate(angle);
    }
    void Player::rotation(float angle)
    {
        _sprite.setRotation(angle);
    }
    float Player::rotation() const
    {
        return _sprite.getRotation();
    }

    void Player::update(float dt)
    {
        auto speed = 450.0f * dt;
        auto rot_speed = 180.0f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            translate(sf::Vector2f(-speed, 0.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            translate(sf::Vector2f(speed, 0.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            translate(sf::Vector2f(0.0f, speed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            translate(sf::Vector2f(0.0f, -speed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            rotateBy(-rot_speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            rotateBy(rot_speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            auto gs = GameSystem::main();
            auto bullet = gs->bullet_manager().clone_pattern_bullet("bullet_blue_circle");
            auto trans = _sprite.getTransform();
            trans.rotate(-90);
            auto size = _sprite.getTexture()->getSize();
            trans.translate(size.x * -0.5f, size.y * 0.5f);
            bullet->init_transform(trans);
        }
    }
    void Player::draw(sf::RenderTarget &target, sf::RenderStates state) const
    {
        target.draw(_sprite, state);
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
