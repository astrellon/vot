#include "player.h"

#include "game_system.h"
#include "bullet.h"

namespace vot
{
    Player::Player(const sf::Texture &texture) :
        Character(texture),
        _cooldown(0.0f)
    {

    }

    void Player::update(float dt)
    {
        auto speed = 270.0f * dt;
        auto rot_speed = 180.0f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            translate(sf::Vector2f(-speed, 0.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            translate(sf::Vector2f(speed, 0.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            translate(sf::Vector2f(0.0f, speed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            translate(sf::Vector2f(0.0f, -speed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            rotateBy(-rot_speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            rotateBy(rot_speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && _cooldown <= 0.0f)
        {
            auto bullet = spawn_bullet();
            bullet->pattern_type(2u);
            auto trans = forward_center_trans();
            bullet->init_transform(trans);
            
            bullet = spawn_bullet();
            bullet->pattern_type(2u);
            trans = forward_center_trans();
            trans.rotate(10.0f);
            bullet->init_transform(trans);

            bullet = spawn_bullet();
            bullet->pattern_type(2u);
            trans = forward_center_trans();
            trans.rotate(-10.0f);
            bullet->init_transform(trans);

            _cooldown = 0.1f;
        }
        _cooldown -= dt;
    }

    PatternBullet *Player::spawn_bullet()
    {
        return GameSystem::main()->bullet_manager().spawn_pattern_bullet("straight_blue", id(), Bullet::PLAYER);
    }

}
