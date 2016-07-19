#pragma once
#include "lua.h"
#include "unicode/utypes.h"

// Defines variable initialized as UTF-16 string
#define DEF_UTF16_LITERAL(var, str) \
  UChar var[strlen(str) + 1]; \
  u_charsToUChars(str, var, strlen(str)); \
  var[strlen(str)] = 0;

UChar * convert_str(const char * data, int32_t data_len, int32_t * str_len);
void push_str(lua_State *l, const UChar *str, int32_t str_len);
