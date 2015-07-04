#pragma once

#include "../main.hpp"

extern "C" {
	# include <lua.h>
	# include <lauxlib.h>
	# include <lualib.h>
}

namespace luabridge {
	class LuaRef;
}
using namespace luabridge;


class Lua {
public:
	// PUBLIC MEMBERS


	// PUBLIC METHODS
	Lua( const std::string& _file);
	~Lua();

	std::string getStr( const std::string& _var, const std::string& _default = "");
	int getInt( const std::string& _var, int _default = 0);
	float getFloat( const std::string& _var, float _default = 0.f);
	bool getBool( const std::string& _var, bool _default = false);

	bool tableExists( const std::string& _var);

private:
	// PRIVATE MEMBERS
	lua_State* m_lua; //lua state
	LuaRef* m_table; //global table extracted from lua file
	std::string m_fileName; // lua script name for debugging
	
	// PRIVATE METHODS
	std::string adjustVarName( const std::string& _var, LuaRef* ref);
	LuaRef getLuaVar( const std::string& _var, int _type, LuaRef* _table );
	void getGlobalTable();
};

