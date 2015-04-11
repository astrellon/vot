#include "ui_state.h"

#include "main_menu.h"
#include "level_select.h"
#include "profile_select.h"

namespace vot
{
    namespace ui
    {
        State::StateValue State::s_state = State::MAIN_MENU;

        bool State::init()
        {
            if (!MainMenu::init())
            {
                return false;
            }
            if (!ProfileSelect::init())
            {
                return false;
            }
            if (!LevelSelect::init())
            {
                return false;
            }
            return true; 
        }
        void State::deinit()
        {
            LevelSelect::deinit();
            ProfileSelect::deinit();
            MainMenu::deinit();
        }

        void State::state(State::StateValue state)
        {
            s_state = state;
        }
        State::StateValue State::state()
        {
            return s_state;
        }
    }
}

