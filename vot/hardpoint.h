#pragma once

#include <SFML/Graphics.hpp>

#include "bullet.h"
#include "beam.h"

#include <utils/data.h>

namespace vot
{
    class Character;
    class ParticleSystem;

    // Hardpoint {{{
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

            void track_ahead(bool value);
            bool track_ahead() const;

            virtual float projectile_speed() const;

            // Sets up the x,y position, initial rotation, and min and max angle restrictions.
            void setup(float x, float y, float rotation, float min = 0.0f, float max = 360.0f);

            void max_cooldown(float value);
            float max_cooldown() const;
            float cooldown() const;
            
            Group::Type group() const;

            virtual void update(float dt);
            virtual void fire() = 0;
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

            ::utils::Data *serialise() const;
            void deserialise(::utils::Data *data);

        protected:
            void cooldown(float value);
            void change_cooldown(float delta);

            virtual void do_serialise(::utils::Data *data) const;

        private:
            float _cooldown;
            float _max_cooldown;
            Character *_parent;
            Character *_target;
            Group::Type _group;
            sf::Sprite _sprite;

            float _max_angle;
            float _min_angle;
            bool _track_ahead;
    };
    // }}}

    // PatternBulletHardpoint {{{
    class PatternBulletHardpoint : public Hardpoint
    {
        public:
            PatternBulletHardpoint(const PatternBullet *blueprint, Group::Type group);

            void pattern_type(uint32_t type);
            uint32_t pattern_type() const;

            virtual float projectile_speed() const;

            const PatternBullet *blueprint() const;

            virtual void update(float dt);
            virtual void fire();
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

            virtual void deserialise(::utils::Data *data);

        protected:
            virtual void do_serialise(::utils::Data *data) const;

        private:
            const PatternBullet *_blueprint;
            uint32_t _pattern_type;
            bool _fire_bullet;
    };
    // }}}

    // HomingBulletHardpoint {{{
    class HomingBulletHardpoint : public Hardpoint
    {
        public:
            HomingBulletHardpoint(const HomingBullet *blueprint, Group::Type group);

            virtual void fire();

            const HomingBullet *blueprint() const;

            virtual void deserialise(::utils::Data *data);

        protected:
            virtual void do_serialise(::utils::Data *data) const;

        private:
            const HomingBullet *_blueprint;
    };
    // }}}

    // BeamHardpoint {{{
    class BeamHardpoint : public Hardpoint
    {
        public:
            BeamHardpoint(const Beam *blueprint, Group::Type group);

            virtual void update(float dt);
            virtual void fire();

            const Beam *blueprint() const;

            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
            
            virtual void deserialise(::utils::Data *data);
        
        protected:
            virtual void do_serialise(::utils::Data *data) const;

        private:
            const Beam *_blueprint;
            Beam *_active_beam;
            bool _prev_charging_up;;
            bool _fire_beam;
            sf::Sprite _beam_glow;

            ParticleSystem *_charge_up_system;
            float _charge_up;
    };
    // }}}
    
    // HardpointPlacement {{{
    class HardpointPlacement
    {
        public:
            HardpointPlacement();
            HardpointPlacement(float x, float y, float min, float max);

            void setup(float x, float y, float min, float max);

            void hardpoint(Hardpoint *point);
            Hardpoint *hardpoint() const;

            void position(const sf::Vector2f &position);
            sf::Vector2f position() const;

            void max_angle(float value);
            float max_angle() const;
            
            void min_angle(float value);
            float min_angle() const;

        private:
            Hardpoint *_hardpoint;
            sf::Vector2f _position;
            float _min_angle;
            float _max_angle;
    };
    // }}}
}
