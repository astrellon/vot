#include "background.h"

#include <iostream>
#include <random>
#include <math.h>

#include "texture_manager.h"
#include "game_system.h"
#include "utils/utils.h"

namespace vot
{
    Background::Background() :
        sf::Drawable(),
        sf::Transformable(),
        _speed(1.0f)
    {
    }

    void Background::create()
    {
        auto size = 512.0f;
        _target.create(static_cast<uint32_t>(size), static_cast<uint32_t>(size));
        _target.clear();

        auto blue_star = TextureManager::texture("blue_star");
        sf::Sprite blue_star_sprite(*blue_star);
        auto s = blue_star->getSize();
        blue_star_sprite.setOrigin(s.x * 0.5f, s.y * 0.5f);

        auto red_star = TextureManager::texture("red_star");
        s = red_star->getSize();
        sf::Sprite red_star_sprite(*red_star);
        red_star_sprite.setOrigin(s.x * 0.5f, s.y * 0.5f);

        auto sprite = &blue_star_sprite;
        for (auto i = 0; i < 40; i++)
        {
            if (i < 20)
            {
                sprite = &blue_star_sprite;
            }
            else
            {
                sprite = &red_star_sprite;
            }
            auto x = size * (utils::Utils::randf() * 0.95f + 0.025f);
            auto y = size * (utils::Utils::randf() * 0.95f + 0.025f);

            auto r = static_cast<uint8_t>(utils::Utils::randf(0.0, 30.0f) + 155);
            auto g = static_cast<uint8_t>(utils::Utils::randf(0.0, 30.0f) + 155);
            auto b = static_cast<uint8_t>(utils::Utils::randf(0.0, 30.0f) + 155);
            sf::Color colour(r, g, b);

            auto scale = (powf(2.0f, utils::Utils::randf()) - 1.0f) * (powf(2.0f, (_speed + 0.5f) - 1.0f));
            sprite->setScale(scale, scale);
            sprite->setPosition(x, y);
            sprite->setColor(colour);
            _target.draw(*sprite);
        }

        _target.setRepeated(true);
        _target.setSmooth(true);
        _sprite.setTexture(_target.getTexture());
        _sprite.setTextureRect(sf::IntRect(0, 0, 2048, 2048));
    }

    void Background::speed(float value)
    {
        _speed = value;
    }
    float Background::speed() const
    {
        return _speed;
    }

    void Background::update(float dt)
    {
        auto camera = GameSystem::game()->camera();
        auto cam_pos = camera.getCenter();
        auto pos = cam_pos;
        pos.x *= -_speed;
        pos.y *= -_speed;

        auto int_pos = sf::Vector2i(pos);
        auto sizef = _sprite.getTexture()->getSize();
        auto size = sf::Vector2i(sizef);

        auto text_rect = _sprite.getTextureRect();
        auto x = int_pos.x % size.x + cam_pos.x - text_rect.width * 0.5f;
        auto y = int_pos.y % size.y + cam_pos.y - text_rect.height * 0.5f;
        _sprite.setPosition(x, y); 
    }

    void Background::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(_sprite, states);
    }
}
