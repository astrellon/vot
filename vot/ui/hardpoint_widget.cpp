#include "hardpoint_widget.h"

#include <vot/hardpoint.h>

namespace vot
{
    namespace ui
    {
        HardpointWidget::HardpointWidget(Hardpoint *point) :
            _hardpoint(point)
        {

        }

        void HardpointWidget::texture(const sf::Texture &texture)
        {
            _background.setTexture(texture);
        }
        const sf::Texture *HardpointWidget::texture() const
        {
            return _background.getTexture();
        }

        void HardpointWidget::hardpoint(Hardpoint *point)
        {
            _hardpoint = point;
        }
        Hardpoint *HardpointWidget::hardpoint() const
        {
            return _hardpoint;
        }

        void HardpointWidget::update(float dt)
        {
        }
        void HardpointWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const
        {
        }

        sf::Vector2f HardpointWidget::size() const
        {
            return sf::Vector2f(32, 32);
        }
    }
}
