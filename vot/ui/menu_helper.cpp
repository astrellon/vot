#include "menu_helper.h"

#include "component.h"
#include "button.h"
#include "manager.h"

#include <vot/utils/utils.h>
#include <vot/game_system.h>

namespace vot
{
    namespace ui
    {
        MenuHelper::MenuHelper() :
            _visible(false),
            _back_button(nullptr)
        {

        }

        void MenuHelper::visible(bool value)
        {
            _visible = value;

            for (auto i = 0u; i < _components.size(); i++)
            {
                _components[i].first->enabled(value);
            }

            if (value)
            {
                ui::Manager::focus(_back_button);
            }
            else
            {
                ui::Manager::focus(nullptr);
            }
        }
        bool MenuHelper::visible() const
        {
            return _visible;
        }

        void MenuHelper::back_button(Button *btn)
        {
            _back_button = btn;
            add_component(btn);
        }
        Button *MenuHelper::back_button() const
        {
            return _back_button;
        }

        void MenuHelper::add_component(Component *comp, bool adjust_on_resize)
        {
            _components.push_back(std::pair<Component *, bool>(comp, adjust_on_resize));
            Manager::add_component(comp);
        }
        void MenuHelper::remove_component(Component *comp)
        {
            for (auto iter = _components.begin(); iter != _components.end(); ++iter)
            {
                if (iter->first == comp)
                {
                    _components.erase(iter);
                    break;
                }
            }
            Manager::remove_component(comp);
        }
        void MenuHelper::clear_all_component()
        {
            for (auto i = 0u; i < _components.size(); i++)
            {
                Manager::remove_component(_components[i].first);
            }
            _components.clear();
        }
        void MenuHelper::calc_nearby_components()
        {
            for (auto i = 0u; i < _components.size(); i++)
            {
                auto comp = _components[i].first;

                if (i == 0u)
                {
                    comp->to_above(_components.back().first);
                }
                else
                {
                    comp->to_above(_components[i - 1].first);
                }
                
                if (i == _components.size() - 1)
                {
                    comp->to_below(_components.front().first);
                }
                if (i > 0)
                {
                    _components[i - 1].first->to_below(comp);
                }
            }
        }
        const MenuHelper::ComponentList &MenuHelper::components() const
        {
            return _components;
        }

        void MenuHelper::on_resize(uint32_t width, uint32_t height)
        {
            auto fwidth = static_cast<float>(width);
            for (auto i = 0u; i < _components.size(); i++)
            {
                auto comp = _components[i].first;
                if (!_components[i].second)
                {
                    continue;
                }
                auto size = comp->size();
                auto x = utils::Utils::round((fwidth - size.x) * 0.5f);

                comp->setPosition(x, comp->getPosition().y);
            }
        }
        void MenuHelper::on_resize()
        {
            auto size = GameSystem::window_size();
            on_resize(size.x, size.y);
        }
    }
}
