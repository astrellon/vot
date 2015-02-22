#include "bullet.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "utils.h"
#include "character.h"

namespace vot
{
    #define _UMAX std::numeric_limits<uint32_t>::max()

    // Bullet {{{
    Bullet::Bullet(const sf::Texture &texture, float damage) :
        sf::Sprite(texture),
        _damage(damage),
        _index(_UMAX),
        _owner(0u),
        _group(NATURE)
    {
        auto size = texture.getSize();
        setOrigin(size.x * 0.5f, size.y * 0.5f);
    }
    Bullet::Bullet(const Bullet &clone) :
        sf::Sprite(clone),
        _damage(clone._damage),
        _index(_UMAX),
        _hitbox(clone._hitbox.radius()),
        _owner(0u),
        _group(NATURE)
    {
        auto size = getTexture()->getSize();
        setOrigin(size.x * 0.5f, size.y * 0.5f);
    }

    sf::Vector2f Bullet::center() const
    {
        auto pos = getPosition();
        auto size = getTexture()->getSize();
        return sf::Vector2f(pos.x - size.x * 0.5f, pos.y - size.y * 0.5f);
    }

    void Bullet::damage(float value)
    {
        _damage = value;
    }
    float Bullet::damage() const
    {
        return _damage;
    }

    void Bullet::index(uint32_t value)
    {
        _index = value;
    }
    uint32_t Bullet::index() const
    {
        return _index;
    }

    void Bullet::owner(uint16_t value)
    {
        _owner = value;
    }
    uint16_t Bullet::owner() const
    {
        return _owner;
    }

    void Bullet::group(Bullet::Group value)
    {
        _group = value;
    }
    Bullet::Group Bullet::group() const
    {
        return _group;
    }

    Circle &Bullet::hitbox()
    {
        return _hitbox;
    }
    // }}}
    
    // PatternBullet {{{
    PatternBullet::PatternBullet(const sf::Texture &texture, float damage) :
        Bullet(texture, damage),
        _lifetime(0.0f),
        _total_lifetime(3.0f),
        _pattern_type(0u)
    {
    }
    PatternBullet::PatternBullet(const PatternBullet &clone) :
        Bullet(clone),
        _lifetime(clone._lifetime),
        _total_lifetime(clone._total_lifetime),
        _pattern_type(clone._pattern_type)
    {

    }

    bool PatternBullet::active() const
    {
        return _lifetime >= 0.0f;
    }
    bool PatternBullet::dead() const
    {
        return _lifetime > _total_lifetime;
    }

    
    void PatternBullet::init_transform(sf::Transform trans)
    {
        _init_transform = trans;
    }
    const sf::Transform &PatternBullet::init_transform() const
    {
        return _init_transform;
    }

    void PatternBullet::pattern_type(uint32_t type)
    {
        _pattern_type = type;
    }
    uint32_t PatternBullet::pattern_type() const
    {
        return _pattern_type;
    }

    void PatternBullet::update(float dt)
    {
        _lifetime += dt;

        if (_lifetime < 0.0f)
        {
            return;
        }

        auto dl = _lifetime / _total_lifetime;
        auto speed_dl = dl * 0.5f;
        auto point = getPosition();
        auto prevPoint = getPosition();

        if (_pattern_type == 0u)
        {
            auto x = dl * 1000.0f;
            point = _init_transform.transformPoint(x, 0.0f);
        }
        else if (_pattern_type == 2u)
        {
            auto x = dl * 2000.0f;
            point = _init_transform.transformPoint(x, 0.0f);
        }
        else if (_pattern_type == 1u)
        {
            auto x = cosf(speed_dl * 7.0f) * sinf(speed_dl * 3.0f) * 300.0f + 320.0f;
            auto y = sinf(speed_dl * 2.0f) * cosf(speed_dl * 13.0f) * 300.0f + 320.0f;
            if (dl > 1.0f)
            {
                _lifetime -= _total_lifetime;
            }

            point = _init_transform.transformPoint(x, y);
        }

        auto dp = point - prevPoint;
        auto angle = atan2(dp.y, dp.x);

        setPosition(point);

        hitbox().location(point);
        setRotation(angle * 180.0f / M_PI + 90.0f);
    }
    // }}}
    
    // HomingBullet {{{
    HomingBullet::HomingBullet(const sf::Texture &texture, float damage) :
        Bullet(texture, damage),
        _target(nullptr),
        _lifetime(0.0f),
        _total_lifetime(3.0f),
        _tracking_time(0.0f)
    {

    }
    HomingBullet::HomingBullet(const HomingBullet &clone) :
        Bullet(clone),
        _target(nullptr),
        _lifetime(0.0f),
        _total_lifetime(clone._total_lifetime),
        _tracking_time(0.0f)
    {

    }

