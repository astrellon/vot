#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>
#include <stdint.h>

namespace vot
{
    // Particle {{{
    class Particle : public sf::Sprite
    {
        public:
            Particle();

            void texture(const sf::Texture &texture);

            void update(float dt);

            void reset();

            void active(bool value);
            bool active() const;

            void lifetime(float value);
            float lifetime() const;

            void total_lifetime(float value);
            float total_lifetime() const;

            void scales(float init, float target);
            void positions(const sf::Vector2f &init, const sf::Vector2f &target);
            void alphas(uint8_t init, uint8_t target);

        private:
            float _lifetime;
            float _total_lifetime;
            bool _active;

            float _init_scale;
            float _target_scale;

            uint8_t _init_alpha;
            uint8_t _target_alpha;

            sf::Vector2f _init_position;
            sf::Vector2f _target_position;
    };
    // }}}

    // ParticleSystem {{{
    class ParticleSystem : public sf::Drawable, public sf::Transformable
    {
        public:
            ParticleSystem(const sf::Texture &texture, uint32_t num_particles);

            void init();

            void system_type(uint32_t value);
            uint32_t system_type() const;

            void loop_system(bool value);
            bool loop_system() const;

            void auto_remove(bool value);
            bool auto_remove() const;

            void spawning_active(bool active);
            bool spawning_active() const;

            void speed_factor(float value);
            float speed_factor() const;

            void scale_factor(float value);
            float scale_factor() const;

            void spawn_rate(float dt);
            float spawn_rate() const;

            void spawn_rotation_offset(float offset);
            float spawn_rotation_offset() const;

            bool update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            const sf::Texture &_texture;
            std::vector<Particle> _particles;
            bool _loop_system;
            bool _auto_remove;
            bool _active;
            bool _spawning_active;

            uint32_t _system_type;
            float _speed_factor;
            float _scale_factor;
            float _spawn_rate;
            float _spawn_cooldown;
            float _spawn_rotation_offset;

            void init_particle(Particle &particle, bool on_init);
    };
    // }}}
    
    // ParticleSystemManager {{{
    class ParticleSystemManager : public sf::Drawable
    {
        public:
            ParticleSystem *spawn_system(const sf::Texture &texture, uint32_t num_particles);

            void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            std::vector<std::unique_ptr<ParticleSystem> > _active_systems;
    };
    // }}}
}
