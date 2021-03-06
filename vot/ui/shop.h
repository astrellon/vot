#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>

namespace vot
{
    namespace ui
    {
        class Shop
        {
            public:
                static bool init();

                static void visible(bool value);
                static bool visible();

                static void update(float dt);
                static void draw(sf::RenderTarget &target, sf::RenderStates states);

                static void on_resize(uint32_t width, uint32_t height);

            private:

        };
    }
}
