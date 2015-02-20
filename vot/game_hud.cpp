#include "game_hud.h"

#include "game_system.h"
#include "font_manager.h"
#include "texture_manager.h"

#include <sstream>

namespace vot
{
    // HudMain {{{
    HudMain::HudMain()
    {
    }

    void HudMain::create()
    {
        _sans = FontManager::main()->font("sans");
        _health.setFont(*_sans);
        _health.setCharacterSize(13);
    }

    void HudMain::update(float dt)
    {
        auto gs = GameSystem::main();

        std::stringstream health;
        health << "Health: " << gs->player()->life();
        if (gs->player()->looking_at_target())
        {
            health << "\nLooking at target";
        }
        if (gs->player()->auto_target())
        {
            health << "\nAuto targetting";
        }
        auto target = gs->player()->target();
        if (target != nullptr)
        {
            health << "\n\nTarget: " << target->life() << "/" << target->max_life();
        }
        _health.setString(health.str());
    }

    void HudMain::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(_health, states);
    }
    // }}}
    
    // HudWorld {{{
    HudWorld::HudWorld()
    {
    }

    void HudWorld::create()
    {
        _sans = FontManager::main()->font("sans");

        auto target_texture = TextureManager::main()->texture("target");
        _target.setTexture(*target_texture);
        auto size = target_texture->getSize();
        _target.setOrigin(size.x * 0.5, size.y * 0.5);
    }

    void HudWorld::update(float dt)
    {
        auto gs = GameSystem::main();
        auto player = gs->player();
        _has_target = false;

        if (player != nullptr)
        {
            auto target = player->target();
            if (target != nullptr)
            {
                auto pos = target->location();
                _target.setPosition(pos.x, pos.y);
                _target.setRotation(gs->camera().getRotation());
                _has_target = true;
            }
        }
    }

    void HudWorld::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (_has_target)
        {
            target.draw(_target, states);
        }
    }
    // }}}
}
