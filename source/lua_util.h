#ifndef LUA_UTIL_H
#define LUA_UTIL_H


#include "lua.hpp"


char *readFile(const char *filename, size_t *size);
int luaSafeCall(lua_State *L, int nargs, int nres);	//func, ...[nargs]



#define FFI_CDEF(x)	\
	lua_getfield(L, ffi, "cdef");	\
	lua_pushstring(L, x);	\
	luaSafeCall(L, 1, 0);
	
#define FFI_CDEF_FILE(filename)	\
	{	\
		size_t size = 0;	\
		char *data = readFile(filename, &size);	\
		if (data) { FFI_CDEF(data);	free(data); } \
	}
	
#define FFI_CAST(type_str, ptr)	\
	lua_getglobal(L, "require");		/*require*/\
	lua_pushstring(L, "ffi");			/*require 'ffi'*/\
	luaSafeCall(L, 1, 1);				/*ffi*/\
	lua_getfield(L, -1, "cast");		/*ffi ffi.cast*/\
	lua_pushstring(L, type_str);		/*ffi ffi.cast type_str*/\
	lua_pushlightuserdata(L, ptr);		/*ffi ffi.cast type_str ptr*/\
	luaSafeCall(L, 2, 1);				/*ffi*/\
	lua_pop(L,1);						/**/

#define BINDNAME(x)	\
	lua_pushcfunction(L, binding_##x); \
	lua_setfield(L, -2, #x);

#define BINDv(x)	\
int binding_##x(lua_State *L) {	\
	x();	\
	return 0;	\
}

#define BINDi(x)	\
int binding_##x(lua_State *L) {	\
	int i = (int)x();	\
	lua_pushnumber(L, i);	\
	return 1;	\
}

#define BINDvp(x,t1) \
int binding_##x(lua_State *L) { \
	x((t1*)lua_touserdata(L,1));	\
	return 0;	\
}

#define BINDvi(x)	\
int binding_##x(lua_State *L) {	\
	x(lua_tointeger(L,1));	\
	return 0;	\
}

#define BINDii(x)	\
int binding_##x(lua_State *L) {	\
	lua_pushinteger(L, x(lua_tointeger(L, 1)));	\
	return 1;	\
}

#define BINDin(x) \
int binding_##x(lua_State *L) {	\
	lua_pushinteger(L, x(lua_tonumber(L, 1))); \
	return 1;	\
}

#define BINDpp(tr,x,t1)	\
int binding_##x(lua_State *L) { \
	tr *pr = (tr*)x((t1*)lua_touserdata(L,1)); \
	FFI_CAST(#tr" *", pr);	\
	return 1; \
}

#define BINDni2(x) \
int binding_##x(lua_State *L) {	\
	lua_pushnumber(L, x(lua_tointeger(L,1), lua_tointeger(L,2)));	\
	return 1;	\
}

#define BINDpi2(tr, x) \
int binding_##x(lua_State *L) {	\
	tr *pr = (tr*)x(lua_tointeger(L,1), lua_tointeger(L, 2));	\
	FFI_CAST(#tr" *", pr);	\
	return 1; \
}

#define BINDppi(tr, x, t1)	\
int binding_##x(lua_State *L) {	\
	tr *pr = (tr*)x((t1*)lua_touserdata(L,1), lua_tointeger(L, 2));	\
	FFI_CAST(#tr" *", pr);	\
	return 1; \
}

// structures by-reference ...
#define BINDvsi(x, s1)	\
int binding_##x(lua_State *L) {	\
	x(*(s1*)lua_touserdata(L,1), lua_tointeger(L,2));	\
	return 0;	\
}

#define BINDppi2(tr, x, t1) \
int binding_##x(lua_State *L) {	\
	tr *pr = (tr*)x((t1*)lua_touserdata(L,1), lua_tointeger(L,2), lua_tointeger(L,3)); \
	FFI_CAST(#tr" *", pr); \
	return 1;	\
}

#define BINDvi4(x)	\
int binding_##x(lua_State *L) {	\
	x(lua_tointeger(L,1), lua_tointeger(L,2), lua_tointeger(L,3), lua_tointeger(L,4));	\
	return 0;	\
}

#define BINDvpi5(x,t1) \
int binding_##x(lua_State *L) {	\
	x((t1*)lua_touserdata(L,1), lua_tointeger(L,2), lua_tointeger(L,3), lua_tointeger(L,4), lua_tointeger(L,5), lua_tointeger(L,6)); \
	return 0; \
}

