#pragma once

#include <SFML/Graphics.hpp>

#include <stdint.h>

namespace vot
{
    namespace ui
    {
        class Button;

        class MainMenu 
        {
            public:
                static bool init();
                static void deinit();

                static void visible(bool value);
                static bool visible();

                static void on_resize(uint32_t width, uint32_t height);

            private:

                static bool s_visible;
                static Button *s_start_game;
                static Button *s_options;
                static Button *s_quit;
        };
    }
}
