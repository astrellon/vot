#include "profile_widget.h"

#include <vot/font_manager.h>
#include <vot/profile.h>

#include <vot/utils/utils.h>

namespace vot
{
    namespace ui
    {
        bool ProfileWidget::s_visible = false;
        sf::Text ProfileWidget::s_label;
        sf::Text ProfileWidget::s_profile_name;
        vot::Profile *ProfileWidget::s_profile = nullptr;

        bool ProfileWidget::init()
        {
            auto sans = FontManager::font("sans");

            s_label.setFont(*sans);
            s_label.setString("Current Profile");
            s_label.setCharacterSize(16);
            s_label.setColor(sf::Color::White);
            s_label.setPosition(300, 200);

            s_profile_name.setFont(*sans);
            s_profile_name.setString("<No Profile>");
            s_profile_name.setCharacterSize(16);
            s_profile_name.setColor(sf::Color::White);
            s_profile_name.setPosition(300, 230);

            return true;
        }
        void ProfileWidget::deinit()
        {

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
            auto profile = ProfileManager::current_profile();
            if (profile != s_profile)
            {
                s_profile = profile;
                if (profile != nullptr)
                {
                    s_profile_name.setString(profile->name());
                }
                else
                {
                    s_profile_name.setString("- No Profile -");
                }
            }
        }
        void ProfileWidget::draw(sf::RenderTarget &target, sf::RenderStates states)
        {
            if (!s_visible)
            {
                return;
            }

            target.draw(s_label, states);
            target.draw(s_profile_name, states);
        }

        void ProfileWidget::on_resize(uint32_t width, uint32_t height)
        {
            auto size = 256.0f;
            auto x = utils::Utils::round((static_cast<float>(width) - size) * 0.5f);
            auto offset = utils::Utils::round(1.1f * size);

            s_label.setPosition(x + offset, s_label.getPosition().y);
            s_profile_name.setPosition(x + offset, s_profile_name.getPosition().y);
        }
    }
}

