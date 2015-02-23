#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <stdint.h>

namespace vot
{
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

        private:
            float _lifetime;
            float _total_lifetime;
            bool _active;

            float _init_scale;
            float _target_scale;

            sf::Vector2f _init_position;
            sf::Vector2f _target_position;
    };

    class ParticleSystem : public sf::Drawable, public sf::Transformable
    {
        public:
            ParticleSystem(const sf::Texture &texture, uint32_t num_particles);

            void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            const sf::Texture &_texture;
            std::vector<Particle> _particles;

            void init_particle(Particle &particle);
    };
}
