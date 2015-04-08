#include "menu_helper.h"

#include "component.h"
#include "button.h"
#include "manager.h"

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
                _components[i]->enabled(value);
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

        void MenuHelper::add_component(Component *comp)
        {
            _components.push_back(comp);
            Manager::add_component(comp);
        }
        void MenuHelper::calc_nearby_components()
        {
            for (auto i = 0u; i < _components.size(); i++)
            {
                auto comp = _components[i];

                if (i == 0u)
                {
                    comp->to_above(_components.back());
                }
                else
                {
                    comp->to_above(_components[i - 1]);
                }
                
                if (i == _components.size() - 1)
                {
                    comp->to_below(_components.front());
                }
                if (i > 0)
                {
                    _components[i - 1]->to_below(comp);
                }
            }
        }
        const MenuHelper::ComponentList &MenuHelper::components() const
        {
            return _components;
        }

        void MenuHelper::on_resize(uint32_t width, uint32_t height)
        {
            auto size = _back_button->texture()->getSize();
            auto x = (static_cast<float>(width) - size.x) * 0.5f;

            //_back_button->setPosition(x, _back_button->getPosition().y);
            for (auto i = 0u; i < _components.size(); i++)
            {
                auto comp = _components[i];
                comp->setPosition(x, comp->getPosition().y);
            }
        }
    }
}
