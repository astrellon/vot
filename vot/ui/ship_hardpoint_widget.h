#pragma once

#include <SFML/Graphics.hpp>

#include "component.h"

namespace vot
{
    class HardpointPlacement;
    namespace ui
    {
        class ShipHardpointWidget : public Component
        {
            public:
                ShipHardpointWidget(HardpointPlacement *placement);

                HardpointPlacement *placement() const;

                virtual void update(float dt);
                virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

                virtual sf::Vector2f size() const;

            private:
                HardpointPlacement *_placement;
        };
    }
}
