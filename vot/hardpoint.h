#pragma once

#include <SFML/Graphics.hpp>

#include "bullet.h"
#include "beam.h"

namespace vot
{
    class Character;
    class ParticleSystem;

    class Hardpoint : public sf::Drawable, public sf::Transformable
    {
        public:
            Hardpoint(Group::Type group);

            void parent(Character *value);
            Character *parent() const;

            void target(Character *value);
            Character *target() const;

            void texture(const sf::Texture *value);
            const sf::Texture *texture() const;

            sf::Sprite &sprite();

            void max_angle(float value);
            float max_angle() const;
            
            void min_angle(float value);
            float min_angle() const;

            void setup(float x, float y, float rotation, float min = 0.0f, float max = 360.0f);

            void max_cooldown(float value);
            float max_cooldown() const;
            float cooldown() const;
            
            Group::Type group() const;

            virtual void update(float dt);
            virtual void fire() = 0;
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        protected:
            void cooldown(float value);
            void change_cooldown(float delta);

        private:
            float _cooldown;
            float _max_cooldown;
            Character *_parent;
            Character *_target;
            Group::Type _group;
            sf::Sprite _sprite;

            float _max_angle;
            float _min_angle;
    };

    class PatternBulletHardpoint : public Hardpoint
    {
        public:
            PatternBulletHardpoint(const PatternBullet &blueprint, Group::Type group);

            void pattern_type(uint32_t type);
            uint32_t pattern_type() const;

            virtual void update(float dt);
            virtual void fire();
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            const PatternBullet &_blueprint;
            uint32_t _pattern_type;
            bool _fire_bullet;
    };

    class HomingBulletHardpoint : public Hardpoint
    {
        public:
            HomingBulletHardpoint(const HomingBullet &blueprint, Group::Type group);

            virtual void fire();

        private:
            const HomingBullet &_blueprint;
    };

    class BeamHardpoint : public Hardpoint
    {
        public:
            BeamHardpoint(const Beam &blueprint, Group::Type group);

            virtual void update(float dt);
            virtual void fire();

            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            const Beam &_blueprint;
            Beam *_active_beam;
            bool _prev_charging_up;;
            bool _fire_beam;
            sf::Sprite _beam_glow;

            ParticleSystem *_charge_up_system;
            float _charge_up;
    };
}
