#include "player.h"

#include "game_system.h"
#include "bullet.h"

#include <iostream>

namespace vot
{
    Player::Player(const sf::Texture &texture) :
        Character(texture),
        _cooldown(0.0f),
        _target(nullptr),
        _look_at_target(false)
    {

    }

    void Player::update(float dt)
    {
        auto gs = GameSystem::main();

        auto speed = 270.0f * dt;
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

        if (gs->is_key_pressed(sf::Keyboard::T))
        {
            target(gs->next_target(_target));
        }
        if (gs->is_key_pressed(sf::Keyboard::Z))
        {
            _look_at_target = !_look_at_target;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && _cooldown <= 0.0f)
        {
            auto bullet = spawn_bullet();
            bullet->pattern_type(2u);
            auto trans = forward_center_trans();
            bullet->init_transform(trans);
            
            bullet = spawn_bullet();
            bullet->pattern_type(2u);
            trans = forward_center_trans();
            trans.rotate(10.0f);
            bullet->init_transform(trans);

            bullet = spawn_bullet();
            bullet->pattern_type(2u);
            trans = forward_center_trans();
            trans.rotate(-10.0f);
            bullet->init_transform(trans);

            _cooldown = 0.1f;
        }

        if (_look_at_target && _target != nullptr)
        {
            auto target_center = _target->center();

            auto d_pos = center() - target_center;
            auto length = sqrt(d_pos.x * d_pos.x + d_pos.y * d_pos.y);
            d_pos.x /= length;
            d_pos.y /= length;

            auto to_angle = atan2(d_pos.y, d_pos.x) * 180.0f / M_PI;
            to_angle -= 90.0f;
            auto d_angle = rotation() - to_angle;
            while (d_angle >= 180.0f) d_angle -= 360.0f;
            while (d_angle <= -180.0f) d_angle += 360.0f;

            if (d_angle < rot_speed && d_angle > -rot_speed)
            {
                rotation(to_angle);
            }
            else
            {
                rotateBy(d_angle > 0 ? -rot_speed : rot_speed);
            }
            
        }
        _cooldown -= dt;
    }

    PatternBullet *Player::spawn_bullet()
    {
        return GameSystem::main()->bullet_manager().spawn_pattern_bullet("straight_blue", id(), Bullet::PLAYER);
    }
    
    void Player::target(Enemy *value)
    {
        _target = value;
    }
    Enemy *Player::target() const
    {
        return _target;
    }

    bool Player::looking_at_target() const
    {
        return _look_at_target;
    }
}
