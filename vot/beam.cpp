#include "beam.h"

#include "utils.h"

namespace vot
{
    // Beam {{{
    Beam::Beam() :
        _index(Utils::max_uint),
        _active(false),
        _group(Group::NATURE),
        _max_length(100.0f),
        _hitting_target_length(-1.0f)
    {

    }

    Ray &Beam::hitbox()
    {
        return _hitbox;
    }
    const Ray &Beam::hitbox() const
    {
        return _hitbox;
    }

    void Beam::active(bool value)
    {
        _active = value;
    }
    bool Beam::active() const
    {
        return _active;
    }

    void Beam::max_length(float value)
    {
        _max_length = value;
    }
    float Beam::max_length() const
    {
        return _max_length;
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

    void Beam::update(float dt)
    {
        auto length = _hitting_target_length < 0.0f ? _max_length : _hitting_target_length;
        _shape.setOrigin(0.0f, 3.0f);
        _shape.setSize(sf::Vector2f(length, 6.0f));
        _shape.setRotation(_hitbox.rotation());
    }

    void Beam::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (!_active)
        {
            return;
        }

        target.draw(_shape, states);
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

    void BeamManager::add_src_beam(const std::string &name, Beam *beam)
    {
        _src_beams[name] = std::unique_ptr<Beam>(beam);
    }

    void BeamManager::update(float dt)
    {
        for (auto i = 0u; i < _objects.size(); i++)
        {
            auto beam = _objects[i].get();
            if (beam != nullptr)
            {
                beam->update(dt);
            }
        }
    }
    void BeamManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        for (auto i = 0u; i < _objects.size(); i++)
        {
            auto beam = _objects[i].get();
            if (beam != nullptr)
            {
                target.draw(*beam, states);
            }
        }
    }
    // }}}

}