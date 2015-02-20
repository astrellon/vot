#include "player.h"

#include "game_system.h"
#include "bullet.h"
#include "utils.h"

#include <iostream>

namespace vot
{
    Player::Player(const sf::Texture &texture) :
        Character(texture),
        _cooldown(0.0f),
        _target(nullptr),
        _look_at_target(false),
        _auto_target(true)
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                _auto_target = !_auto_target;
            }
            else
            {
                target(gs->next_target(_target));
            }
        }
        if (gs->is_key_pressed(sf::Keyboard::Z))
        {
            _look_at_target = !_look_at_target;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && _cooldown <= 0.0f)
        {
            auto bullet = spawn_pattern_bullet();
            bullet->pattern_type(2u);
            auto trans = forward_center_trans();
            bullet->init_transform(trans);
            
            auto homing_bullet = spawn_homing_bullet();
            auto angle = rotation() - 90.0f;
            homing_bullet->setPosition(location());
            homing_bullet->rotate(angle + 10.0f);
            homing_bullet->target(_target);

            homing_bullet = spawn_homing_bullet();
            homing_bullet->setPosition(location());
            homing_bullet->rotate(angle - 10.0f);
            homing_bullet->target(_target);

            homing_bullet = spawn_homing_bullet();
            homing_bullet->setPosition(location());
            homing_bullet->rotate(angle - 30.0f);
            homing_bullet->target(_target);

            homing_bullet = spawn_homing_bullet();
            homing_bullet->setPosition(location());
            homing_bullet->rotate(angle + 30.0f);
            homing_bullet->target(_target);

            _cooldown = 0.1f;
        }

        if (_look_at_target && _target != nullptr)
        {
            auto angles = Utils::calculate_angles(center(), _target->center(), rotation(), -90.0f);
            if (angles.delta_angle() < rot_speed && angles.delta_angle() > -rot_speed)
            {
                rotation(angles.to_angle());
            }
            else
            {
                rotateBy(angles.delta_angle() > 0 ? -rot_speed : rot_speed);
            }
            
        }
        _cooldown -= dt;
    }

    PatternBullet *Player::spawn_pattern_bullet()
    {
        return GameSystem::main()->bullet_manager().spawn_pattern_bullet("straight_blue", id(), Bullet::PLAYER);
    }
    HomingBullet *Player::spawn_homing_bullet()
    {
        return GameSystem::main()->bullet_manager().spawn_homing_bullet("homing_blue", id(), Bullet::PLAYER);
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
    bool Player::auto_target() const
    {
        return _auto_target;
    }
}
