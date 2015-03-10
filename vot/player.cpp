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
        auto gs = GameSystem::main();

        auto beam_turret_texture = TextureManager::texture("beam_turret");
        auto bullet_turret_texture = TextureManager::texture("bullet_turret");

        auto homing_bullet = gs->bullet_manager().find_src_homing_bullet("homing_blue");
        auto pattern_bullet = gs->bullet_manager().find_src_pattern_bullet("player_bullet_small");

        /*
        auto pattern_turret = new PatternBulletHardpoint(*pattern_bullet, Group::PLAYER);
        pattern_turret->setup(-16, 4, -90.0f, 180.0f, 300.0f);
        pattern_turret->texture(bullet_turret_texture);
        add_hardpoint(pattern_turret);
        
        pattern_turret = new PatternBulletHardpoint(*pattern_bullet, Group::PLAYER);
        pattern_turret->setup(16, 4, -90.0f, 240.0f, 0.0f);
        pattern_turret->texture(bullet_turret_texture);
        add_hardpoint(pattern_turret);
        
        pattern_turret = new PatternBulletHardpoint(*pattern_bullet, Group::PLAYER);
        pattern_turret->setup(20, 10, -90.0f, 270.0f, 45.0f);
        pattern_turret->texture(bullet_turret_texture);
        add_hardpoint(pattern_turret);

        pattern_turret = new PatternBulletHardpoint(*pattern_bullet, Group::PLAYER);
        pattern_turret->setup(-20, 10, -90.0f, 75.0f, 270.0f);
        pattern_turret->texture(bullet_turret_texture);
        add_hardpoint(pattern_turret);
        */

        add_hardpoint_placement(new HardpointPlacement(-16, 4, 180, 300));
        add_hardpoint_placement(new HardpointPlacement(16, 4, 240, 0));
        add_hardpoint_placement(new HardpointPlacement(20, 10, 270, 45));
        add_hardpoint_placement(new HardpointPlacement(-20, 10, 75, 270));
        /*
        auto beam_blueprint = gs->beam_manager().find_src_beam("beam1");
        auto beam_turret = new BeamHardpoint(*beam_blueprint, Group::PLAYER);
        beam_turret->setPosition(18, -10);
        beam_turret->setRotation(-90.0f);

        beam_turret->texture(beam_turret_texture);
        add_hardpoint(beam_turret);
        
        beam_turret = new BeamHardpoint(*beam_blueprint, Group::PLAYER);
        beam_turret->setPosition(-18, -10);
        beam_turret->setRotation(-90.0f);

        beam_turret->texture(beam_turret_texture);
        add_hardpoint(beam_turret);
        */
    }

    void Player::translate(const sf::Vector2f &vector)
    {
        Character::translate(vector);

        _powerup_hitbox.location(getPosition());
    }
    void Player::location(const sf::Vector2f &vector)
    {
        Character::location(vector);

        _powerup_hitbox.location(getPosition());
    }
    sf::Vector2f Player::location() const
    {
        return Character::location();
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
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            auto points = hardpoints();
            for (auto i = 0u; i < points->size(); i++)
            {
                points->at(i)->fire();
            }
        }

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

        if (_look_at_target && _target != nullptr)
        {
            auto angles = Utils::calculate_angles(getPosition(), _target->getPosition(), rotation(), -90.0f);
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

        Character::update(dt);
    }

    PatternBullet *Player::spawn_pattern_bullet(const std::string &name, uint32_t pattern_type)
    {
        auto bullet = GameSystem::main()->bullet_manager().spawn_pattern_bullet(name, Group::PLAYER); 
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
        return GameSystem::main()->bullet_manager().spawn_homing_bullet("homing_blue", Group::PLAYER);
    }
    void Player::spawn_homing_bullet_pair(float offset_angle)
    {
        auto homing_bullet = spawn_homing_bullet();
        auto angle = rotation() - 90.0f;
        homing_bullet->setup(location(), angle + offset_angle);
        homing_bullet->target(_target);

        homing_bullet = spawn_homing_bullet();
        homing_bullet->setup(location(), angle - offset_angle);
        homing_bullet->target(_target);
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
        /*
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
        */

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

        if (powerup.type() == Powerup::BULLET)
        {
            auto pattern_bullet = GameSystem::main()->bullet_manager().find_src_pattern_bullet("player_bullet_small");
            auto pattern_turret = new PatternBulletHardpoint(*pattern_bullet, Group::PLAYER);
            auto bullet_turret_texture = TextureManager::texture("bullet_turret");
            pattern_turret->texture(bullet_turret_texture);

            empty_placement->hardpoint(pattern_turret);
            add_hardpoint(pattern_turret);
        }
    }

    void Player::add_hardpoint_placement(HardpointPlacement *placement)
    {
        _hardpoint_placements.push_back(std::unique_ptr<HardpointPlacement>(placement));
    }
}
