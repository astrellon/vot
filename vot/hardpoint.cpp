#include "hardpoint.h"

#include "game_system.h"
#include "texture_manager.h"
#include "utils/utils.h"

namespace vot
{
    // Hardpoint {{{
    Hardpoint::Hardpoint() :
        _cooldown(0.0f),
        _max_cooldown(0.2f),
        _parent(nullptr),
        _target(nullptr),
        _name("Hardpoint"),
        _max_angle(360.0f),
        _min_angle(0.0f),
        _track_ahead(false)
    {

    }
    Hardpoint::Hardpoint(const ::utils::Data *data) :
        _parent(nullptr),
        _target(nullptr)
    {
        deserialise(data);
    }
    Hardpoint::Hardpoint(const Hardpoint &clone) :
        _cooldown(clone._cooldown),
        _max_cooldown(clone._max_cooldown),
        _name(clone._name),
        _max_angle(clone._max_angle),
        _min_angle(clone._min_angle),
        _track_ahead(clone._track_ahead)
    {
        texture(clone._sprite.getTexture());
    }

    void Hardpoint::parent(HardpointPlacement *value)
    {
        _parent = value;
    }
    HardpointPlacement *Hardpoint::parent() const
    {
        return _parent;
    }

    Character *Hardpoint::parent_char() const
    {
        return _parent->parent();
    }

    void Hardpoint::target(Character *value)
    {
        _target = value;
    }
    Character *Hardpoint::target() const
    {
        return _target;
    }

    void Hardpoint::name(const std::string &value)
    {
        _name = value;
    }
    std::string Hardpoint::name() const
    {
        return _name;
    }

    void Hardpoint::texture(const sf::Texture *value)
    {
        _sprite.setTexture(*value);
        _sprite.setRotation(90.0f);
        _sprite.setScale(0.5f, 0.5f);
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

    void Hardpoint::track_ahead(bool value)
    {
        _track_ahead = value;
    }
    bool Hardpoint::track_ahead() const
    {
        return _track_ahead;
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
        if (_parent == nullptr)
        {
            return Group::NATURE;
        }
        return _parent->parent()->group();
    }

    float Hardpoint::projectile_speed() const
    {
        // Real fast!
        return 1000000.0f;
    }

    void Hardpoint::update(float dt)
    {
        _cooldown -= dt;

        if (_target != nullptr)
        {
            auto rot_speed = 90.0f * dt;
            auto parent_trans = _parent->parent()->getInverseTransform();
            auto target_position = _target->getPosition();
            if (track_ahead())
            {
                auto distance = utils::Utils::vector_length(_parent->parent()->getPosition() - _target->getPosition());
                auto projectile_time = distance / projectile_speed();
                target_position = _target->getPosition() + _target->velocity() * projectile_time * 4.0f + 0.5f * _target->acceleration() * projectile_time * projectile_time;
            }

            auto local_target = parent_trans * target_position;
            auto angles = utils::Utils::calculate_angles(getPosition(), local_target, getRotation(), 180.0f);
            if (angles.delta_angle() < rot_speed && angles.delta_angle() > -rot_speed)
            {
                setRotation(angles.to_angle());
            }
            else
            {
                rotate(angles.delta_angle() > 0 ? -rot_speed : rot_speed);
            }
            
            //auto angle = getRotation();
            /*
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
                auto dmin = utils::Utils::abs(_min_angle - angle);
                auto dmax = utils::Utils::abs(_max_angle - angle);
                setRotation(dmin < dmax ? _min_angle : _max_angle);
            }
            */
        }
    }
    void Hardpoint::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(_sprite, states);
    }

    void Hardpoint::serialise(::utils::Data *data) const
    {
        if (cooldown() > 0.0f)
        {
            data->at("cooldown", cooldown());
        }
        data->at("max_cooldown", max_cooldown());
        data->at("max_angle", max_angle());
        data->at("min_angle", min_angle());
        data->at("track_ahead", track_ahead());
        data->at("name", name());
        data->at("texture", TextureManager::texture_name(_sprite.getTexture()));
    }
    void Hardpoint::deserialise(const ::utils::Data *data)
    {
        _cooldown = data->at("cooldown")->number();
        _max_cooldown = data->at("max_cooldown")->number();

        _max_angle = data->at("max_angle")->number();
        _min_angle = data->at("min_angle")->number();
        _track_ahead = data->at("track_ahead")->boolean();

        _name = data->at("name")->string();

        auto texture = data->at("texture")->string();
        _sprite.setTexture(*TextureManager::texture(texture));
    }

    Hardpoint *Hardpoint::create_from_data(const ::utils::Data *data)
    {
        auto type = data->at("hardpoint_type")->string();
        if (type == "pattern")
        {
            return new PatternBulletHardpoint(data);
        }
        else if (type == "homing")
        {
            return new HomingBulletHardpoint(data);
        }
        else if (type == "beam")
        {
            return new BeamHardpoint(data);
        }

        return nullptr;
    }
    // }}}

