#include "enemy.h"

#include "game_system.h"

namespace vot
{
    Enemy::Enemy(const sf::Texture &texture) :
        Character(texture),
        _enemy_type(0u),
        _bullet_count(0u),
        _firing_angle(0.0f),
        _cooldown(0.0f)
    {

    }

    void Enemy::update(float dt)
    {
        auto gs = GameSystem::main();
        if (_enemy_type == 0u)
        {
            // Arena
            if (_cooldown <= 0.0f)
            {
                for (auto i = 0; i < 16; i++)
                {
                    auto bullet = gs->bullet_manager().spawn_pattern_bullet("straight_blue", id());
                    auto trans = forward_center_trans();
                    trans.rotate(i * 22.5);
                    bullet->init_transform(trans);
                }
                _cooldown = 0.5f;
            }
            _cooldown -= dt;

            auto rot_speed = 45.0f * dt;
            rotateBy(rot_speed);
        }
    }

    void Enemy::enemy_type(uint32_t value)
    {
        _enemy_type = value;
    }
    uint32_t Enemy::enemy_type() const
    {
        return _enemy_type;
    }
}