    void HomingBullet::target(const Character *value)
    {
        _target = value;
    }
    const Character *HomingBullet::target() const
    {
        return _target;
    }

    bool HomingBullet::active() const
    {
        return _lifetime >= 0.0f;
    }
    bool HomingBullet::dead() const
    {
        return _lifetime > _total_lifetime;
    }

    void HomingBullet::update(float dt)
    {
        _lifetime += dt;
        auto speed = 90.0f * dt;
        if (_tracking_time >= 1.0f)
        {
            auto top_speed = 1000.0f;
            auto base_speed = 100.0f;
            speed = (_tracking_time - 1.0f) * (top_speed - base_speed) + base_speed;
            speed = speed > top_speed ? top_speed : speed;
            speed *= dt;
        }
        auto matrix = getTransform().getMatrix();
        auto x = speed * matrix[0];
        auto y = -speed * matrix[4];
        move(x, y);
        hitbox().location(getPosition());
        
        if (_target == nullptr || _target->is_dead())
        {
            return;
        }
        _tracking_time += dt;
        if (_tracking_time < 1.0f)
        {
            return;
        }
        
        auto angles = Utils::calculate_angles(getPosition(), _target->location(), getRotation(), 0.0f);
        auto rot_speed = 180.0f * dt;

        if (angles.delta_angle() < rot_speed && angles.delta_angle() > -rot_speed)
        {
            setRotation(angles.to_angle());
        }
        else
        {
            rotate(angles.delta_angle() > 0 ? rot_speed : -rot_speed);
        }
    }
    // }}}

    // BulletManager {{{
    BulletManager::BulletManager() :
        _bullet_index(0u)
    {

    }

    void BulletManager::remove_bullet(Bullet *bullet)
    {
        _bullets[bullet->index()] = nullptr;
    }
    PatternBullet *BulletManager::spawn_pattern_bullet(const std::string &name, uint16_t owner, Bullet::Group group)
    {
        auto find = _src_pattern_bullets.find(name);
        if (find == _src_pattern_bullets.end())
        {
            return nullptr;
        }

        auto index = find_empty_bullet();
        if (index == _UMAX)
        {
            return nullptr;
        }

        auto new_bullet = new PatternBullet(*find->second.get());
        new_bullet->owner(owner);
        new_bullet->group(group);
        insert_bullet(new_bullet, index);
        return new_bullet;
    }
    HomingBullet *BulletManager::spawn_homing_bullet(const std::string &name, uint16_t owner, Bullet::Group group)
    {
        auto find = _src_homing_bullets.find(name);
        if (find == _src_homing_bullets.end())
        {
            return nullptr;
        }

        auto index = find_empty_bullet();
        if (index == _UMAX)
        {
            return nullptr;
        }

        auto new_bullet = new HomingBullet(*find->second.get());
        new_bullet->owner(owner);
        new_bullet->group(group);
        insert_bullet(new_bullet, index);
        return new_bullet;
    }

    void BulletManager::insert_bullet(Bullet *bullet, uint32_t index)
    {
        bullet->index(index);
        _bullets[index] = std::unique_ptr<Bullet>(bullet);;
    }

    void BulletManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        for (auto i = 0u; i < _bullets.size(); i++)
        {
            auto bullet = _bullets[i].get(); 
            if (bullet != nullptr && bullet->active())
            {
                target.draw(*bullet, states);
                //target.draw(bullet->hitbox(), states);
            }
        }
    }

    void BulletManager::add_src_pattern_bullet(PatternBullet *bullet, const std::string &name)
    {
        _src_pattern_bullets[name] = std::unique_ptr<PatternBullet>(bullet);
    }
    void BulletManager::add_src_homing_bullet(HomingBullet *bullet, const std::string &name)
    {
        _src_homing_bullets[name] = std::unique_ptr<HomingBullet>(bullet);
    }

    BulletManager::BulletList *BulletManager::bullets()
    {
        return &_bullets;
    }

    uint32_t BulletManager::find_empty_bullet() const
    {
        auto start = _bullet_index;
        auto index = _bullet_index + 1;
        while (start != index)
        {
            if (_bullets[index].get() == nullptr)
            {
                return index;
            }

            index++;
            if (index >= _bullets.size())
            {
                index = 0;
            }
        }

        return _UMAX;
    }
    // }}}
}

