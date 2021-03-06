#pragma once

namespace vot
{
    namespace ui
    {
        class State
        {
            public:
                enum StateValue
                {
                    MAIN_MENU,
                    PROFILE_SELECT,
                    LEVEL_SELECT,
                    IN_GAME,
                    PAUSE_GAME,
                    SHIP_HANGER
                };

                static bool init();

                static void state(StateValue state);
                static StateValue state();

            private:
                static StateValue s_state;
                static void change_state(StateValue prev_state, StateValue new_state);
        };
    }
}
