#pragma once

#include <SFML/Graphics.hpp>

namespace vot
{
    class MainMenu : public sf::Drawable
    {
        public:
            MainMenu();

            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:

    };
}
