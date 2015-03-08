#include "beam.h"

#include "utils.h"
#include "texture_manager.h"

namespace vot
{
    // Beam {{{
    Beam::Beam() :
        _index(Utils::max_uint),
        _group(Group::NATURE),
        _max_length(100.0f),
        _width(10.0f),
        _hitting_target_length(-1.0f),
        _dps(2.0f),
        _is_active(false),
        _hit_particle_cooldown(0.0f)
    {
        _shape.setOrigin(0.0f, _width * 0.5f);
        _shape_top.setOrigin(0.0f, _width * 0.5f);

        auto texture = TextureManager::texture("beam");
        _shape.setTexture(texture, true);

        texture = TextureManager::texture("beam_top");
        _shape_top.setTexture(texture, true);
    }

    Ray &Beam::hitbox()
    {
        return _hitbox;
    }
    const Ray &Beam::hitbox() const
    {
        return _hitbox;
    }

    void Beam::max_length(float value)
    {
        _max_length = value;
    }
    float Beam::max_length() const
    {
        return _max_length;
    }

    void Beam::width(float value)
    {
        _width = value;
        _shape.setOrigin(0.0f, _width * 0.5f);
        _shape_top.setOrigin(0.0f, _width * 0.5f);

        auto size = _shape_top.getTexture()->getSize();
        _shape_top.setSize(sf::Vector2f(size.x, _width));
    }
    float Beam::width() const
    {
        return _width;
    }

    void Beam::index(uint32_t value)
    {
        _index = value;
    }
    uint32_t Beam::index() const
    {
        return _index;
    }

    void Beam::group(Group::Type value)
    {
        _group = value;
    }
    Group::Type Beam::group() const
    {
        return _group;
    }

    void Beam::hitting_target_length(float value)
    {
        _hitting_target_length = value;
    }
    float Beam::hitting_target_length() const
    {
        return _hitting_target_length;
    }

    void Beam::is_active(bool value)
    {
        _is_active = value;
    }
    bool Beam::is_active() const
    {
        return _is_active;
    }
    void Beam::toggle_active()
    {
        _is_active = !_is_active;
    }

    void Beam::hit_particle_cooldown(float value)
    {
        _hit_particle_cooldown = value;
    }
    float Beam::hit_particle_cooldown() const
    {
        return _hit_particle_cooldown;
    }

    void Beam::dps(float value)
    {
        _dps = value;
    }
    float Beam::dps() const
    {
        return _dps;
    }

    void Beam::update(float dt)
    {
        auto length = _hitting_target_length < 0.0f ? _max_length : _hitting_target_length;
        _shape.setSize(sf::Vector2f(length, _width));
        _shape.setRotation(_hitbox.rotation());
        _shape.setPosition(_hitbox.origin());

        auto top_pos = _hitbox.direction() * length + _hitbox.origin();
        _shape_top.setRotation(_hitbox.rotation());
        _shape_top.setPosition(top_pos);

        _hit_particle_cooldown += dt;
    }

    void Beam::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(_shape, states);
        target.draw(_shape_top, states);
    }
    // }}}

    // BeamManager {{{
    void BeamManager::remove_beam(Beam *beam)
    {
        _objects[beam->index()] = nullptr;
    }


    Beam *BeamManager::spawn_beam(const std::string &name, Group::Type group)
    {
        auto find = _src_beams.find(name);
        if (find == _src_beams.end())
        {
            return nullptr;
        }

        auto index = find_empty_object();
        if (index == Utils::max_uint)
        {
            return nullptr;
        }

        auto new_beam = new Beam(*find->second.get());
        new_beam->group(group);
        insert_object(new_beam, index);
        return new_beam;
    }
    Beam *BeamManager::spawn_beam(const Beam &blueprint, Group::Type group)
    {
        auto index = find_empty_object();
        if (index == Utils::max_uint)
        {
            return nullptr;
        }

        auto new_beam = new Beam(blueprint);
        new_beam->group(group);
        insert_object(new_beam, index);
        return new_beam;
    }

    void BeamManager::add_src_beam(const std::string &name, Beam *beam)
    {
        _src_beams[name] = std::unique_ptr<Beam>(beam);
    }
    Beam *BeamManager::find_src_beam(const std::string &name) const
    {
        auto find = _src_beams.find(name);
        if (find == _src_beams.end())
        {
            return nullptr;
        }

        return find->second.get();
    }

    void BeamManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.blendMode = sf::BlendAdd;

        for (auto i = 0u; i < _objects.size(); i++)
        {
            auto beam = _objects[i].get();
            if (beam != nullptr && beam->is_active())
            {
                target.draw(*beam, states);
            }
        }
    }
    // }}}

}
