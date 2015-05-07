#pragma once

#include <SFML/Graphics.hpp>

#include "bullet.h"
#include "beam.h"

#include <utils/data.h>

namespace vot
{
    class HardpointPlacement;
    class Character;
    class ParticleSystem;

    // Hardpoint {{{
    class Hardpoint : public sf::Drawable, public sf::Transformable
    {
        public:
            Hardpoint();
            Hardpoint(const ::utils::Data *data);
            Hardpoint(const Hardpoint &clone);

            virtual Hardpoint *clone() const = 0;

            void parent(HardpointPlacement *value);
            HardpointPlacement *parent() const;

            Character *parent_char() const;

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

            void name(const std::string &value);
            std::string name() const;

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

            virtual void serialise(::utils::Data *data) const;
            virtual void deserialise(const ::utils::Data *data);

            static Hardpoint *create_from_data(const ::utils::Data *data);

        protected:
            void cooldown(float value);
            void change_cooldown(float delta);

        private:
            sf::Sprite _sprite;
            float _cooldown;
            float _max_cooldown;
            HardpointPlacement *_parent;
            Character *_target;
            std::string _name;

            float _max_angle;
            float _min_angle;
            bool _track_ahead;
    };
    // }}}

    // PatternBulletHardpoint {{{
    class PatternBulletHardpoint : public Hardpoint
    {
        public:
            PatternBulletHardpoint();
            PatternBulletHardpoint(const PatternBullet *blueprint);
            PatternBulletHardpoint(const ::utils::Data *data);
            PatternBulletHardpoint(const PatternBulletHardpoint &clone);

            virtual Hardpoint *clone() const;

            void pattern_type(uint32_t type);
            uint32_t pattern_type() const;

            virtual float projectile_speed() const;

            void blueprint(PatternBullet *value);
            const PatternBullet *blueprint() const;

            virtual void update(float dt);
            virtual void fire();
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

            virtual void serialise(::utils::Data *data) const;
            virtual void deserialise(const ::utils::Data *data);

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
            HomingBulletHardpoint();
            HomingBulletHardpoint(const HomingBullet *blueprint);
            HomingBulletHardpoint(const ::utils::Data *data);
            HomingBulletHardpoint(const HomingBulletHardpoint &clone);

            virtual Hardpoint *clone() const;

            virtual void fire();

            void blueprint(HomingBullet *value);
            const HomingBullet *blueprint() const;

            virtual void serialise(::utils::Data *data) const;
            virtual void deserialise(const ::utils::Data *data);

        private:
            const HomingBullet *_blueprint;
    };
    // }}}

    // BeamHardpoint {{{
    class BeamHardpoint : public Hardpoint
    {
        public:
            BeamHardpoint();
            BeamHardpoint(const Beam *blueprint);
            BeamHardpoint(const ::utils::Data *data);
            BeamHardpoint(const BeamHardpoint &clone);

            virtual Hardpoint *clone() const;

            virtual void update(float dt);
            virtual void fire();

            void blueprint(Beam *value);
            const Beam *blueprint() const;

            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
            
            virtual void serialise(::utils::Data *data) const;
            virtual void deserialise(const ::utils::Data *data);

        private:
            const Beam *_blueprint;
            Beam *_active_beam;
            bool _prev_charging_up;;
            bool _fire_beam;
            sf::Sprite _beam_glow;

            ParticleSystem *_charge_up_system;
            float _charge_up;

            void init();
    };
    // }}}
    
    // HardpointManager {{{
    class HardpointManager
    {
        public:
            static PatternBulletHardpoint *spawn_pattern_hardpoint(const std::string &name, const std::string &blueprint);
            static HomingBulletHardpoint *spawn_homing_hardpoint(const std::string &name, const std::string &blueprint);
            static BeamHardpoint *spawn_beam_hardpoint(const std::string &name, const std::string &blueprint);

            static void add_src_pattern_hardpoint(const std::string &name, PatternBulletHardpoint *point);
            static void add_src_homing_hardpoint(const std::string &name, HomingBulletHardpoint *point);
            static void add_src_beam_hardpoint(const std::string &name, BeamHardpoint *point);
            
            typedef std::map<std::string, std::unique_ptr<PatternBulletHardpoint> > PatternMap; 
            typedef std::map<std::string, std::unique_ptr<HomingBulletHardpoint> > HomingMap;
            typedef std::map<std::string, std::unique_ptr<BeamHardpoint> > BeamMap;

        private:
            static PatternMap s_src_pattern_hardpoints;
            static HomingMap s_src_homing_hardpoints;
            static BeamMap s_src_beam_hardpoints;
    };
    // }}}
    
}
