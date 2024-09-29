/*
 ********************************************************************************
 * File: rand.c
 *
 *
 * Related Files: rand.h
 *
 * History:
 *
 * yyyymmmdd  Changed by       Change Reason
 * ---------  --------------   ------------------------------------------------
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static long seed1;
#define MODULUS    2147483647 /* DON'T CHANGE THIS VALUE                  */
#define MULTIPLIER 48271      /* DON'T CHANGE THIS VALUE                  */

double RawRandom()
{
	const long Q = MODULUS / MULTIPLIER;
	const long R = MODULUS % MULTIPLIER;
	long t;

	t = MULTIPLIER * (seed1 % Q) - R * (seed1 / Q);

	if (t > 0)
	{
		seed1 = t;
	}
	else
	{
		seed1 = t + MODULUS;
	}

 // printf("-----------a   %d   || %f ", seed_,((double)seed_) / MODULUS ) ;
  double KRand = seed1;
  KRand = KRand  / MODULUS;
	//return ((double) seed_ / MODULUS);
  return KRand ;
}

static int RawSetSeed(lua_State* L)
{
	long curs = luaL_checknumber(L, 2);
  	seed1 = curs;
	return 0;
}

static int RandInt(lua_State* L)
{
	long imin = luaL_checknumber(L, 2);
	long imax = luaL_checknumber(L, 3);
	double curRand = RawRandom();

	long dif = imax - imin + 1;

	long newInt = (long)(dif * curRand + imin) ;

	if (newInt > imax)
		newInt = imax;

	lua_pushnumber(L, newInt);
	return 1;
}

static int RandDouble(lua_State* L)
{
	double curRand = RawRandom();
	lua_pushnumber(L, curRand);
	return 1;
}

// signed int RandPickFromPrios( unsigned int * prios , unsigned int count ,signed long prioSum)
// {
// 	if (count == 0 ) return - 1;

// 	unsigned int i = 0,j=0;
// 	if(prioSum<0)
// 	{
// 		prioSum = 0;
// 		for(i = 0;i<count;i++)
// 		{
// 			prioSum += prios[i];
// 		}
// 	}

// 	long CurInt = RandInt(1, prioSum) ;

// 	for(j = 0 ;j<count;j++)
// 	{
// 		if(CurInt< prios[j])
// 		{
// 			return j;
// 		}
// 		else
// 		{
// 			CurInt -= prios[j];
// 		}
// 	}

// 	return -1 ;

// }


signed int RandPickFromPrios( double * prios , unsigned int count ,double prioSum)
{
	if (count == 0 ) return - 1;

	unsigned int i = 0,j=0;
	if(prioSum<0)
	{
		prioSum = 0;
		for(i = 0;i<count;i++)
		{
			prioSum += prios[i];
		}
	}

	double CurR = RawRandom() * prioSum;

	for(j = 0 ;j<count;j++)
	{
		if(CurR< prios[j])
		{
			return j;
		}
		else
		{
			CurR -= prios[j];
		}
	}

	return count - 1 ;

}

static int getmicrosecond(lua_State *L) {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long microsecond = tv.tv_sec*1000000+tv.tv_usec;
    lua_pushnumber(L, microsecond);
    return 1;
}

static int getmillisecond(lua_State *L) {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long millisecond = (tv.tv_sec*1000000+tv.tv_usec)/1000;
    lua_pushnumber(L, millisecond);
    return 1;
}

static luaL_Reg rand[] = {
	{"RandDouble", RandDouble},
	{"RawSetSeed", RawSetSeed},
	{"RandInt", RandInt},
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