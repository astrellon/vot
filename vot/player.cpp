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
        _homing_cooldown(0.0f),
        _target(nullptr),
        _powerup_hitbox(20.0f),
        _look_at_target(false),
        _auto_target(true)
    {

    }

    void Player::update(float dt)
    {
        auto gs = GameSystem::main();

        auto speed = 270.0f * dt;
        auto rot_speed = 180.0f * dt;
        // Keyboard input {{{
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
        // }}}

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (_cooldown <= 0.0f)
            {
                auto bullet_level = _powerups[Powerup::BULLET]; 

                if (bullet_level <= 5)
                {
                    if (bullet_level == 0)
                    {
                        auto bullet = spawn_pattern_bullet("player_bullet_small", 2u);
                        auto trans = forward_center_trans();
                        bullet->init_transform(trans);
                    }
                    else
                    {
                        spawn_pattern_bullet_pair("player_bullet_small", 2u, 4.0f, 0.0f);
                        if (bullet_level > 1)
                        {
                            spawn_pattern_bullet_pair("player_bullet_small", 2u, 30.0f, -20.0f);
                            if (bullet_level > 2)
                            {
                                spawn_pattern_bullet_pair("player_bullet_small", 2u, 34.0f, -20.0f);
                            }
                        }
                    }
                }

                _cooldown = 0.3f;

                if (bullet_level > 1)
                {
                    _cooldown = 0.2f;
                }
            }
            
            auto homing_level = _powerups[Powerup::HOMING]; 
            if (_homing_cooldown <= 0.0f && homing_level > 0)
            {
                auto homing_bullet = spawn_homing_bullet();
                auto angle = rotation() - 90.0f;
                homing_bullet->setup(location(), angle + 10.0f);
                homing_bullet->target(_target);

                homing_bullet = spawn_homing_bullet();
                homing_bullet->setup(location(), angle - 10.0f);
                homing_bullet->target(_target);

                if (homing_level > 1)
                {
                    homing_bullet = spawn_homing_bullet();
                    homing_bullet->setup(location(), angle + 30.0f);
                    homing_bullet->target(_target);

                    homing_bullet = spawn_homing_bullet();
                    homing_bullet->setup(location(), angle - 30.0f);
                    homing_bullet->target(_target);
                }

                _homing_cooldown = 0.75f;
            }
        }

        if (_look_at_target && _target != nullptr)
        {
            auto angles = Utils::calculate_angles(sprite().getPosition(), _target->sprite().getPosition(), rotation(), -90.0f);
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
        _homing_cooldown -= dt;

        _powerup_hitbox.location(sprite().getPosition());
    }

    PatternBullet *Player::spawn_pattern_bullet(const std::string &name, uint32_t pattern_type)
    {
        auto bullet = GameSystem::main()->bullet_manager().spawn_pattern_bullet(name, id(), Bullet::PLAYER); 
        bullet->pattern_type(pattern_type);
        return bullet;
    }
    void Player::spawn_pattern_bullet_pair(const std::string &name, uint32_t pattern_type, float x, float y)
    {
        auto bullet = spawn_pattern_bullet(name, pattern_type);
        auto trans = forward_center_trans();
        trans.translate(y, x);
        bullet->init_transform(trans);

        bullet = spawn_pattern_bullet(name, pattern_type);
        trans = forward_center_trans();
        trans.translate(y, -x);
        bullet->init_transform(trans);

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

    Circle &Player::powerup_hitbox()
    {
        return _powerup_hitbox;
    }

    void Player::add_powerup(const Powerup &powerup)
    {
        auto new_value = _powerups[powerup.type()] + powerup.value();
        if (new_value > 5)
        {
            new_value = 0;
        }
        if (new_value < 0)
        {
            new_value = 0;
        }

        _powerups[powerup.type()] = new_value;
    }
}
