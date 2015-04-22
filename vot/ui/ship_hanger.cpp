#include "ship_hanger.h"

#include <iostream>

#include <vot/player.h>
#include <vot/game_system.h>
#include <vot/texture_manager.h>

#include "button.h"
#include "ui_state.h"
#include "ship_hardpoint_widget.h"
#include "manager.h"

namespace vot
{
    namespace ui
    {
        Player *ShipHanger::s_player_render = nullptr;
        MenuHelper ShipHanger::s_helper;
        sf::View ShipHanger::s_player_camera;

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

            auto continue_btn = new Button("Continue");
            s_helper.add_component(continue_btn);
            continue_btn->setPosition(300, 110);
            continue_btn->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                State::state(State::LEVEL_SELECT);

                return true;
            });

            s_helper.calc_nearby_components();

            auto player_texture = TextureManager::texture("player");
            s_player_render = new Player(*player_texture);
            s_player_render->sprite().setScale(0.5f, 0.5f);
            s_player_render->init();

            s_player_camera.setCenter(0, 0);

            auto placements = s_player_render->hardpoint_placements();
            for (auto i = 0u; i < placements->size(); i++)
            {
                auto placement = placements->at(i).get();
                auto widget = new ShipHardpointWidget(placement);
                widget->local_view(&s_player_camera);
                s_helper.add_component(widget, false);

                auto pos = placement->position();
                auto size = widget->size();
                widget->setPosition(pos.x - (size.x * 0.5f), pos.y - (size.y * 0.5f));
            }

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
            if (!s_helper.visible())
            {
                return;
            }

            if (s_player_render != nullptr)
            {
                target.setView(s_player_camera);

                target.draw(*s_player_render, states);
            }
        }

        void ShipHanger::on_resize( uint32_t width, uint32_t height )
        {
            s_helper.on_resize(width, height);

            auto half_width = width * 0.5f;
            auto half_height = height * 0.5f;
            s_player_camera.setSize(sf::Vector2f(half_width, half_height));
        }

        void ShipHanger::apply_player_to_renderer()
        {

        }
        void ShipHanger::apply_renderer_to_player()
        {

        }
    }
}
