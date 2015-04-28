#include "lua_serialiser.h"

#include "data.h"

namespace utils
{
    // Serialise {{{
    void LuaSerialiser::serialise(Data *value, std::ostream &output)
    {
        do_serialise(value, output, 0u);
    }
    void LuaSerialiser::do_serialise(Data *value, std::ostream &output ,std::size_t depth)
    {
        if (value == nullptr)
        {
            output << "nil";
            return;
        }

        auto type = value->type();
        if (type == Data::NUMBER)
        {
            output << value->number();
            return;
        }
        else if (type == Data::STRING)
        {
            output << '"' << value->string() << '"';
            return;
        }
        else if (type == Data::BOOLEAN)
        {
            output << (value->boolean() ? "true" : "false");
            return;
        }
        else if (type == Data::ARRAY)
        {
            output << "{\n";

            ++depth;

            auto i = 0u;
            for (auto iter = value->begin_array(); iter != value->end_array(); ++iter)
            {
                output_tabs(depth, output);
                do_serialise(iter->get(), output, depth);
                if (i < value->size_array() - 1)
                {
                    output << ",\n";
                }
                ++i;
            }
            output << '\n';
            output_tabs(depth - 1, output);
            output << '}';
        }
        else if (type == Data::MAP)
        {
            output << "{\n";

            ++depth;

            auto i = 0u;
            for (auto iter = value->begin_map(); iter != value->end_map(); ++iter)
            {
                output_tabs(depth, output);
                output << '"' << iter->first << "\" = ";
                do_serialise(iter->second.get(), output, depth);
                if (i < value->size_map() - 1)
                {
                    output << ",\n";
                }
                ++i;
            }

            output << '\n';
            output_tabs(depth - 1, output);
            output << '}';
        }
    }

    void LuaSerialiser::output_tabs(std::size_t num, std::ostream &output)
    {
        for (auto i = 0u; i < num; i++)
        {
            output << '\t';
        }
    }
    // }}}
    
    Data *LuaSerialiser::deserialise(std::istream &input)
    {
		auto lua = luaL_newstate();
		// We always want the standard libs as they provide basic table manipulation.
		luaL_openlibs(lua);

        std::istreambuf_iterator<char> eos;
        std::string s(std::istreambuf_iterator<char>(input), eos);

        auto load_result = !luaL_dostring(lua, s.c_str());
        std::cout << "Load result: " << load_result << "\n";

        if (!load_result)
        {
            std::cout << "Load error: " << lua_tostring(lua, -1) << "\n";
            return nullptr;
        }

        return do_deserialise(lua, -1);
    }

    Data *LuaSerialiser::do_deserialise(lua_State *lua, int n)
    {
        if (lua_istable(lua, -1))
        {
            auto table_type = lua_table_type(lua, -1);
            if (table_type == ARRAY)
            {
                auto array = new Data(Data::ARRAY);
                lua_pushnil(lua);
                while (lua_next(lua, -2) != 0)
                {
                    array->push(do_deserialise(lua, -1));
                    lua_pop(lua, 1);
                }
                return array;
            }
            else if (table_type == MAP)
            {
                auto map = new Data(Data::MAP);
                lua_pushnil(lua);
                while (lua_next(lua, -2) != 0)
                {
                    map->at(lua_tostring(lua, -2), do_deserialise(lua, -1));
                    lua_pop(lua, 1);
                }
                return map;
            }
            else
            {
                std::cout << "Unsupported table contains pairs and ipairs :(\n";
                return nullptr;
            }
        }
        else
        {
            auto type = lua_type(lua, n);
            if (type == LUA_TNUMBER)
            {
                return new Data(lua_tonumber(lua, n));
            }
            else if (type == LUA_TSTRING)
            {
                return new Data(lua_tostring(lua, n));
            }
            else if (type == LUA_TBOOLEAN)
            {
                return new Data(lua_toboolean(lua, n) > 0);
            }
            else if (type == LUA_TNIL)
            {
                return nullptr;
            }
            return nullptr;
        }
    }

    LuaSerialiser::LuaTableType LuaSerialiser::lua_table_type(lua_State *lua, int n)
    {
        auto has_pair = false;
        auto has_ipair = false;

        lua_pushnil(lua);
        while (lua_next(lua, -2) != 0)
        {
            auto type = lua_type(lua, -1);
            if (type == LUA_TNUMBER)
            {
                has_ipair = true;
                if (has_pair)
                {
                    lua_pop(lua, 1);
                    return BOTH;
                }
            }
            else if (type == LUA_TSTRING)
            {
                has_pair = true;
                if (has_ipair)
                {
                    lua_pop(lua, 1);
                    return BOTH;
                }
            }
            lua_pop(lua, 1);
        }

        if (has_pair)
        {
            return MAP;
        }
        return ARRAY;
    }

}
