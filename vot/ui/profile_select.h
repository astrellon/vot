#pragma once

#include <stdint.h>
#include <vector>

#include "menu_helper.h"

namespace vot
{
    namespace ui
    {
        class Button;

        class ProfileSelect
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
