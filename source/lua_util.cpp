#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>

#include <ogc/video.h>
#include <wiiuse/wpad.h>

#include <fat.h>
#include <string.h>
#include <dirent.h>

#include "lua_util.h"



template<> void luaPushValue<char>(lua_State *L, const char &returnValue) { lua_pushnumber(L, (lua_Number)returnValue); }
template<> void luaPushValue<unsigned char>(lua_State *L, const unsigned char &returnValue) { lua_pushnumber(L, (lua_Number)returnValue); }
template<> void luaPushValue<short>(lua_State *L, const short &returnValue) { lua_pushnumber(L, (lua_Number)returnValue); }
template<> void luaPushValue<unsigned short>(lua_State *L, const unsigned short &returnValue) { lua_pushnumber(L, (lua_Number)returnValue); }
template<> void luaPushValue<int>(lua_State *L, const int &returnValue) { lua_pushnumber(L, (lua_Number)returnValue); }
template<> void luaPushValue<unsigned int>(lua_State *L, const unsigned int &returnValue) { lua_pushnumber(L, (lua_Number)returnValue); }

template<> void luaPushValue<void*>(lua_State *L, void* const& returnValue) { FFI_CAST("void*", returnValue); }
template<> void luaPushValue<unsigned short const*>(lua_State *L, unsigned short const* const& returnValue) { FFI_CAST("unsigned short const*", (unsigned short *)returnValue); }
template<> void luaPushValue<char*>(lua_State *L, char* const& returnValue) { FFI_CAST("char*", returnValue); }
template<> void luaPushValue<char const*>(lua_State *L, char const* const& returnValue) { FFI_CAST("char const*", (char*)returnValue); }


template<> unsigned char luaGetValue<unsigned char>(lua_State *L, int i) { return (unsigned char)lua_tointeger(L,i); }
template<> unsigned short luaGetValue<unsigned short>(lua_State *L, int i) { return (unsigned short)lua_tointeger(L,i); }
template<> int luaGetValue<int>(lua_State *L, int i) { return lua_tointeger(L,i); }
template<> size_t luaGetValue<size_t>(lua_State *L, int i) { return (size_t)lua_tonumber(L,i); }
template<> float luaGetValue<float>(lua_State *L, int i) { return (float)lua_tonumber(L,i); }

//TODO: macro for auto-casting strings and userdata
template<> void* luaGetValue<void*>(lua_State *L, int i) { return lua_touserdata(L,i); }
template<> void const* luaGetValue<void const*>(lua_State *L, int i) { return lua_touserdata(L,i); }
template<> char* luaGetValue<char*>(lua_State *L, int i) { return (char*)lua_tostring(L,i); }
template<> char const* luaGetValue<char const*>(lua_State *L, int i) { return lua_tostring(L,i); }
template<> unsigned char* luaGetValue<unsigned char*>(lua_State *L, int i) { return (unsigned char*)lua_tostring(L,i); }
template<> unsigned char const* luaGetValue<unsigned char const*>(lua_State *L, int i) { return (unsigned char const*)lua_tostring(L,i); }
template<> unsigned short* luaGetValue<unsigned short*>(lua_State *L, int i) { return (unsigned short*)lua_tostring(L,i); }
template<> unsigned short const* luaGetValue<unsigned short const*>(lua_State *L, int i) { return (unsigned short const*)lua_tostring(L,i); }
template<> int* luaGetValue<int*>(lua_State *L, int i) { return (int*)lua_touserdata(L,i); }
template<> unsigned int* luaGetValue<unsigned int*>(lua_State *L, int i) { return (unsigned int*)lua_touserdata(L,i); }

template<> char** luaGetValue<char**>(lua_State *L, int i) { return (char**)lua_touserdata(L,i); }
template<> char const** luaGetValue<char const**>(lua_State *L, int i) { return (char const**)lua_touserdata(L,i); }
template<> unsigned char** luaGetValue<unsigned char**>(lua_State *L, int i) { return (unsigned char**)lua_touserdata(L,i); }



 
char *readFile(const char *filename, size_t *size) {
	FILE *file = fopen(filename, "rb");
	if (!file) return NULL;
	fseek(file, 0, SEEK_END);
	*size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *buffer = (char*)malloc(*size + 1);
	fread(buffer, *size, 1, file);
	buffer[*size] = 0;
	fclose(file);
	return buffer;
}
 
//courtesy of lua.c
static int luaErrorHandler(lua_State *L) {	//err
	if (!lua_isstring(L, 1))  /* 'message' not a string? */
		return 1;  /* keep it intact */
	lua_getfield(L, LUA_GLOBALSINDEX, "debug");
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		return 1;
	}
	lua_getfield(L, -1, "traceback");
	if (!lua_isfunction(L, -1)) {
		lua_pop(L, 2);
		return 1;
	}
	lua_pushvalue(L, 1);  /* pass error message */
	lua_pushinteger(L, 2);  /* skip this function and traceback */
	lua_pcall(L, 2, 1, 0);  /* call debug.traceback */
	return 1;
}

//luaSafeCall prints by default.  maybe it should just return?
//and maybe each caller should print?
int luaSafeCall(lua_State *L, int nargs, int nres) {	//func, ...[nargs]
	
	lua_pushcfunction(L, luaErrorHandler);	//func, ...[nargs], errfunc
	int errfuncloc = lua_gettop(L) - nargs - 1;
	lua_insert(L, errfuncloc);				//errfunc, func, ...[nargs]
	int ret = 1;
	if (lua_pcall(L, nargs, nres, errfuncloc)) {	//errfunc, ...[nres] or errmsg
		ret = false;
		if (lua_isstring(L, -1)) {
			printf("%s\n", lua_tostring(L, -1));
		}
	}
	lua_remove(L, errfuncloc);		//...[nres] or errmsg
	return ret;
}
