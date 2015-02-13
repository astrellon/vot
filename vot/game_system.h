#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <memory>

#include "bullet.h"
#include "player.h"
#include "enemy.h"

namespace vot
{
    class GameSystem : public sf::Drawable
    {
        public:
            GameSystem(sf::RenderWindow &window);

            sf::RenderWindow &window() const;

            BulletManager &bullet_manager();
            void create_default_bullets();

            EnemyManager &enemy_manager();
            void create_default_enemieS();

            void player(Player *value);
            Player *player() const;

            void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

            static void main(GameSystem *main);
            static GameSystem *main();

        private:
            sf::RenderWindow &_window;

            BulletManager _bullet_manager;
            std::unique_ptr<Player> _player;
            EnemyManager _enemy_manager;

            uint16_t _id_counter;

            static GameSystem *s_main;
    };
}
