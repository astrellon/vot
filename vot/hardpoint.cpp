#include "hardpoint.h"

namespace vot
{
    // Hardpoint {{{
    Hardpoint::Hardpoint(Group::Type group) :
        _cooldown(0.0f),
        _max_cooldown(0.5f),
        _target(nullptr),
        _group(group),
        _world_rotation(0.0f)
    {

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
        _blueprint(blueprint)
    {

    }

    void PatternBulletHardpoint::fire()
    {

    }
    // }}}
}