    // PatternBulletHardpoint {{{
    PatternBulletHardpoint::PatternBulletHardpoint() :
        Hardpoint(),
        _blueprint(nullptr),
        _pattern_type(0u),
        _fire_bullet(false)
    {
        track_ahead(true);
    }
    PatternBulletHardpoint::PatternBulletHardpoint(const PatternBullet *blueprint) :
        Hardpoint(),
        _blueprint(blueprint),
        _pattern_type(0u),
        _fire_bullet(false)
    {
        track_ahead(true);
    }
    PatternBulletHardpoint::PatternBulletHardpoint(const ::utils::Data *data) :
        Hardpoint(data)
    {
        deserialise(data);
    }
    PatternBulletHardpoint::PatternBulletHardpoint(const PatternBulletHardpoint &clone) :
        Hardpoint(clone),
        _blueprint(clone._blueprint),
        _pattern_type(clone._pattern_type)
    {
        
    }

    Hardpoint *PatternBulletHardpoint::clone() const
    {
        return new PatternBulletHardpoint(*this);
    }

    void PatternBulletHardpoint::pattern_type(uint32_t type)
    {
        _pattern_type = type;
    }
    uint32_t PatternBulletHardpoint::pattern_type() const
    {
        return _pattern_type;
    }

    float PatternBulletHardpoint::projectile_speed() const
    {
        return _blueprint->speed();
    }

    void PatternBulletHardpoint::blueprint(PatternBullet *value)
    {
        _blueprint = value;
    }
    const PatternBullet *PatternBulletHardpoint::blueprint() const
    {
        return _blueprint;
    }

