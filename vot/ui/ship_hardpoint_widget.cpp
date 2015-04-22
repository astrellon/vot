#include "ship_hardpoint_widget.h"

#include <vot/hardpoint.h>

namespace vot
{
    namespace ui
    {
        ShipHardpointWidget::ShipHardpointWidget(HardpointPlacement *placement) :
            _placement(placement)
        {

        }

        HardpointPlacement *ShipHardpointWidget::placement() const
        {
            return _placement;
        }

        void ShipHardpointWidget::update(float dt)
        {

        }
        void ShipHardpointWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const
        {
            sf::RectangleShape shape;
            shape.setSize(sf::Vector2f(6, 6));
            shape.setOutlineColor(sf::Color::Red);
            shape.setFillColor(sf::Color::Transparent);
            if (has_focus())
            {
                shape.setFillColor(sf::Color::Red);
            }
            shape.setOutlineThickness(1);

            states.transform *= getTransform();
            target.draw(shape, states);
        }
                
        bool ShipHardpointWidget::do_click(int32_t x, int32_t y, sf::Mouse::Button button)
        {
            return false;
        }

        sf::Vector2f ShipHardpointWidget::size() const
        {
            return sf::Vector2f(6, 6);
        }
    }
}
