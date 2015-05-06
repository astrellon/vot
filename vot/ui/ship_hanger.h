#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>

#include "menu_helper.h"

namespace vot
{
    class Player;
    class Hardpoint;

    namespace ui
    {
        class Button;

        class ShipHanger
        {
            public:
                static bool init();
                static void deinit();

                static void visible(bool value);
                static bool visible();

                static void update(float dt);
                static void draw(sf::RenderTarget &target, sf::RenderStates states);

                static void on_resize(uint32_t width, uint32_t height);

            private:
                static Player *s_player_render; 
                static MenuHelper s_helper;
                static sf::View s_player_camera;
                static Hardpoint *s_held_hardpoint;

                static void apply_player_to_renderer();
                static void apply_renderer_to_player();

                static void create_inventory_buttons();

                static std::vector<Button *> s_inventory_buttons;
        };
    }
}
