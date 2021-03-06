#include "LuaHelper.h"

extern "C" {
    #include "lua.h"
}
#include "luaT.h"
#include "THTensor.h"

#include <iostream>
using namespace std;

void dumpStack(lua_State *L) {
    cout << "luatop " << lua_gettop(L) << endl;
    if(lua_gettop(L) >= 1 ) cout << "-1 type " << lua_typename(L, lua_type(L, -1)) << endl;
    if(lua_gettop(L) >= 2 ) cout << "-2 type " << lua_typename(L, lua_type(L, -2)) << endl;
    if(lua_gettop(L) >= 3 ) cout << "-3 type " << lua_typename(L, lua_type(L, -3)) << endl;
}
void popAsSelf(lua_State *L, void *instanceKey) {
    lua_pushlightuserdata(L, instanceKey);
    lua_insert(L, -2);
    lua_settable(L, LUA_REGISTRYINDEX);
}
void deleteSelf(lua_State *L, void *instanceKey) {
    lua_pushlightuserdata(L, instanceKey);
    lua_pushnil(L);
    lua_settable(L, LUA_REGISTRYINDEX);
}
void pushSelf(lua_State *L, void *instanceKey) {
    lua_pushlightuserdata(L, instanceKey);
    lua_gettable(L, LUA_REGISTRYINDEX);
}
void getInstanceField(lua_State *L, void *instanceKey, const char *name) {
    lua_pushlightuserdata(L, instanceKey);
    lua_gettable(L, LUA_REGISTRYINDEX);
    lua_getfield(L, -1, name);
    lua_remove(L, -2);
}
const char * popString(lua_State *L) {
    const char *res = lua_tostring(L, -1);
    lua_remove(L, -1);
    return res;
}
float popFloat(lua_State *L) {
    float res = lua_tonumber(L, -1);
    lua_remove(L, -1);
    return res;
}
THFloatTensor *popFloatTensor(lua_State *L) {
    void **pTensor = (void **)lua_touserdata(L, -1);
    THFloatTensor *tensor = (THFloatTensor *)(*pTensor);
    lua_remove(L, -1);
    return tensor;
}
void pushFloatTensor(lua_State *L, THFloatTensor *tensor) {
    THFloatTensor_retain(tensor);
    luaT_pushudata(L, tensor, "torch.FloatTensor");
}
THDoubleTensor *popDoubleTensor(lua_State *L) {
    void **pTensor = (void **)lua_touserdata(L, -1);
    THDoubleTensor *tensor = (THDoubleTensor *)(*pTensor);
    lua_remove(L, -1);
    return tensor;
}
void pushDoubleTensor(lua_State *L, THDoubleTensor *tensor) {
    THDoubleTensor_retain(tensor);
    luaT_pushudata(L, tensor, "torch.DoubleTensor");
}
THByteTensor *popByteTensor(lua_State *L) {
    void **pTensor = (void **)lua_touserdata(L, -1);
    THByteTensor *tensor = (THByteTensor *)(*pTensor);
    lua_remove(L, -1);
    return tensor;
}
void pushByteTensor(lua_State *L, THByteTensor *tensor) {
    THByteTensor_retain(tensor);
    luaT_pushudata(L, tensor, "torch.ByteTensor");
}
void pushGlobal(lua_State *L, const char *name1) {
    lua_getglobal(L, name1);
}
void pushGlobal(lua_State *L, const char *name1, const char *name2) {
    lua_getglobal(L, name1);
    lua_getfield(L, -1, name2);
    lua_remove(L, -2);
}
void pushGlobal(lua_State *L, const char *name1, const char *name2, const char *name3) {
    lua_getglobal(L, name1);
    lua_getfield(L, -1, name2);
    lua_remove(L, -2);
    lua_getfield(L, -1, name3);
    lua_remove(L, -2);
}

void *getGlobal1(lua_State *L, const char *name1) {
    pushGlobal(L, name1);
    void **pres = (void **)lua_touserdata(L, -1);
    void *res = *pres;
//    void *res = lua_touserdata(L, -1);
    lua_remove(L, -1);
    return res;
}

void *getGlobal2(lua_State *L, const char *name1, const char *name2) {
    pushGlobal(L, name1, name2);
    void **pres = (void **)lua_touserdata(L, -1);
    void *res = *pres;
//    void *res = lua_touserdata(L, -1);
    lua_remove(L, -1);
    return res;
}

void luaRequire(lua_State *L, const char *name) {
    lua_getglobal(L, "require");
    lua_pushstring(L, name);
    lua_call(L, 1, 0);
//    lua_setglobal(L, "nn");
}
int getLuaRegistryIndex() {
    return LUA_REGISTRYINDEX;
}

