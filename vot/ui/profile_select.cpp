#include "profile_select.h"

#include <iostream>

#include "button.h"
#include "manager.h"
#include "main_menu.h"

#include <vot/texture_manager.h>
#include <vot/player_info.h>

namespace vot
{
    namespace ui
    {
        MenuHelper ProfileSelect::s_helper;

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
            });

            auto profiles = PlayerInfoManager::player_infos();
            std::cout << "Num profiles: " << profiles->size() << "\n";
            for (auto i = 0u; i < profiles->size(); i++)
            {
                auto profile = profiles->at(i).get();
                auto btn = new Button(profile->name());
                btn->texture(*idle_texture);
                btn->setPosition(300, 260 + (60 * i));
                s_helper.add_component(btn);
            }

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
    }
}
