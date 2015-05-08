#pragma once

#include <SFML/Graphics.hpp>

#include "component.h"

namespace vot
{
    class Hardpoint;

    namespace ui
    {
        class HardpointWidget : public Component
        {
            public:
                HardpointWidget(Hardpoint *point);

                void texture(const sf::Texture &texture);
                const sf::Texture *texture() const;

                void hardpoint(Hardpoint *point);
                Hardpoint *hardpoint() const;

                virtual void update(float dt);
                virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

                virtual sf::Vector2f size() const;

            private:
                sf::Sprite _background;
                Hardpoint *_hardpoint;

        };
    }
}
