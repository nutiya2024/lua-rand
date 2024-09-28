#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>



//微秒
static int getmicrosecond(lua_State *L) {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long microsecond = tv.tv_sec*1000000+tv.tv_usec;
    lua_pushnumber(L, microsecond);
    return 1;
}

//毫秒
static int getmillisecond(lua_State *L) {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long millisecond = (tv.tv_sec*1000000+tv.tv_usec)/1000;
    lua_pushnumber(L, millisecond);

    return 1;
}


int genrand(lua_State* L) 
{	

	char szBuf[200];  
	int seed = luaL_checknumber(L, 1);
	int min = luaL_checknumber(L, 2);
	int max = luaL_checknumber(L, 3);

	seed = seed * 214013L + 2531011L;
	int n = (seed >> 16) & 0x7fff;
	int val = (n * (max-min+1)) >> 15;
	lua_pushnumber(L, val);
	lua_pushnumber(L, seed);
	return 2;
}

//luaL_Reg结构体的第一个字段为字符串，在注册时用于通知Lua该函数的名字。
//第一个字段为C函数指针。
//结构体数组中的最后一个元素的两个字段均为NULL，用于提示Lua注册函数已经到达数组的末尾。
static luaL_Reg rand[] = { 
	{"genrand", genrand},
	{"getmillisecond", getmillisecond},
    {"getmicrosecond", getmicrosecond},
    {NULL, NULL} 
}; 

int luaopen_rand(lua_State* L) 
{
    lua_newtable(L); 
    luaL_setfuncs(L, rand, 0);
    return 1;
}
