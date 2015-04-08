#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include <stdint.h>

#include "menu_helper.h"

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

                static MenuHelper s_helper;
        };
    }
}
