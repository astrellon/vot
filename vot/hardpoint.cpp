#include "hardpoint.h"

#include "game_system.h"

namespace vot
{
    // Hardpoint {{{
    Hardpoint::Hardpoint(Group::Type group) :
        _cooldown(0.0f),
        _max_cooldown(0.5f),
        _parent(nullptr),
        _target(nullptr),
        _group(group),
        _world_rotation(0.0f)
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
    }
    void Hardpoint::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(_sprite, states);
    }

    void Hardpoint::world_position(sf::Vector2f value)
    {
        _world_position = value;
    }
    void Hardpoint::world_rotation(float value)
    {
        _world_rotation = value;
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
}
