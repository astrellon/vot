#include "ship_hanger.h"

#include <vot/player.h>
#include <vot/game_system.h>
#include <vot/texture_manager.h>

#include "button.h"
#include "ui_state.h"

namespace vot
{
    namespace ui
    {
        Player *ShipHanger::s_player_render = nullptr;
        MenuHelper ShipHanger::s_helper;

        bool ShipHanger::init()
        {
            auto back = new Button("Back");
            s_helper.back_button(back);
            back->setPosition(300, 50);
            back->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                State::state(State::MAIN_MENU);

                return true;
            });

            auto size = GameSystem::window_size();
            on_resize(size.x, size.y);
            visible(false);

            auto player_texture = TextureManager::texture("player");
            s_player_render = new Player(*player_texture);

            return true;
        }
        void ShipHanger::deinit()
        {
            if (s_player_render != nullptr)
            {
                delete s_player_render;
                s_player_render = nullptr;
            }
        }

        void ShipHanger::visible(bool value)
        {
            auto visible = s_helper.visible(); 
            s_helper.visible(value);

            if (visible != value)
            {
                // If visible set current player data onto the player renderer.
                if (value)
                {
                    apply_player_to_renderer();
                }
            }
        }

        void ShipHanger::update(float dt)
        {

        }
        void ShipHanger::draw(sf::RenderTarget &target, sf::RenderStates states)
        {
            if (s_player_render != nullptr)
            {
                target.draw(*s_player_render, states);
            }
        }

        void ShipHanger::on_resize( uint32_t width, uint32_t height )
        {
            s_helper.on_resize(width, height);            
        }

        void ShipHanger::apply_player_to_renderer()
        {

        }
        void ShipHanger::apply_renderer_to_player()
        {

        }
    }
}
