#include "ui_state.h"

#include "main_menu.h"
#include "level_select.h"
#include "profile_select.h"
#include "profile_widget.h"
#include "ship_hanger.h"

namespace vot
{
    namespace ui
    {
        State::StateValue State::s_state = State::MAIN_MENU;

        bool State::init()
        {
            state(MAIN_MENU);

            return true; 
        }

        void State::state(State::StateValue state)
        {
            change_state(s_state, state);
            s_state = state;
        }
        State::StateValue State::state()
        {
            return s_state;
        }

        void State::change_state(StateValue prev_state, StateValue new_state)
        {
            if (prev_state == MAIN_MENU || prev_state == PAUSE_GAME)
            {
                MainMenu::visible(false);
            }
            else if (prev_state == PROFILE_SELECT)
            {
                ProfileSelect::visible(false);
            }
            else if (prev_state == LEVEL_SELECT)
            {
                LevelSelect::visible(false);
            }
            else if (prev_state == SHIP_HANGER)
            {
                ShipHanger::visible(false);
            }

            if (new_state == MAIN_MENU || new_state == PAUSE_GAME)
            {
                MainMenu::visible(true);
            }
            else if (new_state == PROFILE_SELECT)
            {
                ProfileSelect::visible(true);
            }
            else if (new_state == LEVEL_SELECT)
            {
                LevelSelect::visible(true);
            }
            else if (new_state == SHIP_HANGER)
            {
                ShipHanger::visible(true);
            }

            ProfileWidget::visible(new_state != IN_GAME && new_state != PAUSE_GAME);
        }
    }
}

