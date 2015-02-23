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
        auto position = Utils::para_lerp(_init_position, _target_position, dl);  
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
        _particles(num_particles)
    {
        for (auto i = 0u; i < num_particles; i++)
        {
            _particles[i] = Particle();
            _particles[i].lifetime(i * 0.1f);
            init_particle(_particles[i]);
        }
    }

    void ParticleSystem::update(float dt)
    {
        for (auto i = 0u; i < _particles.size(); i++)
        {
            auto &particle = _particles[i]; 
            particle.update(dt);
            if (!particle.active())
            {
                particle.reset();
                init_particle(particle);
            }
        }
    }

    void ParticleSystem::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        for (auto particle : _particles)
        {
            target.draw(particle, states);
        }
    }

    void ParticleSystem::init_particle(Particle &particle)
    {
        auto angle = Utils::randf(0, M_PI * 2.0f);
        auto x = cos(angle) * 10.0f;
        auto y = sin(angle) * 10.0f;

        particle.positions(sf::Vector2f(0, 0), sf::Vector2f(x, y));
        particle.scales(0.7f, 0.1f);
        particle.texture(_texture);
    }
    // }}}
}
