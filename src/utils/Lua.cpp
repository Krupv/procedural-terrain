#include "Lua.hpp"

#include "../static_libs/LuaBridge/LuaBridge.h"

#include <string>
#include <cmath> // to return NAN for wrong lua float variable
				// returning INT_MAX for wrong lua integer variable
#include <algorithm> // number of '.' in string

std::string getLuaType(int _type)
{
	switch (_type) 
	{
		case LUA_TNIL: 
			return "nil";
		case LUA_TNUMBER: 
			return "int";
		case LUA_TSTRING: 
			return "string";
		case LUA_TTABLE: 
			return "table";
		case LUA_TFUNCTION: 
			return "function";
		case LUA_TUSERDATA: 
			return "userdata";
		default: 
			return "unknown";
	}
}


// PUBLIC
//------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////
Lua::Lua (  const std::string& _file )
{
	m_fileName = _file;

	m_lua = luaL_newstate();
	if (luaL_loadfile(m_lua, _file.c_str()) || lua_pcall(m_lua, 0,0,0))
	{
		error("lua file" + _file + " not found");
		return;
	}

	luaL_openlibs(m_lua);

	getGlobalTable();
}

////////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR
////////////////////////////////////////////////////////////////////////////////
Lua::~Lua ()
{
	if (m_table)
		delete m_table;
	if (m_lua)
		lua_close(m_lua);
}



////////////////////////////////////////////////////////////////////////////////
// GET VARIABLES
////////////////////////////////////////////////////////////////////////////////
std::string Lua::getStr( const std::string& _var, const std::string& _default)
{
	LuaRef ref = *m_table;

	std::string nam = adjustVarName(_var, &ref);
	
	ref = getLuaVar(nam, LUA_TSTRING, &ref);

	if (ref.isNil())
		return _default;
	else
		return ref.cast<std::string>();
}

int Lua::getInt( const std::string& _var, int _default)
{
	LuaRef ref = *m_table;

	std::string nam = adjustVarName(_var, &ref);
	
	ref = getLuaVar(nam, LUA_TNUMBER, &ref);

	if (ref.isNil())
		return _default;
	else
		return ref.cast<int>();
}

float Lua::getFloat( const std::string& _var, float _default)
{
	LuaRef ref = *m_table;

	std::string nam = adjustVarName(_var, &ref);
	
	ref = getLuaVar(nam, LUA_TNUMBER, &ref);

	if (ref.isNil())
		return _default;
	else
		return ref.cast<float>();
}

bool Lua::getBool( const std::string& _var, bool _default )
{
	LuaRef ref = *m_table;

	std::string nam = adjustVarName(_var, &ref);
	
	ref = getLuaVar(nam, LUA_TBOOLEAN, &ref);

	if (ref.isNil())
		return _default;
	else
		return ref.cast<bool>();
}

bool Lua::tableExists( const std::string& _var)
{
	LuaRef ref = *m_table;

	std::string nam = adjustVarName(_var, &ref);

	ref = getLuaVar(nam, LUA_TTABLE, &ref);

//	std::cout << "exist " << nam << " "<< ref[nam].cast<std::string>() << std::endl;
	
	return ref.isTable();

}

// PRIVATE
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// TREAT MULTITABLE VARIABLE NAME
////////////////////////////////////////////////////////////////////////////////
std::string Lua::adjustVarName( const std::string& _var, LuaRef* ref)
{
	std::size_t times = std::count(_var.begin(), _var.end(), '.');

	std::string str[10]; 
	
	unsigned int count = 0;
	for ( unsigned int i = 0; i < _var.size(); i++)
	{
		if (_var.at(i) == '.')
		{
			if (count == times )
				break;
			else
				*ref = getLuaVar(str[count], LUA_TTABLE, ref);
			count++;
		}
		else
		{
			str[count] += _var.at(i);
		}
		
	}

	return str[count];

}

////////////////////////////////////////////////////////////////////////////////
// GET VARIABLE FROM LUA
////////////////////////////////////////////////////////////////////////////////
LuaRef Lua::getLuaVar( const std::string& _var, int _type, LuaRef* _table )
{
	if (_table->isNil())
	{
		LuaRef nil(m_lua);
		return nil;
	}
		
	LuaRef out = (*_table)[_var.c_str()];

	if (out.isNil() || (out.type() != _type) )
	{
		#ifdef DEBUG
			std::string luaType = getLuaType( out.type() );
			error(_var + " is of type " + luaType + " in " + m_fileName);
		#endif

		//create empty lua reference
		out = LuaRef(m_lua);
	}
	return out;
}


////////////////////////////////////////////////////////////////////////////////
// get global table from lua file
////////////////////////////////////////////////////////////////////////////////
void Lua::getGlobalTable()
{ 
	m_table = new LuaRef(getGlobal(m_lua, "_G"));
}
