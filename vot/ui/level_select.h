#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include <stdint.h>

namespace vot
{
    namespace ui
    {
        class Button;

        class LevelSelect
        {
            public:
                static bool init();
                static void deinit();

                static void visible(bool value);
                static bool visible();

                static void on_resize(uint32_t width, uint32_t height);

            private:

                static bool s_visible;
                static std::vector<Button *> s_level_buttons;
                static Button *s_back_button;
        };
    }
}
