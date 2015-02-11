#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <map>
#include <memory>
#include <array>

#include "circle.h"

namespace vot
{
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

            Circle &hitbox();

        private:
            float _damage;
            uint32_t _index;
            Circle _hitbox;
    };
    // }}}
    
    class PatternBullet : public Bullet
    {
        public:
            PatternBullet(const sf::Texture &texture, float damage);
            PatternBullet(const PatternBullet &clone);

            void init_transform(sf::Transform trans);
            const sf::Transform &init_transform() const;

            void pattern_type(uint32_t type);
            uint32_t pattern_type() const;

            virtual bool active() const;
            virtual bool dead() const;

            virtual void update(float dt);

        private:
            sf::Transform _init_transform;
            float _lifetime;
            float _total_lifetime;
            uint32_t _pattern_type;

    };

    // BulletManager {{{
    class BulletManager
    {
        public:
            BulletManager();

            void remove_bullet(Bullet *bullet);
            PatternBullet *spawn_pattern_bullet(sf::Texture &texture, float damamge = 1.0f);
            PatternBullet *clone_pattern_bullet(const std::string &name);

            void add_src_pattern_bullet(PatternBullet *bullet, const std::string &name);

            void draw(sf::RenderWindow &window);

            typedef std::array<std::unique_ptr<Bullet>, 2048> BulletList;
            BulletList *bullets();

        private:
            uint32_t _bullet_index;
            BulletList _bullets;

            std::map<std::string, std::unique_ptr<PatternBullet> > _src_pattern_bullets;

            uint32_t find_empty_bullet() const;

            void insert_bullet(Bullet *bullet, uint32_t index);
    };
    // }}}
}