#define BINDvn6(x)	\
int binding_##x(lua_State *L) {	\
	x(lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4),lua_tonumber(L,5),lua_tonumber(L,6));	\
	return 0;	\
}

#define BEGIN_PACKAGE(n)	\
	lua_newtable(L);	/*t*/ \
	lua_getglobal(L, "package"); /*t package*/	\
	lua_getfield(L, -1, "loaded"); /*t package package.loaded*/	\
	lua_remove(L, lua_gettop(L)-2);	/*t package.loaded*/ \
	lua_pushvalue(L, -2);	/*t package.loaded t*/ \
	lua_setfield(L, -2, "ffi."#n);	/*t package.loaded, package.loaded[ffi..n] = t*/ \
	lua_pop(L,1); /*t*/ \
	lua_getglobal(L, "setmetatable"); /*t setmetatable*/ \
	lua_insert(L, lua_gettop(L)-2);	/*setmetatable t*/ \
	lua_getglobal(L, "require");	/*setmetatable t require*/ \
	lua_pushstring(L, "ffi");	/*setmetatable t require "ffi"*/ \
	luaSafeCall(L, 1, 1);	/*setmetatable t ffi*/ \
	lua_getfield(L, -1, "C"); /*setmetatable t ffi ffi.C*/ \
	lua_remove(L, lua_gettop(L)-2); /*setmetatable t ffi.C*/ \
	luaSafeCall(L, 2, 1);		/*t, getmetatable(t) == (require 'ffi').C*/
	
#define END_PACKAGE()		lua_pop(L,1);






template<typename T> void luaPushValue(lua_State *L, const T &returnValue);

template<> void luaPushValue<char>(lua_State *L, const char &returnValue);
template<> void luaPushValue<unsigned char>(lua_State *L, const unsigned char &returnValue);
template<> void luaPushValue<short>(lua_State *L, const short &returnValue);
template<> void luaPushValue<unsigned short>(lua_State *L, const unsigned short &returnValue);
template<> void luaPushValue<int>(lua_State *L, const int &returnValue);
template<> void luaPushValue<unsigned int>(lua_State *L, const unsigned int &returnValue);

template<> void luaPushValue<void*>(lua_State *L, void* const& returnValue);
template<> void luaPushValue<unsigned short const*>(lua_State *L, unsigned short const* const& returnValue);
template<> void luaPushValue<char*>(lua_State *L, char* const& returnValue);
template<> void luaPushValue<char const*>(lua_State *L, char const* const& returnValue);

//TODO ffi-cast per-return-type

template<typename T> T luaGetValue(lua_State *L, int i);

template<> unsigned char luaGetValue<unsigned char>(lua_State *L, int i);
template<> unsigned short luaGetValue<unsigned short>(lua_State *L, int i);
template<> int luaGetValue<int>(lua_State *L, int i);
template<> size_t luaGetValue<size_t>(lua_State *L, int i);
template<> float luaGetValue<float>(lua_State *L, int i);

template<> void* luaGetValue<void*>(lua_State *L, int i);
template<> void const* luaGetValue<void const*>(lua_State *L, int i);
template<> char* luaGetValue<char*>(lua_State *L, int i);
template<> char const* luaGetValue<char const*>(lua_State *L, int i);
template<> unsigned char* luaGetValue<unsigned char*>(lua_State *L, int i);
template<> unsigned char const* luaGetValue<unsigned char const*>(lua_State *L, int i);
template<> unsigned short* luaGetValue<unsigned short*>(lua_State *L, int i);
template<> unsigned short const* luaGetValue<unsigned short const*>(lua_State *L, int i);
template<> int* luaGetValue<int*>(lua_State *L, int i);
template<> unsigned int* luaGetValue<unsigned int*>(lua_State *L, int i);

template<> char** luaGetValue<char**>(lua_State *L, int i);
template<> char const** luaGetValue<char const**>(lua_State *L, int i);
template<> unsigned char** luaGetValue<unsigned char**>(lua_State *L, int i);


//specifically crafted for mapping arguments from lua to C++

#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2


#define LT <
#define GT >

#define MAP_FOR_EACH_0(what, x)
#define MAP_FOR_EACH_1(what, x) what LT x GT(L, n)
#define MAP_FOR_EACH_2(what, x, ...) what LT x GT(L, n-1), MAP_FOR_EACH_1(what, __VA_ARGS__)
#define MAP_FOR_EACH_3(what, x, ...) what LT x GT(L, n-2), MAP_FOR_EACH_2(what, __VA_ARGS__)
#define MAP_FOR_EACH_4(what, x, ...) what LT x GT(L, n-3), MAP_FOR_EACH_3(what, __VA_ARGS__)
#define MAP_FOR_EACH_5(what, x, ...) what LT x GT(L, n-4), MAP_FOR_EACH_4(what, __VA_ARGS__)
#define MAP_FOR_EACH_6(what, x, ...) what LT x GT(L, n-5), MAP_FOR_EACH_5(what, __VA_ARGS__)
#define MAP_FOR_EACH_7(what, x, ...) what LT x GT(L, n-6), MAP_FOR_EACH_6(what, __VA_ARGS__)
#define MAP_FOR_EACH_8(what, x, ...) what LT x GT(L, n-7), MAP_FOR_EACH_7(what, __VA_ARGS__)
#define MAP_FOR_EACH_9(what, x, ...) what LT x GT(L, n-8), MAP_FOR_EACH_8(what, __VA_ARGS__)
#define MAP_FOR_EACH_10(what, x, ...) what LT x GT(L, n-9), MAP_FOR_EACH_9(what, __VA_ARGS__)
#define MAP_FOR_EACH_11(what, x, ...) what LT x GT(L, n-10), MAP_FOR_EACH_10(what, __VA_ARGS__)
#define MAP_FOR_EACH_12(what, x, ...) what LT x GT(L, n-11), MAP_FOR_EACH_11(what, __VA_ARGS__)
#define MAP_FOR_EACH_13(what, x, ...) what LT x GT(L, n-12), MAP_FOR_EACH_12(what, __VA_ARGS__)
#define MAP_FOR_EACH_14(what, x, ...) what LT x GT(L, n-13), MAP_FOR_EACH_13(what, __VA_ARGS__)
#define MAP_FOR_EACH_15(what, x, ...) what LT x GT(L, n-14), MAP_FOR_EACH_14(what, __VA_ARGS__)
#define MAP_FOR_EACH_16(what, x, ...) what LT x GT(L, n-15), MAP_FOR_EACH_15(what, __VA_ARGS__)

#define MAP_FOR_EACH_(N, what, ...) CONCATENATE(MAP_FOR_EACH_, N)(what, __VA_ARGS__)
#define MAP_FOR_EACH(what, ...) MAP_FOR_EACH_(NARG(__VA_ARGS__), what, __VA_ARGS__)



#define PARAM_FOR_EACH_0(x)	
#define PARAM_FOR_EACH_1(x) x p1
#define PARAM_FOR_EACH_2(x, ...) x p2, PARAM_FOR_EACH_1(__VA_ARGS__)
#define PARAM_FOR_EACH_3(x, ...) x p3, PARAM_FOR_EACH_2(__VA_ARGS__)
#define PARAM_FOR_EACH_4(x, ...) x p4, PARAM_FOR_EACH_3(__VA_ARGS__)
#define PARAM_FOR_EACH_5(x, ...) x p5, PARAM_FOR_EACH_4(__VA_ARGS__)
#define PARAM_FOR_EACH_6(x, ...) x p6, PARAM_FOR_EACH_5(__VA_ARGS__)
#define PARAM_FOR_EACH_7(x, ...) x p7, PARAM_FOR_EACH_6(__VA_ARGS__)
#define PARAM_FOR_EACH_8(x, ...) x p8, PARAM_FOR_EACH_7(__VA_ARGS__)
#define PARAM_FOR_EACH_9(x, ...) x p9, PARAM_FOR_EACH_8(__VA_ARGS__)
#define PARAM_FOR_EACH_10(x, ...) x p10, PARAM_FOR_EACH_9(__VA_ARGS__)
#define PARAM_FOR_EACH_11(x, ...) x p11, PARAM_FOR_EACH_10(__VA_ARGS__)
#define PARAM_FOR_EACH_12(x, ...) x p12, PARAM_FOR_EACH_11(__VA_ARGS__)
#define PARAM_FOR_EACH_13(x, ...) x p13, PARAM_FOR_EACH_12(__VA_ARGS__)
#define PARAM_FOR_EACH_14(x, ...) x p14, PARAM_FOR_EACH_13(__VA_ARGS__)
#define PARAM_FOR_EACH_15(x, ...) x p15, PARAM_FOR_EACH_14(__VA_ARGS__)
#define PARAM_FOR_EACH_16(x, ...) x p16, PARAM_FOR_EACH_15(__VA_ARGS__)

#define PARAM_FOR_EACH_(N, ...) CONCATENATE(PARAM_FOR_EACH_, N)(__VA_ARGS__)
#define PARAM_FOR_EACH(...) PARAM_FOR_EACH_(NARG(__VA_ARGS__), __VA_ARGS__)



#define INVOKE_FOR_EACH_0(x)	
#define INVOKE_FOR_EACH_1(x) p1
#define INVOKE_FOR_EACH_2(x, ...) p2, INVOKE_FOR_EACH_1(__VA_ARGS__)
#define INVOKE_FOR_EACH_3(x, ...) p3, INVOKE_FOR_EACH_2(__VA_ARGS__)
#define INVOKE_FOR_EACH_4(x, ...) p4, INVOKE_FOR_EACH_3(__VA_ARGS__)
#define INVOKE_FOR_EACH_5(x, ...) p5, INVOKE_FOR_EACH_4(__VA_ARGS__)
#define INVOKE_FOR_EACH_6(x, ...) p6, INVOKE_FOR_EACH_5(__VA_ARGS__)
#define INVOKE_FOR_EACH_7(x, ...) p7, INVOKE_FOR_EACH_6(__VA_ARGS__)
#define INVOKE_FOR_EACH_8(x, ...) p8, INVOKE_FOR_EACH_7(__VA_ARGS__)
#define INVOKE_FOR_EACH_9(x, ...) p9, INVOKE_FOR_EACH_8(__VA_ARGS__)
#define INVOKE_FOR_EACH_10(x, ...) p10, INVOKE_FOR_EACH_9(__VA_ARGS__)
#define INVOKE_FOR_EACH_11(x, ...) p11, INVOKE_FOR_EACH_10(__VA_ARGS__)
#define INVOKE_FOR_EACH_12(x, ...) p12, INVOKE_FOR_EACH_11(__VA_ARGS__)
#define INVOKE_FOR_EACH_13(x, ...) p13, INVOKE_FOR_EACH_12(__VA_ARGS__)
#define INVOKE_FOR_EACH_14(x, ...) p14, INVOKE_FOR_EACH_13(__VA_ARGS__)
#define INVOKE_FOR_EACH_15(x, ...) p15, INVOKE_FOR_EACH_14(__VA_ARGS__)
#define INVOKE_FOR_EACH_16(x, ...) p16, INVOKE_FOR_EACH_15(__VA_ARGS__)

#define INVOKE_FOR_EACH_(N, ...) CONCATENATE(INVOKE_FOR_EACH_, N)(__VA_ARGS__)
#define INVOKE_FOR_EACH(...) INVOKE_FOR_EACH_(NARG(__VA_ARGS__), ##__VA_ARGS__)


#define REVERSE 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define ARGN(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N
#define NARG_(dummy, ...) ARGN(__VA_ARGS__)
#define NARG(...) NARG_(dummy, ##__VA_ARGS__, REVERSE)



#define BINDFUNC(returnType, funcName, args...)	\
template<typename T> struct wrapper_##funcName {	\
	lua_State *L;	\
	returnType (*f)(args);	\
	wrapper_##funcName(returnType (*_f)(args)) : f(_f) {}	\
	int operator()(PARAM_FOR_EACH(args)) {	\
		luaPushValue<returnType>(L, f(INVOKE_FOR_EACH(args)));	\
		return 1;	\
	} \
};	\
template<> struct wrapper_##funcName<void> {	\
	lua_State *L;	\
	returnType (*f)(args);	\
	wrapper_##funcName(returnType (*_f)(args)) : f(_f) {}	\
	int operator()(PARAM_FOR_EACH(args)) {	\
		f(INVOKE_FOR_EACH(args));	\
		return 0;	\
	} \
};	\
static int binding_##funcName(lua_State *L) {	\
	int n = lua_gettop(L);	/* total # args */ \
	const int numExpectedArgs = NARG(args);	\
	if (n != numExpectedArgs) luaL_error(L, "expected %d but found %d arguments", numExpectedArgs, n);	\
	wrapper_##funcName<returnType> wrapper(funcName);	\
	wrapper.L = L;	\
	return wrapper.operator()(MAP_FOR_EACH(luaGetValue, args));	\
}






#endif	//LUA_UTIL_H
