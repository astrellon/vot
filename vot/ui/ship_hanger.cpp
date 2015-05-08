#include "ship_hanger.h"

#include <iostream>
#include <sstream>

#include <vot/player.h>
#include <vot/game_system.h>
#include <vot/texture_manager.h>
#include <vot/hardpoint.h>

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
        Hardpoint *ShipHanger::s_held_hardpoint = nullptr;
        float ShipHanger::s_scale = 0.5f;

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

            auto player_texture = TextureManager::texture("player");
            s_player_render = new Player(*player_texture);
            s_player_render->sprite().setScale(s_scale, s_scale);
            s_player_render->init();

            s_player_camera.setCenter(0, 0);

            auto placements = s_player_render->placements();
            for (auto iter = placements->cbegin(); iter != placements->cend(); ++iter)
            {
                auto placement = iter->second.get();
                auto widget = new ShipHardpointWidget(placement);
                widget->local_view(&s_player_camera);
                s_helper.add_component(widget, false);
                
                auto pos = placement->position();
                auto size = widget->size();
                widget->setPosition(pos.x - (size.x * s_scale), pos.y - (size.y * s_scale));
                widget->on_click([placement, pos] (int32_t x, int32_t y, sf::Mouse::Button btn)
                {
                    if (s_held_hardpoint != nullptr)
                    {
                        if (placement->hardpoint() == nullptr)
                        {
                            placement->hardpoint(s_held_hardpoint);
                            s_held_hardpoint = nullptr;
                        }
                        else
                        {
                            auto temp = s_held_hardpoint;
                            s_held_hardpoint = placement->hardpoint();
                            placement->hardpoint(temp);
                        }
                    }
                    else
                    {
                        s_held_hardpoint = placement->hardpoint();
                        placement->hardpoint(nullptr);
                    }
                    return true;
                });
            }

            s_helper.calc_nearby_components();

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
                    ProfileManager::current_profile()->apply_to_player(s_player_render);
                }
                else
                {
                    ProfileManager::current_profile()->apply_from_player(s_player_render);
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

            if (s_held_hardpoint != nullptr)
            {
                target.draw(*s_held_hardpoint, states);
            }
        }

        void ShipHanger::on_resize( uint32_t width, uint32_t height )
        {
            s_helper.on_resize(width, height);

            auto half_width = width * s_scale;
            auto half_height = height * s_scale;
            s_player_camera.setSize(sf::Vector2f(half_width, half_height));
        }
        void ShipHanger::on_mouse_move(int32_t x, int32_t y)
        {
            if (!s_helper.visible())
            {
                return;
            }

            if (s_held_hardpoint != nullptr)
            {
                auto window = sf::Vector2i(GameSystem::window_size());
                auto pos = sf::Vector2i(x, y);
                pos.x = pos.x * s_scale - window.x * s_scale * 0.5f;
                pos.y = pos.y * s_scale - window.y * s_scale * 0.5f;
                s_held_hardpoint->setPosition(pos.x, pos.y);
            }
        }
    }
}
