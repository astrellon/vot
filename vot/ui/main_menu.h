#pragma once

#include <SFML/Graphics.hpp>

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

            private:

                static bool s_visible;
                static Button *s_start_game;
                static Button *s_options;
                static Button *s_quit;
        };
    }
}
