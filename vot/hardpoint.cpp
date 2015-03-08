#include "hardpoint.h"

#include "game_system.h"
#include "texture_manager.h"

namespace vot
{
    // Hardpoint {{{
    Hardpoint::Hardpoint(Group::Type group) :
        _cooldown(0.0f),
        _max_cooldown(0.5f),
        _parent(nullptr),
        _target(nullptr),
        _group(group)
    {

    }

    void Hardpoint::parent(Character *value)
    {
        _parent = value;
    }
    Character *Hardpoint::parent() const
    {
        return _parent;
    }

    void Hardpoint::target(Character *value)
    {
        _target = value;
    }
    Character *Hardpoint::target() const
    {
        return _target;
    }

    void Hardpoint::texture(const sf::Texture *value)
    {
        _sprite.setTexture(*value);
        _sprite.setRotation(90.0f);
        auto size = value->getSize();
        _sprite.setOrigin(size.x * 0.5f, size.y * 0.5f);
    }
    const sf::Texture *Hardpoint::texture() const
    {
        return _sprite.getTexture();
    }

    sf::Sprite &Hardpoint::sprite()
    {
        return _sprite;
    }

    void Hardpoint::max_cooldown(float value)
    {
        _max_cooldown = value;
    }
    float Hardpoint::max_cooldown() const
    {
        return _max_cooldown;
    }

    void Hardpoint::cooldown(float value)
    {
        _cooldown = value;
    }
    void Hardpoint::change_cooldown(float delta)
    {
        _cooldown += delta;
    }
    float Hardpoint::cooldown() const
    {
        return _cooldown;
    }

    Group::Type Hardpoint::group() const
    {
        return _group;
    }

    void Hardpoint::update(float dt)
    {
        _cooldown -= dt;

        if (_target != nullptr)
        {
            auto rot_speed = 90.0f * dt;
            auto parent_trans = _parent->getInverseTransform();
            auto local_target = parent_trans * _target->getPosition();
            auto angles = Utils::calculate_angles(getPosition(), local_target, getRotation(), 180.0f);
            if (angles.delta_angle() < rot_speed && angles.delta_angle() > -rot_speed)
            {
                setRotation(angles.to_angle());
            }
            else
            {
                rotate(angles.delta_angle() > 0 ? -rot_speed : rot_speed);
            }
        }
    }
    void Hardpoint::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(_sprite, states);
    }
    // }}}

    // PatternBulletHardpoint {{{
    PatternBulletHardpoint::PatternBulletHardpoint(const PatternBullet &blueprint, Group::Type group) :
        Hardpoint(group),
        _blueprint(blueprint),
        _pattern_type(0u)
    {

    }

    void PatternBulletHardpoint::pattern_type(uint32_t type)
    {
        _pattern_type = type;
    }
    uint32_t PatternBulletHardpoint::pattern_type() const
    {
        return _pattern_type;
    }

    void PatternBulletHardpoint::fire()
    {
        if (cooldown() < 0.0f)
        {
            auto bullet = GameSystem::main()->bullet_manager().spawn_pattern_bullet(_blueprint, Group::PLAYER);
            bullet->pattern_type(_pattern_type);

            auto trans = parent()->getTransform() * getTransform();
            bullet->init_transform(trans);

            cooldown(max_cooldown());
        }
    }
    // }}}

    // HomingBulletHardpoint {{{
    HomingBulletHardpoint::HomingBulletHardpoint(const HomingBullet &blueprint, Group::Type group) :
        Hardpoint(group),
        _blueprint(blueprint)
    {

    }

    void HomingBulletHardpoint::fire()
    {
        if (cooldown() < 0.0f)
        {
            auto bullet = GameSystem::main()->bullet_manager().spawn_homing_bullet(_blueprint, Group::PLAYER);

            auto trans = parent()->getTransform() * getTransform();
            bullet->setup(trans.transformPoint(sf::Vector2f()), parent()->getRotation() + getRotation());

            cooldown(max_cooldown());
        }
    }
    // }}}

    // BeamHardpoint {{{
    BeamHardpoint::BeamHardpoint(const Beam &blueprint, Group::Type group) :
        Hardpoint(group),
        _blueprint(blueprint),
        _charge_up_system(nullptr),
        _charge_up(0.0f)
    {
        _active_beam = GameSystem::main()->beam_manager().spawn_beam(blueprint, group);

        auto texture = TextureManager::texture("bullet_blue_circle");
        _charge_up_system = GameSystem::main()->particle_manager().spawn_system(*texture, 2);
        _charge_up_system->system_type(1u);
        _charge_up_system->auto_remove(false);
    }

    void BeamHardpoint::update(float dt)
    {
        Hardpoint::update(dt);

        auto charge = _fire_beam ? dt : -dt;
        _charge_up += charge;
        if (_charge_up > 1.0f) _charge_up = 1.0f;
        if (_charge_up < 0.0f) _charge_up = 0.0f;
        
        _active_beam->is_active(_fire_beam && _charge_up >= 1.0f);
                
        auto trans = parent()->getTransform() * getTransform();
        auto global_position = trans.transformPoint(sf::Vector2f(8, 0));
        _charge_up_system->setPosition(global_position);


        if (_fire_beam)
        {
            
            if (_charge_up < 1.0f)
            {
                if (!_prev_charging_up)
                {
                    _charge_up_system->init();
                }
            }

            if (_charge_up >= 1.0f)
            {
                _charge_up = 1.0f;
                _active_beam->hitbox().origin(global_position);
                _active_beam->hitbox().rotation(getRotation() + parent()->getRotation());
            }
        }

        _prev_charging_up = _fire_beam;

        _fire_beam = false;
    }
    void BeamHardpoint::fire()
    {
        _fire_beam = true;
        if (_charge_up < 0.0f)
        {
            _charge_up = 0.0f;
        }
    }
    // }}}
}
