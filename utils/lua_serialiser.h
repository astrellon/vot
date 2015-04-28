#pragma once

#include <iostream>

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
#	include <lua/src/lstate.h>
} 

namespace utils
{
    class Data;

    class LuaSerialiser
    {
        public:
            static void serialise(Data *value, std::ostream &output);
            static Data *deserialise(std::istream &input);

        private:
            static void do_serialise(Data *value, std::ostream &output, std::size_t depth);
            static Data *do_deserialise(lua_State *lua);

            static void output_tabs(std::size_t num, std::ostream &output);

            enum LuaTableType
            {
                ARRAY,
                MAP,
                BOTH
            };

            static LuaTableType lua_table_type(lua_State *lua);
    };
}
