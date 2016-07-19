#include "utils.h"
#include <stdlib.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "unicode/ustring.h"

// Converts byte data (string from lua) into UTF-16 string
UChar * convert_str(const char * data, int32_t data_len, int32_t * str_len) {
  // Get actual string length
  UErrorCode status = U_ZERO_ERROR;
  u_strFromUTF8(NULL, 0, str_len, data, data_len, &status);
  if(U_FAILURE(status) && status != U_BUFFER_OVERFLOW_ERROR) {
    *str_len = -1;
    return NULL;
  }

  // Check for empty string
  if(*str_len < 1) {
    return NULL;
  }

  // Allocate memory
  UChar * buf = (UChar*) malloc(sizeof(UChar) * (*str_len + 1));
  if(!buf) {
    return NULL;
  }

  // Convert string to UTF-16
  status = U_ZERO_ERROR;
  u_strFromUTF8(buf, *str_len, NULL, data, data_len, &status);
  if(U_FAILURE(status)) {
    free(buf);
    return NULL;
  }

  // Terminator
  buf[*str_len] = 0;

  return buf;
}

// Converts string from UTF-16 into UTF-8 and pushes it on the stack
void push_str(lua_State *l, const UChar *str, int32_t str_len) {
  UErrorCode status = U_ZERO_ERROR;
  int32_t out_len;
  u_strToUTF8(NULL, 0, &out_len, str, str_len, &status);
  if(U_FAILURE(status)) {
    if(status != U_BUFFER_OVERFLOW_ERROR) {
      luaL_error(l, "Unable to convert string back to UTF-8.");
      return ;
    }

    char * result = malloc(out_len + 1);
    if(!result) {
      luaL_error(l, "Unable to allocate memory.");
      return ;
    }

    status = U_ZERO_ERROR;
    u_strToUTF8(result, out_len + 1, NULL, str, str_len, &status);
    if(U_FAILURE(status)) {
      free(result);
      luaL_error(l, "Unable to convert string back to UTF-8.");
      return ;
    }

    result[out_len] = 0;
    lua_pushstring(l, result);
    return ;
  }

  lua_pushstring(l, "");
}
