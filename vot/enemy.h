#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <array>

#include "object_manager.h"
#include "character.h"
#include "icontroller.h"

namespace vot
{
    // Enemy {{{
    class Enemy : public Character
    {
        public:
            Enemy(const sf::Texture &texture);
            Enemy(const Enemy &clone);

            void controller(IController *controller);
            IController *controller() const;

            void update(float dt);

            void index(uint32_t value);
            uint32_t index() const;

        private:
            uint32_t _index;

            std::unique_ptr<IController> _controller;
    };
    // }}}

    // EnemyManager {{{
    class EnemyManager : public sf::Drawable, public ObjectManager<Enemy, 128>
    {
        public:
            EnemyManager();

            void remove_enemy(Enemy *enemy);
            Enemy *spawn_enemy(const std::string &name);

            void add_src_enemy(const std::string &name, Enemy *enemy);
            uint32_t num_enemies() const;

            void clear_enemies();

            typedef std::map<std::string, std::unique_ptr<Enemy> > EnemyMap;
            const EnemyMap &src_enemies() const;

            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            std::vector<std::unique_ptr<Enemy> > _dead_enemies;

            EnemyMap _src_enemies;
            uint16_t _enemy_counter;
            uint32_t _num_enemies;
    };
    // }}}
}
