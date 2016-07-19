#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "convert_case.h"
#include "normalize.h"
#include "latinize.h"

// Init
// Push the module on the stack
int luaopen_icu_utils(lua_State * l) {
  lua_newtable(l);

  lua_pushcclosure(l, normalize, 0);
  lua_setfield(l, -2, "normalize");

  lua_pushcclosure(l, latinize, 0);
  lua_setfield(l, -2, "latinize");

  lua_pushcclosure(l, strtoupper, 0);
  lua_setfield(l, -2, "upper");

  lua_pushcclosure(l, strtolower, 0);
  lua_setfield(l, -2, "lower");

  lua_pushcclosure(l, strtotitle, 0);
  lua_setfield(l, -2, "capitalize");

  return 1;
}
