#include "enemy.h"

#include "game_system.h"
#include "utils.h"
#include "common.h"

namespace vot
{
    // Enemy {{{
    Enemy::Enemy(const sf::Texture &texture) :
        Character(texture),
        _enemy_type(0u),
        _bullet_count(0u),
        _firing_angle(0.0f),
        _cooldown(0.0f),
        _index(Utils::max_uint)
    {

    }
    Enemy::Enemy(const Enemy &clone) :
        Character(clone),
        _enemy_type(clone._enemy_type),
        _bullet_count(0u),
        _firing_angle(0.0f),
        _cooldown(0.0f),
        _index(Utils::max_uint)
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
                    auto bullet = gs->bullet_manager().spawn_pattern_bullet("straight_red_circle", Group::ENEMY);
                    if (bullet == nullptr)
                    {
                        continue;
                    }
                    auto trans = forward_center_trans();
                    trans.rotate(i * 22.5f);
                    bullet->init_transform(trans);
                }
                _cooldown = 0.5f;
            }
            _cooldown -= dt;

            //auto rot_speed = 45.0f * dt;
            //rotateBy(rot_speed);
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
    
    void Enemy::index(uint32_t value)
    {
        _index = value;
    }
    uint32_t Enemy::index() const
    {
        return _index;
    }
    // }}}

    // EnemyManager {{{
    EnemyManager::EnemyManager() :
        _enemy_counter(1u),
        _num_enemies(0u)
    {

    }

    void EnemyManager::remove_enemy(Enemy *enemy)
    {
        _dead_enemies.push_back(std::move(_objects[enemy->index()]));
        _num_enemies--;
    }

    Enemy *EnemyManager::spawn_enemy(const std::string &name)
    {
        auto find = _src_enemies.find(name);
        if (find == _src_enemies.end())
        {
            return nullptr;
        }

        auto index = find_empty_object();
        if (index == Utils::max_uint)
        {
            return nullptr;
        }

        auto new_enemy = new Enemy(*find->second.get());
        new_enemy->id(++_enemy_counter);
        insert_object(new_enemy, index);
        _num_enemies++;
        return new_enemy;
    }

    uint32_t EnemyManager::num_enemies() const
    {
        return _num_enemies;
    }

    const EnemyManager::EnemyMap &EnemyManager::src_enemies() const
    {
        return _src_enemies;
    }

    void EnemyManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        for (auto i = 0u; i < _objects.size(); i++)
        {
            auto enemy = _objects[i].get();
            if (enemy != nullptr)
            {
                target.draw(*enemy, states);
                //target.draw(enemy->hitbox(), states);
            }
        }
    }

    void EnemyManager::add_src_enemy(const std::string &name, Enemy *enemy)
    {
        _src_enemies[name] = std::unique_ptr<Enemy>(enemy);
    }
    // }}}
}