    void PatternBulletHardpoint::update(float dt)
    {
        Hardpoint::update(dt);

        if (_fire_bullet && cooldown() < 0.0f)
        {
            auto texture = TextureManager::texture("bullet_blue_circle");
            auto system = GameSystem::particle_manager()->spawn_system(*texture, 3);
            system->speed_factor(1.75f);

            auto trans = parent_char()->getTransform() * getTransform();
            auto global_position = trans.transformPoint(sf::Vector2f(8, 0));
            auto global_rotation = getRotation() + parent_char()->getRotation();
            system->setPosition(global_position);
            system->setRotation(global_rotation);
            system->init();

            auto bullet = GameSystem::bullet_manager()->spawn_pattern_bullet(*_blueprint, group());
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

    void PatternBulletHardpoint::serialise(::utils::Data *data) const
    {
        Hardpoint::serialise(data);

        data->at("hardpoint_type", "pattern");
        data->at("pattern_type", pattern_type());
        if (blueprint() != nullptr)
        {
            data->at("blueprint", GameSystem::bullet_manager()->find_src_pattern_bullet(blueprint()));
        }
    }
    void PatternBulletHardpoint::deserialise(const ::utils::Data *data)
    {
        pattern_type(data->at("pattern_type")->uint32());
        if (data->has("blueprint"))
        {
            auto blueprint_name = data->at("blueprint")->string();
            _blueprint = GameSystem::bullet_manager()->find_src_pattern_bullet(blueprint_name);
        }
    }
    // }}}

    // HomingBulletHardpoint {{{
    HomingBulletHardpoint::HomingBulletHardpoint() :
        Hardpoint(),
        _blueprint(nullptr)
    {

    }
    HomingBulletHardpoint::HomingBulletHardpoint(const HomingBullet *blueprint) :
        Hardpoint(),
        _blueprint(blueprint)
    {

    }
    HomingBulletHardpoint::HomingBulletHardpoint(const ::utils::Data *data) :
        Hardpoint(data)
    {
        deserialise(data);
    }
    HomingBulletHardpoint::HomingBulletHardpoint(const HomingBulletHardpoint &clone) :
        Hardpoint(clone),
        _blueprint(clone._blueprint)
    {

    }

    Hardpoint *HomingBulletHardpoint::clone() const
    {
        return new HomingBulletHardpoint(*this);
    }

    void HomingBulletHardpoint::blueprint(HomingBullet *value)
    {
        _blueprint = value;
    }
    const HomingBullet *HomingBulletHardpoint::blueprint() const
    {
        return _blueprint;
    }

    void HomingBulletHardpoint::fire()
    {
        if (cooldown() < 0.0f)
        {
            auto bullet = GameSystem::bullet_manager()->spawn_homing_bullet(*_blueprint, group());

            auto trans = parent_char()->getTransform() * getTransform();
            bullet->setup(trans.transformPoint(sf::Vector2f()), parent_char()->getRotation() + getRotation());
            
            auto sound = GameSystem::sound_manager()->spawn_sound("laser2", Sound::SoundEffects);
            sound->play();

            cooldown(max_cooldown());
        }
    }

    void HomingBulletHardpoint::serialise(::utils::Data *data) const
    {
        Hardpoint::serialise(data);

        data->at("hardpoint_type", "homing");
        if (blueprint() != nullptr)
        {
            data->at("blueprint", GameSystem::bullet_manager()->find_src_homing_bullet(blueprint()));
        }
    }
    void HomingBulletHardpoint::deserialise(const ::utils::Data *data)
    {
        if (data->has("blueprint"))
        {
            _blueprint = GameSystem::bullet_manager()->find_src_homing_bullet(data->at("blueprint")->string());
        }
    }
    // }}}

    // BeamHardpoint {{{
    BeamHardpoint::BeamHardpoint() :
        Hardpoint(),
        _blueprint(nullptr),
        _charge_up_system(nullptr),
        _charge_up(0.0f)
    {
        init();
    }
    BeamHardpoint::BeamHardpoint(const Beam *blueprint) :
        Hardpoint(),
        _blueprint(blueprint),
        _charge_up_system(nullptr),
        _charge_up(0.0f)
    {
        init();
    }
    BeamHardpoint::BeamHardpoint(const ::utils::Data *data) :
        Hardpoint(data)
    {
        deserialise(data);
        init();
    }
    BeamHardpoint::BeamHardpoint(const BeamHardpoint &clone) :
        Hardpoint(clone),
        _blueprint(clone._blueprint)
    {
        init();
    }

    Hardpoint *BeamHardpoint::clone() const
    {
        return new BeamHardpoint(*this);
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

    void BeamHardpoint::blueprint(Beam *value)
    {
        _blueprint = value;
    }
    const Beam *BeamHardpoint::blueprint() const
    {
        return _blueprint;
    }

    void BeamHardpoint::update(float dt)
    {
        Hardpoint::update(dt);

        auto charge = _fire_beam ? dt : -dt;
        _charge_up += charge;
        if (_charge_up > 1.0f) _charge_up = 1.0f;
        if (_charge_up < 0.0f) _charge_up = 0.0f;
        
        _active_beam->is_active(_fire_beam && _charge_up >= 1.0f);

        auto trans = parent_char()->getTransform() * getTransform();
        auto global_position = trans.transformPoint(sf::Vector2f(8, 0));
        auto global_rotation = getRotation() + parent_char()->getRotation();
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

    void BeamHardpoint::serialise(::utils::Data *data) const
    {
        Hardpoint::serialise(data);
            
        data->at("hardpoint_type", "beam");
        if (blueprint() != nullptr)
        {
            data->at("blueprint", GameSystem::beam_manager()->find_src_beam(blueprint()));
        }
    }
    void BeamHardpoint::deserialise(const ::utils::Data *data)
    {
        if (data->has("blueprint"))
        {
            _blueprint = GameSystem::beam_manager()->find_src_beam(data->at("blueprint")->string());
        }
    }

    void BeamHardpoint::init()
    {
        _active_beam = GameSystem::beam_manager()->spawn_beam(*_blueprint, group());

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
    // }}}
    
    // HardpointManager {{{
    HardpointManager::PatternMap HardpointManager::s_src_pattern_hardpoints;
    HardpointManager::HomingMap HardpointManager::s_src_homing_hardpoints;
    HardpointManager::BeamMap HardpointManager::s_src_beam_hardpoints;

    PatternBulletHardpoint *HardpointManager::spawn_pattern_hardpoint(const std::string &name, const std::string &blueprint)
    {
        auto find = s_src_pattern_hardpoints.find(name);
        if (find == s_src_pattern_hardpoints.end())
        {
            return nullptr;
        }

        auto new_point = new PatternBulletHardpoint(*find->second.get());
        new_point->blueprint(GameSystem::bullet_manager()->find_src_pattern_bullet(blueprint));
        return new_point;
    }
    HomingBulletHardpoint *HardpointManager::spawn_homing_hardpoint(const std::string &name, const std::string &blueprint)
    {
        auto find = s_src_homing_hardpoints.find(name);
        if (find == s_src_homing_hardpoints.end())
        {
            return nullptr;
        }

        auto new_point = new HomingBulletHardpoint(*find->second.get());
        new_point->blueprint(GameSystem::bullet_manager()->find_src_homing_bullet(blueprint));
        return new_point;
    }
    BeamHardpoint *HardpointManager::spawn_beam_hardpoint(const std::string &name, const std::string &blueprint)
    {
        auto find = s_src_beam_hardpoints.find(name);
        if (find == s_src_beam_hardpoints.end())
        {
            return nullptr;
        }

        auto new_point = new BeamHardpoint(*find->second.get());
        new_point->blueprint(GameSystem::beam_manager()->find_src_beam(blueprint));
        return new_point;
    }

    void HardpointManager::add_src_pattern_hardpoint(const std::string &name, PatternBulletHardpoint *point)
    {
        s_src_pattern_hardpoints[name] = std::unique_ptr<PatternBulletHardpoint>(point);
    }
    void HardpointManager::add_src_homing_hardpoint(const std::string &name, HomingBulletHardpoint *point)
    {
        s_src_homing_hardpoints[name] = std::unique_ptr<HomingBulletHardpoint>(point);
    }
    void HardpointManager::add_src_beam_hardpoint(const std::string &name, BeamHardpoint *point)
    {
        s_src_beam_hardpoints[name] = std::unique_ptr<BeamHardpoint>(point);
    }
    // }}}
}
