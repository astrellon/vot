#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <map>
#include <memory>
#include <array>

#include "utils/circle.h"
#include "object_manager.h"
#include "common.h"

namespace vot
{
    class Character;

    // Bullet {{{
    class Bullet : public sf::Drawable
    {
        public:
            Bullet(float damage);
            Bullet(const Bullet &clone);

            virtual void damage(float value);
            virtual float damage() const;

            virtual void update(float dt) = 0;

            virtual bool active() const = 0;
            virtual bool dead() const = 0;

            void index(uint32_t value);
            uint32_t index() const;

            virtual sf::Vector2f location() const = 0;
            virtual void scale(float value) = 0;

            void group(Group::Type value);
            Group::Type group() const;

            utils::Circle &hitbox();

        private:
            float _damage;
            uint32_t _index;
            utils::Circle _hitbox;
            Group::Type _group;
    };
    // }}}
    
    // PatternBullet {{{
    class PatternBullet : public Bullet
    {
        public:
            PatternBullet(const sf::Texture &texture, float damage);
            PatternBullet(const PatternBullet &clone);

            virtual sf::Vector2f location() const;
            virtual void scale(float value);

            void init_transform(sf::Transform trans);
            const sf::Transform &init_transform() const;

            void pattern_type(uint32_t type);
            uint32_t pattern_type() const;

            float lifetime() const;

            void total_lifetime(float value);
            float total_lifetime() const;

            virtual bool active() const;
            virtual bool dead() const;

            virtual void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const; 

        private:
            sf::Transform _init_transform;
            float _lifetime;
            float _total_lifetime;
            uint32_t _pattern_type;
            sf::Sprite _sprite;

    };
    // }}}

    // HomingBullet {{{
    class HomingBullet : public Bullet
    {
        public:
            HomingBullet(const sf::Texture &texture, float damage);
            HomingBullet(const HomingBullet &clone);
            
            void setup(const sf::Vector2f &location, float angle);

            virtual sf::Vector2f location() const;
            virtual void scale(float value);

            void target(const Character *value);
            const Character *target() const;
            
            float lifetime() const;

            void total_lifetime(float value);
            float total_lifetime() const;

            virtual bool active() const;
            virtual bool dead() const;

            virtual void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const; 

        private:
            const Character *_target;

            float _lifetime;
            float _total_lifetime;
            float _tracking_time;
            float _scale;

            std::array<sf::Vector2f, 10> _prev_positions;
            int8_t _prev_position_index;
            int8_t _prev_position_count;
            float _prev_record_cooldown;

            sf::Sprite _sprite;
    };
    // }}}

    // BulletManager {{{
    class BulletManager : public sf::Drawable, public ObjectManager<Bullet, 2048>
    {
        public:
            void remove_bullet(Bullet *bullet);

            PatternBullet *spawn_pattern_bullet(const std::string &name, Group::Type group);
            PatternBullet *spawn_pattern_bullet(const PatternBullet &blueprint, Group::Type group);
            HomingBullet *spawn_homing_bullet(const std::string &name, Group::Type group);
            HomingBullet *spawn_homing_bullet(const HomingBullet &blueprint, Group::Type group);

            void add_src_pattern_bullet(const std::string &name, PatternBullet *bullet);
            void add_src_homing_bullet(const std::string &name, HomingBullet *bullet);

            PatternBullet *find_src_pattern_bullet(const std::string &name) const;
            HomingBullet *find_src_homing_bullet(const std::string &name) const;

            //void draw(sf::RenderWindow &window);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            std::map<std::string, std::unique_ptr<PatternBullet> > _src_pattern_bullets;
            std::map<std::string, std::unique_ptr<HomingBullet> > _src_homing_bullets;
    };
    // }}}
}
