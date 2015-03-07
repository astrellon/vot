#include "particles.h"

#include "utils.h"
#include <iostream>

namespace vot
{
    // Particle {{{
    Particle::Particle() :
        _lifetime(0.0f),
        _total_lifetime(1.0f),
        _active(true),
        _init_scale(1.0f),
        _target_scale(1.0f)
    {

    }

    void Particle::texture(const sf::Texture &texture)
    {
        auto size = texture.getSize();
        setOrigin(size.x * 0.5f, size.y * 0.5f);
        setTexture(texture);
    }

    void Particle::update(float dt)
    {
        if (!_active)
        {
            return;
        }

        _lifetime += dt;
        if (_lifetime < 0.0f)
        {
            return;
        }

        auto dl = _lifetime / _total_lifetime;
        auto position = Utils::lerp(_init_position, _target_position, dl);  
        setPosition(position);
        auto scale = Utils::para_lerp(_init_scale, _target_scale, dl);
        setScale(scale, scale);

        if (_lifetime > _total_lifetime)
        {
            _active = false;
        }
    }

    void Particle::reset()
    {
        _lifetime = 0.0f;
        _active = true;
    }

    void Particle::active(bool value)
    {
        _active = value;
    }
    bool Particle::active() const
    {
        return _active;
    }

    void Particle::lifetime(float value)
    {
        _lifetime = value;
    }
    float Particle::lifetime() const
    {
        return _lifetime;
    }

    void Particle::total_lifetime(float value)
    {
        _total_lifetime = value;
    }
    float Particle::total_lifetime() const
    {
        return _total_lifetime;
    }

    void Particle::scales(float init, float target)
    {
        _init_scale = init;
        _target_scale = target;
    }
    void Particle::positions(const sf::Vector2f &init, const sf::Vector2f &target)
    {
        _init_position = init;
        _target_position = target;
    }
    // }}}

    // ParticleSystem {{{
    ParticleSystem::ParticleSystem(const sf::Texture &texture, uint32_t num_particles) :
        sf::Drawable(),
        sf::Transformable(),
        _texture(texture),
        _particles(num_particles),
        _loop_system(false),
        _system_type(0u)
    {
    }

    void ParticleSystem::init()
    {
        for (auto i = 0u; i < _particles.size(); i++)
        {
            _particles[i] = Particle();
            init_particle(_particles[i]);
        }
    }

    void ParticleSystem::system_type(uint32_t value)
    {
        _system_type = value;
    }
    uint32_t ParticleSystem::system_type() const
    {
        return _system_type;
    }

    void ParticleSystem::loop_system(bool value)
    {
        _loop_system = value;
    }
    bool ParticleSystem::loop_system() const
    {
        return _loop_system;
    }

    bool ParticleSystem::update(float dt)
    {
        auto active = 0u;
        for (auto i = 0u; i < _particles.size(); i++)
        {
            auto &particle = _particles[i]; 
            particle.update(dt);
            if (!particle.active())
            {
                if (_loop_system)
                {
                    particle.reset();
                    init_particle(particle);
                }
            }
            else
            {
                ++active;
            }
        }
        return active > 0u;
    }

    void ParticleSystem::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        states.blendMode = sf::BlendAdd;

        for (auto particle : _particles)
        {
            if (particle.lifetime() < 0.0f)
            {
                continue;
            }
            target.draw(particle, states);
        }
    }

    void ParticleSystem::init_particle(Particle &particle)
    {
        if (_system_type == 0u)
        {
            auto angle = Utils::randf(-0.8f, 0.8f);
            auto dist = Utils::randf(20, 28);
            auto x = cos(angle) * dist;
            auto y = sin(angle) * dist;

            particle.positions(sf::Vector2f(0, 0), sf::Vector2f(x, y));
            particle.scales(0.7f, 0.05f);
            particle.texture(_texture);
        }
        else if (_system_type == 1u)
        {
            auto angle = Utils::randf(0, Utils::pi * 2.0f);
            auto dist = Utils::randf(12, 20);
            auto x = cos(angle) * dist;
            auto y = sin(angle) * dist;

            particle.positions(sf::Vector2f(x, y), sf::Vector2f(0, 0));
            particle.scales(0.05f, 0.5f);
            particle.texture(_texture);

            auto lifetime_offset = Utils::randf(0, 1.0f);
            particle.lifetime(-lifetime_offset);
        }
    }
    // }}}
    
    // ParticleSystemManager {{{
    ParticleSystem *ParticleSystemManager::spawn_system(const sf::Texture &texture, uint32_t num_particles)
    {
        auto system = new ParticleSystem(texture, num_particles);
        _active_systems.push_back(std::unique_ptr<ParticleSystem>(system));

        return system;
    }

    void ParticleSystemManager::update(float dt)
    {
        for (auto i = 0u; i < _active_systems.size(); i++)
        {
            if (!_active_systems[i]->update(dt))
            {
                _active_systems.erase(_active_systems.begin() + i);
                --i;
            }
        }
    }

    void ParticleSystemManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        for (auto i = 0u; i < _active_systems.size(); i++)
        {
            target.draw(*_active_systems[i].get(), states);
        }
    }
    // }}}
}
