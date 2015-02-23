#include "enemy.h"

#include "game_system.h"

namespace vot
{
    #define _UMAX std::numeric_limits<uint32_t>::max()
    // Enemy {{{
    Enemy::Enemy(const sf::Texture &texture) :
        Character(texture),
        _enemy_type(0u),
        _bullet_count(0u),
        _firing_angle(0.0f),
        _cooldown(0.0f),
        _index(_UMAX)
    {

    }
    Enemy::Enemy(const Enemy &clone) :
        Character(clone),
        _enemy_type(clone._enemy_type),
        _bullet_count(0u),
        _firing_angle(0.0f),
        _cooldown(0.0f),
        _index(_UMAX)
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
                    auto bullet = gs->bullet_manager().spawn_pattern_bullet("straight_red_circle", id(), Bullet::ENEMY);
                    if (bullet == nullptr)
                    {
                        continue;
                    }
                    auto trans = forward_center_trans();
                    trans.rotate(i * 22.5);
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
        _enemy_index(0u),
        _enemy_counter(1u),
        _num_enemies(0u)
    {

    }

    void EnemyManager::remove_enemy(Enemy *enemy)
    {
        _dead_enemies.push_back(std::move(_enemies[enemy->index()]));
        _num_enemies--;
    }

    Enemy *EnemyManager::spawn_enemy(const std::string &name)
    {
        auto find = _src_enemies.find(name);
        if (find == _src_enemies.end())
        {
            return nullptr;
        }

        auto index = find_empty_enemy();
        if (index == _UMAX)
        {
            return nullptr;
        }

        auto new_enemy = new Enemy(*find->second.get());
        new_enemy->id(++_enemy_counter);
        insert_enemy(new_enemy, index);
        return new_enemy;
    }

    uint32_t EnemyManager::num_enemies() const
    {
        return _num_enemies;
    }

    EnemyManager::EnemyList *EnemyManager::enemies()
    {
        return &_enemies;
    }
    const EnemyManager::EnemyList *EnemyManager::enemies() const
    {
        return &_enemies;
    }
    const EnemyManager::EnemyMap &EnemyManager::src_enemies() const
    {
        return _src_enemies;
    }

    void EnemyManager::insert_enemy(Enemy *enemy, uint32_t index)
    {
        enemy->index(index);
        _enemies[index] = std::unique_ptr<Enemy>(enemy);
        _num_enemies++;
    }
    uint32_t EnemyManager::find_empty_enemy() const
    {
        auto start = _enemy_index;
        auto index = _enemy_index + 1;
        while (start != index)
        {
            if (_enemies[index].get() == nullptr)
            {
                return index;
            }

            index++;
            if (index >= _enemies.size())
            {
                index = 0;
            }
        }

        return _UMAX;
    }

    void EnemyManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        for (auto i = 0u; i < _enemies.size(); i++)
        {
            auto enemy = _enemies[i].get();
            if (enemy != nullptr)
            {
                target.draw(*enemy, states);
                //target.draw(enemy->hitbox(), states);
            }
        }
    }

    void EnemyManager::add_src_enemy(Enemy *enemy, const std::string &name)
    {
        _src_enemies[name] = std::unique_ptr<Enemy>(enemy);
    }
    // }}}
}
