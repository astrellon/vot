#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <map>
#include <memory>
#include <array>

#include "circle.h"

namespace vot
{
    class Character;

    // Bullet {{{
    class Bullet : public sf::Sprite
    {
        public:
            Bullet(const sf::Texture &texture, float damage);
            Bullet(const Bullet &clone);

            virtual void damage(float value);
            virtual float damage() const;

            virtual void update(float dt) = 0;

            virtual bool active() const = 0;
            virtual bool dead() const = 0;

            void index(uint32_t value);
            uint32_t index() const;

            void owner(uint16_t value);
            uint16_t owner() const;

            sf::Vector2f center() const;

            enum Group
            {
                PLAYER,
                ENEMY,
                NATURE
            };
            void group(Group value);
            Group group() const;

            Circle &hitbox();

        private:
            float _damage;
            uint32_t _index;
            Circle _hitbox;
            uint16_t _owner;
            Group _group;
    };
    // }}}
    
    // PatternBullet {{{
    class PatternBullet : public Bullet
    {
        public:
            PatternBullet(const sf::Texture &texture, float damage);
            PatternBullet(const PatternBullet &clone);

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

        private:
            sf::Transform _init_transform;
            float _lifetime;
            float _total_lifetime;
            uint32_t _pattern_type;

    };
    // }}}

    // HomingBullet {{{
    class HomingBullet : public Bullet
    {
        public:
            HomingBullet(const sf::Texture &texture, float damage);
            HomingBullet(const HomingBullet &clone);

            void target(const Character *value);
            const Character *target() const;
            
            float lifetime() const;

            void total_lifetime(float value);
            float total_lifetime() const;

            virtual bool active() const;
            virtual bool dead() const;

            virtual void update(float dt);

        private:
            const Character *_target;

            float _lifetime;
            float _total_lifetime;
            float _tracking_time;
    };
    // }}}

    // BulletManager {{{
    class BulletManager : public sf::Drawable
    {
        public:
            BulletManager();

            void remove_bullet(Bullet *bullet);
            PatternBullet *spawn_pattern_bullet(const std::string &name, uint16_t owner, Bullet::Group group);
            HomingBullet *spawn_homing_bullet(const std::string &name, uint16_t owner, Bullet::Group group);

            void add_src_pattern_bullet(PatternBullet *bullet, const std::string &name);
            void add_src_homing_bullet(HomingBullet *bullet, const std::string &name);

            //void draw(sf::RenderWindow &window);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

            typedef std::array<std::unique_ptr<Bullet>, 2048> BulletList;
            BulletList *bullets();

        private:
            uint32_t _bullet_index;
            BulletList _bullets;

            std::map<std::string, std::unique_ptr<PatternBullet> > _src_pattern_bullets;
            std::map<std::string, std::unique_ptr<HomingBullet> > _src_homing_bullets;

            uint32_t find_empty_bullet() const;

            void insert_bullet(Bullet *bullet, uint32_t index);
    };
    // }}}
}
