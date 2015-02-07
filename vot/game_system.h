#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <memory>

#include "bullet.h"
#include "player.h"

namespace vot
{
    class GameSystem
    {
        public:
            GameSystem(sf::RenderWindow &window);

            sf::RenderWindow &window() const;

            BulletManager &bullet_manager();
            void create_default_bullets();

            void player(Player *value);
            Player *player() const;

            void update(float dt);
            void draw(sf::RenderWindow &window);

        private:
            sf::RenderWindow &_window;

            BulletManager _bullet_manager;
            std::unique_ptr<Player> _player;

    };
}
