#include "level_select.h"

#include <iostream>

#include "button.h"
#include "manager.h"
#include "main_menu.h"

#include <vot/texture_manager.h>
#include <vot/game_system.h>
#include <vot/levels/level.h>

namespace vot
{
    namespace ui
    {
        MenuHelper LevelSelect::s_helper;

        bool LevelSelect::init()
        {
            auto idle_texture = TextureManager::texture("button_idle");

            auto back = new Button("Back");
            s_helper.back_button(back);
            back->texture(*idle_texture);
            back->setPosition(300, 200);
            back->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                LevelSelect::visible(false);
                MainMenu::visible(true);
                
                return true;
            });

            auto levels = LevelManager::levels();
            auto i = 0u;
            for (auto iter = levels->begin(); iter != levels->end(); ++iter)
            {
                auto btn = new Button(iter->second->name());
                btn->texture(*idle_texture);
                btn->setPosition(300, 260 + (60* i));
                s_helper.add_component(btn);
                i++;

                btn->on_click([iter] (int32_t x, int32_t y, sf::Mouse::Button btn)
                {
                    GameSystem::start_game(iter->first);
                    LevelSelect::visible(false);
                
                    return true;
                });
            }

            s_helper.calc_nearby_components();

            visible(false);
            return true;
        }

        void LevelSelect::deinit()
        {

        }

        void LevelSelect::visible(bool value)
        {
            s_helper.visible(value);
        }
        bool LevelSelect::visible()
        {
            return s_helper.visible();
        }

        void LevelSelect::on_resize( uint32_t width, uint32_t height )
        {
            s_helper.on_resize(width, height);
        }
    }
}
