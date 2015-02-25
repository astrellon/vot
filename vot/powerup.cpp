#include "powerup.h"

#include "game_system.h"

namespace vot
{
    // Powerup {{{
    Powerup::Powerup(const sf::Texture &texture, Type type, int32_t value) :
        sf::Sprite(texture),
        _type(type),
        _value(value),
        _hitbox(5.0f)
    {
        auto size = texture.getSize();
        setOrigin(size.x * 0.5f, size.y * 0.5f);
    }
    Powerup::Powerup(const Powerup &clone) :
        sf::Sprite(clone),
        _type(clone._type),
        _value(clone._value),
        _hitbox(clone._hitbox.radius())
    {
        auto size = getTexture()->getSize();
        setOrigin(size.x * 0.5f, size.y * 0.5f);
    }

    Powerup::Type Powerup::type() const
    {
        return _type;
    }
    int32_t Powerup::value() const
    {
        return _value;
    }

    void Powerup::location(const sf::Vector2f &location)
    {
        setPosition(location);
        _hitbox.location(location);
    }
    void Powerup::init_location(const sf::Vector2f &location)
    {
        _init_location = location;
    }

    Circle &Powerup::hitbox()
    {
        return _hitbox;
    }
    const Circle &Powerup::hitbox() const
    {
        return _hitbox;
    }

    void Powerup::update(float dt)
    {
        _lifetime += dt;

        auto view = GameSystem::main()->camera();
        auto view_angle = view.getRotation(); 
        setRotation(view_angle);
    }
    // }}}
    
    // PowerupManager {{{
    Powerup *PowerupManager::spawn_powerup(const std::string &name)
    {
        auto find = _src_powerups.find(name);
        if (find == _src_powerups.end())
        {
            return nullptr;
        }

        auto new_powerup = new Powerup(*find->second.get());
        _powerups.push_back(std::unique_ptr<Powerup>(new_powerup));
        return new_powerup;
    }
    void PowerupManager::add_src_powerup(const std::string &name, Powerup *powerup)
    {
        _src_powerups[name] = std::unique_ptr<Powerup>(powerup);
    }
    void PowerupManager::remove_powerup(Powerup *powerup)
    {
        if (powerup == nullptr)
        {
            return;
        }

        for (auto i = 0u; i < _powerups.size(); i++)
        {
            if (_powerups[i].get() == powerup)
            {
                _powerups.erase(_powerups.begin() + i);
                break;
            }
        }
    }

    const PowerupManager::PowerupList *PowerupManager::active_powerups() const
    {
        return &_powerups;
    }

    void PowerupManager::update(float dt)
    {
        for (auto i = 0u; i < _powerups.size(); i++)
        {
            _powerups[i]->update(dt);
        }
    }

    void PowerupManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        for (auto i = 0u; i < _powerups.size(); i++)
        {
            target.draw(*_powerups[i].get(), states);
        }
    }
    // }}}
}
