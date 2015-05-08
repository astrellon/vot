#include "profile_widget.h"

#include <vot/font_manager.h>
#include <vot/profile.h>

#include <vot/utils/utils.h>

#include <sstream>

namespace vot
{
    namespace ui
    {
        bool ProfileWidget::s_visible = false;
        sf::Text ProfileWidget::s_text;

        bool ProfileWidget::init()
        {
            auto sans = FontManager::font("sans");

            s_text.setFont(*sans);
            s_text.setString("");
            s_text.setCharacterSize(16);
            s_text.setColor(sf::Color::White);
            s_text.setPosition(300, 200);
            
            return true;
        }

        void ProfileWidget::visible(bool value)
        {
            s_visible = value;
        }
        bool ProfileWidget::visible()
        {
            return s_visible;
        }

        void ProfileWidget::update(float dt)
        {
            if (!s_visible)
            {
                return;
            }

            auto profile = ProfileManager::current_profile();
            if (profile != nullptr)
            {
                std::stringstream ss;
                ss  << "Current Profile\n" << profile->name() << "\n\n"
                    << "Credits: " << profile->credits() << "\n"
                    << "Points: " << profile->points();

                s_text.setString(ss.str());
            }
            else
            {
                s_text.setString("- No Profile -");
            }
        }
        void ProfileWidget::draw(sf::RenderTarget &target, sf::RenderStates states)
        {
            if (!s_visible)
            {
                return;
            }

            target.draw(s_text, states);
        }

        void ProfileWidget::on_resize(uint32_t width, uint32_t height)
        {
            auto size = 256.0f;
            auto x = utils::Utils::round((static_cast<float>(width) - size) * 0.5f);
            auto offset = utils::Utils::round(1.1f * size);

            s_text.setPosition(x + offset, s_text.getPosition().y);
        }
    }
}

