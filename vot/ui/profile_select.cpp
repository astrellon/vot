#include "profile_select.h"

#include <iostream>

#include "button.h"
#include "manager.h"
#include "main_menu.h"
#include "text_input.h"

#include <vot/texture_manager.h>
#include <vot/profile.h>
#include <vot/game_system.h>

namespace vot
{
    namespace ui
    {
        MenuHelper ProfileSelect::s_helper;
        TextInput *ProfileSelect::s_name_input = nullptr;

        bool ProfileSelect::init()
        {
            auto idle_texture = TextureManager::texture("button_idle");

            auto back = new Button("Back");
            back->texture(*idle_texture);
            back->setPosition(300, 200);
            s_helper.back_button(back);
            back->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                ProfileSelect::visible(false);
                MainMenu::visible(true);
                
                return true;
            });

            auto top = 260.0f;
            auto profiles = ProfileManager::profiles();
            std::cout << "Num profiles: " << profiles->size() << "\n";
            auto i = 0u;
            for (; i < profiles->size(); i++)
            {
                auto profile = profiles->at(i).get();
                auto btn = new Button(profile->name());
                btn->texture(*idle_texture);
                btn->setPosition(300, top + (60 * i));
                s_helper.add_component(btn);

                btn->on_click([profile] (int32_t x, int32_t y, sf::Mouse::Button btn)
                {
                    ProfileManager::current_profile(profile);  
                    ProfileSelect::visible(false);
                    MainMenu::visible(true);
                    return true;
                });
            }

            i++;
            auto create_button = new Button("Create Profile");
            create_button->texture(*idle_texture);
            create_button->setPosition(300, top + (60 * i));
            s_helper.add_component(create_button);
            create_button->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                auto info = ProfileManager::spawn_profile(s_name_input->value());
                info->save();

                // Super jank, need a better way of redoing the UI.
                s_helper.clear_all_component();
                ProfileSelect::init();
                ProfileSelect::on_resize();
                ProfileSelect::visible(true);

                return true;
            });

            i++;
            s_name_input = new TextInput("Name");
            s_name_input->setPosition(300, top + (60 * i));
            s_helper.add_component(s_name_input);

            s_helper.calc_nearby_components();

            visible(false);

            return true;
        }
        void ProfileSelect::deinit()
        {

        }

        void ProfileSelect::visible(bool value)
        {
            s_helper.visible(value);
        }
        bool ProfileSelect::visible()
        {
            return s_helper.visible();
        }

        void ProfileSelect::on_resize( uint32_t width, uint32_t height )
        {
            s_helper.on_resize(width, height);
        }
        void ProfileSelect::on_resize()
        {
            auto size = GameSystem::window_size();
            on_resize(size.x, size.y);
        }
    }
}
