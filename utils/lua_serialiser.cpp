#include "lua_serialiser.h"

#include "data.h"

#include <fstream>

namespace utils
{
    // Serialise {{{
    void LuaSerialiser::serialise( utils::Data *value, const std::string &filename )
    {
        std::ofstream output(filename);
        serialise(value, output);
    }
    void LuaSerialiser::serialise(Data *value, std::ostream &output)
    {
        output << "data = ";
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
        else if (type == Data::INT32)
        {
            output << value->int32();
            return;
        }
        else if (type == Data::UINT32)
        {
            output << value->uint32();
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
        else if (type == Data::NIL)
        {
            output << "nil";
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
                output << iter->first << " = ";
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
    
    // Deserialise {{{
    Data *LuaSerialiser::deserialise(const std::string &filename)
    {
        std::ifstream input(filename);
        return deserialise(input);
    }
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
            lua_close(lua);
            return nullptr;
        }

        Data *result = nullptr;
        lua_getglobal(lua, "data");
        if (lua_istable(lua, -1))
        {
            result = do_deserialise(lua);
        }
        lua_close(lua);
        return result;
    }

    Data *LuaSerialiser::do_deserialise(lua_State *lua)
    {
        if (lua_istable(lua, -1))
        {
            auto table_type = lua_table_type(lua);
            if (table_type == ARRAY)
            {
                auto array = new Data(Data::ARRAY);
                lua_pushnil(lua);
                while (lua_next(lua, -2) != 0)
                {
                    array->push(do_deserialise(lua));
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
                    map->at(lua_tostring(lua, -2), do_deserialise(lua));
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
            auto type = lua_type(lua, -1);
            if (type == LUA_TNUMBER)
            {
                return new Data(lua_tonumber(lua, -1));
            }
            else if (type == LUA_TSTRING)
            {
                return new Data(lua_tostring(lua, -1));
            }
            else if (type == LUA_TBOOLEAN)
            {
                return new Data(lua_toboolean(lua, -1) > 0);
            }
            return Data::Nil();
        }
    }

    LuaSerialiser::LuaTableType LuaSerialiser::lua_table_type(lua_State *lua)
    {
        auto has_pair = false;
        auto has_ipair = false;

        lua_pushnil(lua);
        while (lua_next(lua, -2) != 0)
        {
            auto type = lua_type(lua, -2);
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
    // }}}
}
