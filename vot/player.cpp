#include "player.h"

#include "game_system.h"
#include "bullet.h"
#include "utils.h"
#include "texture_manager.h"

#include <iostream>

namespace vot
{
    Player::Player(const sf::Texture &texture) :
        Character(texture),
        _cooldown(0.0f),
        _homing_cooldown(0.0f),
        _look_at_target(false),
        _auto_target(true),
        _target(nullptr),
        _powerup_hitbox(20.0f)
    {

    }

    void Player::init()
    {
        add_hardpoint_placement(new HardpointPlacement(0, 8, 225, 315));
        add_hardpoint_placement(new HardpointPlacement(-16, 4, 180, 300));
        add_hardpoint_placement(new HardpointPlacement(16, 4, 240, 0));
        add_hardpoint_placement(new HardpointPlacement(20, 10, 270, 45));
        add_hardpoint_placement(new HardpointPlacement(-20, 10, 75, 270));

        create_new_hardpoint(Powerup::BULLET);
        
        add_thruster_placement(28, 0, 0, 0.35f);
        add_thruster_placement(28, 0, 180, 0.35f);
        add_thruster_placement(-28, 0, 180, 0.35f);
        add_thruster_placement(-28, 0, 0, 0.35f);

        add_thruster_placement(-8, 14, 180, 0.5f);
        add_thruster_placement(8, 14, 180, 0.5f);
        add_thruster_placement(8, -8, 0, 0.35f);
        add_thruster_placement(-8, -8, 0, 0.35f);
        
        add_thruster_placement(-28, 3, 270, 0.35f);
        add_thruster_placement(28, 3, 90, 0.35f);
    }
    
    void Player::update(float dt)
    {
        auto gs = GameSystem::main();

        // Keyboard input {{{
        sf::Vector2f acc;
        float rot_acc = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            acc.x -= strafe_speed();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            acc.x += strafe_speed();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            acc.y += backwards_speed();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            acc.y -= forwards_speed();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            rot_acc -= rot_speed();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            rot_acc += rot_speed();
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
            auto points = hardpoints();
            for (auto i = 0u; i < points->size(); i++)
            {
                points->at(i)->fire();
            }
        }

        if (_look_at_target && _target != nullptr)
        {
            auto time_to_stop = 0.0f;
            if (rot_acceleration() != 0.0f)
            {
                time_to_stop = rot_velocity() / rot_acceleration();
            }
            auto distance_to_stop = rot_velocity() * time_to_stop + 0.5f * rot_acceleration() * time_to_stop * time_to_stop;

            auto angles = Utils::calculate_angles(getPosition(), _target->getPosition(), rotation(), -90.0f);

            auto abs_delta = Utils::abs(angles.delta_angle());
            auto diff = abs_delta - Utils::abs(distance_to_stop);
            auto dir_speed = (angles.delta_angle() > 0) ? -rot_speed() : rot_speed();
            if (abs_delta > 0.1f && Utils::abs(rot_velocity()) < 0.1f)
            {
                if (diff > 0.1f)
                {
                    rot_acc -= dir_speed;
                }
                else if (diff < -0.1f)
                {
                    rot_acc += dir_speed;
                }
            }
            else if (diff < 0)
            {
                if (diff < -0.1f)
                {
                    rot_acc -= dir_speed;
                }
            }
            else if (diff > 0.1f)
            {
                rot_acc += dir_speed;
            }
        }

        acceleration(acc);
        rot_acceleration(rot_acc);

        _cooldown -= dt;
        _homing_cooldown -= dt;

        Character::update(dt);

        _powerup_hitbox.location(getPosition());
    }

    void Player::target(Enemy *value)
    {
        _target = value;

        auto points = hardpoints();
        for (auto i = 0u; i < points->size(); i++)
        {
            points->at(i)->target(value);
        }
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
        create_new_hardpoint(powerup.type());
    }
    void Player::create_new_hardpoint(Powerup::Type type)
    {
        HardpointPlacement *empty_placement = nullptr;
        for (auto i = 0u; i < _hardpoint_placements.size(); i++)
        {
            auto placement = _hardpoint_placements[i].get();
            if (placement->hardpoint() == nullptr)
            {
                empty_placement = placement;
                break;
            }
        }

        if (empty_placement == nullptr)
        {
            return;
        }

        if (type == Powerup::BULLET)
        {
            auto pattern_bullet = GameSystem::main()->bullet_manager().find_src_pattern_bullet("player_bullet_small");
            auto pattern_turret = new PatternBulletHardpoint(*pattern_bullet, Group::PLAYER);
            auto bullet_turret_texture = TextureManager::texture("bullet_turret");
            pattern_turret->texture(bullet_turret_texture);
            pattern_turret->target(_target);

            empty_placement->hardpoint(pattern_turret);
            add_hardpoint(pattern_turret);
        }
        if (type == Powerup::BEAM)
        {
            auto beam_blueprint = GameSystem::main()->beam_manager().find_src_beam("beam1");
            auto beam_turret = new BeamHardpoint(*beam_blueprint, Group::PLAYER);

            auto beam_turret_texture = TextureManager::texture("beam_turret");
            beam_turret->texture(beam_turret_texture);
            beam_turret->target(_target);
            
            empty_placement->hardpoint(beam_turret);
            add_hardpoint(beam_turret);
        }
        if (type == Powerup::HOMING)
        {
            auto homing_blueprint = GameSystem::main()->bullet_manager().find_src_homing_bullet("homing_blue");
            auto homing_turret = new HomingBulletHardpoint(*homing_blueprint, Group::PLAYER);

            auto bullet_turret_texture = TextureManager::texture("bullet_turret");
            homing_turret->texture(bullet_turret_texture);
            homing_turret->target(_target);

            empty_placement->hardpoint(homing_turret);
            add_hardpoint(homing_turret);
        }
    }

    void Player::add_hardpoint_placement(HardpointPlacement *placement)
    {
        _hardpoint_placements.push_back(std::unique_ptr<HardpointPlacement>(placement));
    }
    void Player::add_thruster_placement(float x, float y, float rotation, float size)
    {
        auto thruster = new Thruster();
        thruster->setPosition(x, y);
        thruster->setRotation(rotation);
        thruster->thrust_size(size);
        add_thruster(thruster);
    }
}
