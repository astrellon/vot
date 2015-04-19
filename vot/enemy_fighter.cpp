#include "enemy_fighter.h"

#include "game_system.h"

namespace vot
{
    EnemyFighter::EnemyFighter(Enemy *controlling) :
        _controlling(controlling),
        _cooldown(0.0f)
    {

    }

    void EnemyFighter::update(float dt)
    {
        if (_controlling == nullptr)
        {
            return;
        }

        auto gs = GameSystem::game();
        auto player = gs->player();

        auto to_player = player->location() - _controlling->location();
        auto dist = utils::Utils::vector_length(to_player);
        auto unit_player = to_player / dist;

        if (dist > 200.0f)
        {
            //_controlling->move(unit_player * dt * 50.0f);
            _controlling->acceleration(unit_player * 50.0f);
        }
        else if (dist < 100.0f)
        {
            //_controlling->move(unit_player * dt * -50.0f);
            _controlling->acceleration(unit_player * -50.0f);
        }
        else
        {
            // Move side ways.
            sf::Transform trans;
            trans.rotate(90.0f);
            auto sideways = trans.transformPoint(unit_player);
            //_controlling->move(sideways * dt * 50.0f);
        }

        // Arena
        if (_cooldown <= 0.0f)
        {
            for (auto i = 0; i < 16; i++)
            {
                auto bullet = GameSystem::bullet_manager()->spawn_pattern_bullet("straight_red_circle", Group::ENEMY);
                if (bullet == nullptr)
                {
                    continue;
                }
                auto trans = _controlling->getTransform();
                trans.rotate(i * 22.5f);
                bullet->init_transform(trans);
            }
            _cooldown = 0.5f;
        }
        _cooldown -= dt;
    }
}
