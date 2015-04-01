#include "level1.h"

#include "game_system.h"
#include "game.h"
#include "enemy.h"
#include "enemy_fighter.h"

namespace vot
{
    void Level1::init()
    {
        _spawn_timer = 0.0f;
    }
    void Level1::deinit()
    {

    }

    std::string Level1::name() const
    {
        return "Level 1";
    }

    void Level1::update(float dt)
    {
        auto enemy_manager = GameSystem::enemy_manager();
        auto game = GameSystem::game();

        if (true && enemy_manager->num_enemies() < 3)
        {
            _spawn_timer += dt;
            if (_spawn_timer > 3.0f)
            {
                auto enemy = enemy_manager->spawn_enemy("enemy1");
                enemy->controller(new EnemyFighter(enemy));
                enemy->translate(utils::Utils::rand_vec(-200.0f, 200.0f));
                _spawn_timer = 0.0f;

                if (game->player()->target() == nullptr && game->player()->auto_target())
                {
                    game->player()->target(enemy);
                }
            }
        }
    }
}
