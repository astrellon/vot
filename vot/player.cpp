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
        
        acceleration(acc);
        rot_acceleration(rot_acc);

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

        /* {{{
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (_cooldown <= 0.0f)
            {
                _cooldown = 0.3f;
                auto bullet_level = _powerups[Powerup::BULLET]; 

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
                        _cooldown = 0.25f;
                    }
                    if (bullet_level > 2)
                    {
                        spawn_pattern_bullet_pair("player_bullet_small", 2u, 34.0f, -20.0f);
                        _cooldown = 0.20f;
                    }
                    if (bullet_level > 3)
                    {
                        _cooldown = 0.15f;
                    }
                }
            }
            
            auto homing_level = _powerups[Powerup::HOMING]; 
            if (_homing_cooldown <= 0.0f && homing_level > 0)
            {
                _homing_cooldown = 0.75f;
                if (homing_level == 1)
                {
                    auto homing_bullet = spawn_homing_bullet();
                    auto angle = rotation() - 90.0f;
                    homing_bullet->setup(location(), angle);
                    homing_bullet->target(_target);
                }
                else
                {
                    spawn_homing_bullet_pair(80.0f);

                    if (homing_level == 3 || homing_level > 4)
                    {
                        auto homing_bullet = spawn_homing_bullet();
                        auto angle = rotation() - 90.0f;
                        homing_bullet->setup(location(), angle);
                        homing_bullet->target(_target);
                    }
                    if (homing_level > 3)
                    {
                        spawn_homing_bullet_pair(100.0f);
                        _homing_cooldown = 0.7f;
                    }
                    if (homing_level > 5)
                    {
                        _homing_cooldown = 0.65f;
                    }
                }
            }
        }
        }}}*/

        if (_look_at_target && _target != nullptr)
        {
            auto rot_spd = rot_speed();
            auto angles = Utils::calculate_angles(getPosition(), _target->getPosition(), rotation(), -90.0f);
            rot_spd *= dt;
            if (angles.delta_angle() < rot_spd && angles.delta_angle() > -rot_spd)
            {
                rotation(angles.to_angle());
            }
            else
            {
                rotateBy(angles.delta_angle() > 0 ? -rot_spd : rot_spd);
            }
        }

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
