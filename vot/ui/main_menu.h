#pragma once

#include <SFML/Graphics.hpp>

#include <stdint.h>

#include "menu_helper.h"

namespace vot
{
    namespace ui
    {
        class Button;
        class TextInput; 

        class MainMenu 
        {
            public:
                static bool init();
                static void deinit();

                static void visible(bool value);
                static bool visible();

                static void on_resize(uint32_t width, uint32_t height);
                static void draw(sf::RenderTarget &target, sf::RenderStates states);

            private:

                static Button *s_start_game;
                static Button *s_profiles;
                static Button *s_options;
                static Button *s_quit;
                static MenuHelper s_helper;
                static sf::Text s_current_profile;
                static sf::Text s_current_profile_label;

                static void set_pos(sf::Transformable *obj, float x);
        };
    }
}
