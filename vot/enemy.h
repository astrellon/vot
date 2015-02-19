#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <array>

#include "character.h"

namespace vot
{
    // Enemy {{{
    class Enemy : public Character
    {
        public:
            Enemy(const sf::Texture &texture);
            Enemy(const Enemy &clone);

            void update(float dt);

            void enemy_type(uint32_t value);
            uint32_t enemy_type() const;

            void index(uint32_t value);
            uint32_t index() const;

        private:
            uint32_t _enemy_type;
            uint32_t _bullet_count;
            float _firing_angle;
            float _cooldown;
            uint32_t _index;
    };
    // }}}

    // EnemyManager {{{
    class EnemyManager : public sf::Drawable
    {
        public:
            EnemyManager();

            void remove_enemy(Enemy *enemy);
            Enemy *spawn_enemy(const std::string &name);

            void add_src_enemy(Enemy *enemy, const std::string &name);
            uint32_t num_enemies() const;

            typedef std::array<std::unique_ptr<Enemy>, 128> EnemyList;
            EnemyList *enemies();
            const EnemyList *enemies() const;

            typedef std::map<std::string, std::unique_ptr<Enemy> > EnemyMap;
            const EnemyMap &src_enemies() const;

            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            EnemyList _enemies;
            std::vector<std::unique_ptr<Enemy> > _dead_enemies;

            EnemyMap _src_enemies;
            uint32_t _enemy_index;
            uint16_t _enemy_counter;
            uint32_t _num_enemies;
            
            uint32_t find_empty_enemy() const;

            void insert_enemy(Enemy *enemy, uint32_t index);

    };
    // }}}
}
