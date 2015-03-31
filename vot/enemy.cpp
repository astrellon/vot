#include "enemy.h"

#include "game_system.h"
#include "utils/utils.h"
#include "common.h"
#include "icontroller.h"

namespace vot
{
    // Enemy {{{
    Enemy::Enemy(const sf::Texture &texture) :
        Character(texture),
        _index(utils::Utils::max_uint),
        _controller(nullptr)
    {

    }
    Enemy::Enemy(const Enemy &clone) :
        Character(clone),
        _index(utils::Utils::max_uint),
        _controller(nullptr)
    {

    }

    void Enemy::controller(IController *controller)
    {
        _controller = std::unique_ptr<IController>(controller);
    }
    IController *Enemy::controller() const
    {
        return _controller.get();
    }

    void Enemy::update(float dt)
    {
        if (_controller != nullptr)
        {
            _controller->update(dt);
        }

        Character::update(dt);
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
        if (index == utils::Utils::max_uint)
        {
            return nullptr;
        }

        auto new_enemy = new Enemy(*find->second.get());
        new_enemy->id(++_enemy_counter);
        insert_object(new_enemy, index);
        _num_enemies++;
        return new_enemy;
    }

    void EnemyManager::clear_enemies()
    {
        for (auto i = 0u; i < _objects.size(); i++)
        {
            _objects[i] = nullptr;
        }
        _num_enemies = 0u;
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
