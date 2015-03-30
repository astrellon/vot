#include "hardpoint.h"

#include "game_system.h"
#include "texture_manager.h"
#include "utils.h"

namespace vot
{
    // Hardpoint {{{
    Hardpoint::Hardpoint(Group::Type group) :
        _cooldown(0.0f),
        _max_cooldown(0.5f),
        _parent(nullptr),
        _target(nullptr),
        _group(group),
        _max_angle(360.0f),
        _min_angle(0.0f)
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
    
    void Hardpoint::max_angle(float value)
    {
        _max_angle = value;
    }
    float Hardpoint::max_angle() const
    {
        return _max_angle;
    }
    
    void Hardpoint::min_angle(float value)
    {
        _min_angle = value;
    }
    float Hardpoint::min_angle() const
    {
        return _min_angle;
    }

    void Hardpoint::setup(float x, float y, float rotation, float min, float max)
    {
        setPosition(x, y);
        setRotation(rotation);
        min_angle(min);
        max_angle(max);
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
            
            auto angle = getRotation();
            if (_max_angle > _min_angle)
            {
                if (angle > _max_angle)
                {
                    setRotation(_max_angle);
                }
                if (angle < _min_angle)
                {
                    setRotation(_min_angle);
                }
            }
            else if (angle < _min_angle && angle > _max_angle)
            {
                auto dmin = Utils::abs(_min_angle - angle);
                auto dmax = Utils::abs(_max_angle - angle);
                setRotation(dmin < dmax ? _min_angle : _max_angle);
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
        _pattern_type(0u),
        _fire_bullet(false)
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

    void PatternBulletHardpoint::update(float dt)
    {
        Hardpoint::update(dt);

        if (_fire_bullet && cooldown() < 0.0f)
        {
            auto texture = TextureManager::texture("bullet_blue_circle");
            auto system = GameSystem::particle_manager()->spawn_system(*texture, 3);
            system->speed_factor(1.75f);

            auto trans = parent()->getTransform() * getTransform();
            auto global_position = trans.transformPoint(sf::Vector2f(8, 0));
            auto global_rotation = getRotation() + parent()->getRotation();
            system->setPosition(global_position);
            system->setRotation(global_rotation);
            system->init();

            auto bullet = GameSystem::bullet_manager()->spawn_pattern_bullet(_blueprint, Group::PLAYER);
            bullet->pattern_type(_pattern_type);

            bullet->init_transform(trans);

            auto sound = GameSystem::sound_manager()->spawn_sound("laser3", Sound::SoundEffects);
            sound->play();

            cooldown(max_cooldown());
        }

        _fire_bullet = false;
    }
    void PatternBulletHardpoint::fire()
    {
        _fire_bullet = true;
    }
    void PatternBulletHardpoint::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        Hardpoint::draw(target, states);
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
            auto bullet = GameSystem::bullet_manager()->spawn_homing_bullet(_blueprint, Group::PLAYER);

            auto trans = parent()->getTransform() * getTransform();
            bullet->setup(trans.transformPoint(sf::Vector2f()), parent()->getRotation() + getRotation());
            
            auto sound = GameSystem::sound_manager()->spawn_sound("laser2", Sound::SoundEffects);
            sound->play();

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
        _active_beam = GameSystem::beam_manager()->spawn_beam(blueprint, group);

        auto texture = TextureManager::texture("bullet_blue_circle");
        _charge_up_system = GameSystem::particle_manager()->spawn_system(*texture, 10);
        _charge_up_system->system_type(1u);
        _charge_up_system->auto_remove(false);

        texture = TextureManager::texture("beam_glow");
        auto size = texture->getSize();
        _beam_glow.setTexture(*texture);
        _beam_glow.setOrigin(size.x * 0.5f, size.y * 0.5f);
        _beam_glow.setPosition(8, 0);
    }

    void BeamHardpoint::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        Hardpoint::draw(target, states);

        if (_charge_up > 0.0f)
        {
            states.blendMode = sf::BlendAdd;
            states.transform *= getTransform();
            target.draw(_beam_glow, states);
        }
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
        auto global_rotation = getRotation() + parent()->getRotation();
        _charge_up_system->setPosition(global_position);
        _charge_up_system->setRotation(global_rotation);
        
        _beam_glow.setScale(_charge_up, _charge_up);

        if (_fire_beam)
        {
            
            if (_charge_up < 1.0f)
            {
                if (!_prev_charging_up)
                {
                    _charge_up_system->init();
                    auto sound = GameSystem::sound_manager()->spawn_sound("chargeup", Sound::SoundEffects);
                    sound->play();
                }
            }

            if (_charge_up >= 1.0f)
            {
                _charge_up = 1.0f;
                _active_beam->hitbox().origin(global_position);
                _active_beam->hitbox().rotation(global_rotation);
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
    
    // HardpointPlacement {{{
    HardpointPlacement::HardpointPlacement() :
        _hardpoint(nullptr),
        _min_angle(0.0f),
        _max_angle(360.0f)
    {

    }
    HardpointPlacement::HardpointPlacement(float x, float y, float min, float max) :
        _hardpoint(nullptr),
        _position(x, y),
        _min_angle(min),
        _max_angle(max)
    {

    }

    void HardpointPlacement::setup(float x, float y, float min, float max)
    {
        position(sf::Vector2f(x, y));
        min_angle(min);
        max_angle(max);
    }
    void HardpointPlacement::hardpoint(Hardpoint *hardpoint)
    {
        _hardpoint = hardpoint;
        if (hardpoint != nullptr)
        {
            auto average = (_min_angle + _max_angle) * 0.5f;
            hardpoint->setup(_position.x, _position.y, average, _min_angle, _max_angle);
        }
    }
    Hardpoint *HardpointPlacement::hardpoint() const
    {
        return _hardpoint;
    }

    void HardpointPlacement::position(const sf::Vector2f &position)
    {
        _position = position;
    }
    sf::Vector2f HardpointPlacement::position() const
    {
        return _position;
    }
    
    void HardpointPlacement::max_angle(float value)
    {
        _max_angle = value;
    }
    float HardpointPlacement::max_angle() const
    {
        return _max_angle;
    }
    
    void HardpointPlacement::min_angle(float value)
    {
        _min_angle = value;
    }
    float HardpointPlacement::min_angle() const
    {
        return _min_angle;
    }
    // }}}
}
