#include "game_hud.h"

#include "game_system.h"
#include "font_manager.h"
#include "texture_manager.h"

#include <iostream>
#include <sstream>

namespace vot
{
    // HudMain {{{
    HudMain::HudMain()
    {
    }

    void HudMain::create()
    {
        _sans = FontManager::font("sans");
        _top_left.setFont(*_sans);
        _top_left.setCharacterSize(13);
        _top_right.setFont(*_sans);
        _top_right.setCharacterSize(13);
    }

    void HudMain::update(float dt)
    {
        auto gs = GameSystem::game();

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

        health << "\n\nTime scale: x" << gs->time_scale() << "\n";

        _top_left.setString(health.str());

        std::stringstream points;
        points << "Points: " << ProfileManager::current_profile()->points() << "\n";
        _top_right.setString(points.str());
        auto window_size = GameSystem::window_size();
        auto bounds = _top_right.getLocalBounds();
        _top_right.setPosition(window_size.x - bounds.width, 0);
    }

    void HudMain::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(_top_left, states);
        target.draw(_top_right, states);
    }
    // }}}
    
    // HudWorld {{{
    HudWorld::HudWorld()
    {
    }

    void HudWorld::create()
    {
        _sans = FontManager::font("sans");

        auto target_texture = TextureManager::texture("target");
        _target.setTexture(*target_texture);
        auto size = target_texture->getSize();
        _target.setOrigin(static_cast<float>(size.x) * 0.5f, static_cast<float>(size.y) * 0.5f);
    }

    void HudWorld::update(float dt)
    {
        auto gs = GameSystem::game();
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
