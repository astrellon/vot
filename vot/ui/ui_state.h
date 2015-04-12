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
                    PAUSE_GAME
                };

                static bool init();
                static void deinit();

                static void state(StateValue state);
                static StateValue state();

            private:
                static StateValue s_state;
        };
    }
}
