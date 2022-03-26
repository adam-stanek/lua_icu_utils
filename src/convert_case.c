// Note: all 3 functions are basically the same. We should probably refactor
// them into some macro or using function pointer.

#include "convert_case.h"
#include <stdlib.h>
#include <string.h>
#include "lualib.h"
#include "lauxlib.h"
#include "unicode/ustring.h"
#include "utils.h"

// Converts string into lower case
int strtolower(lua_State *l) {
  // Convert argument into a UTF-8 string
  const char * data = luaL_checkstring(l, 1);
  int32_t data_len = strlen(data);

  int32_t str_len;
  UChar * str = convert_str(data, data_len, &str_len);

  if(!str) {
    // Empty string
    if(!str_len) {
      lua_pushstring(l, "");
      return 1;
    } else {
      return luaL_error(l, "Unable to decode string from UTF-8.");
    }
  }

  // Its pretty good guess that the size of the strings will be the same
  // so we allocate some memory and try it. But we still need to check
  // because some characters may in fact make the resulting string longer
  // than the original. For example, U+0130
  // (LATIN CAPITAL LETTER I WITH DOT ABOVE) maps to "U+0069 U+0307"
  // (LATIN SMALL LETTER I and COMBINING DOT ABOVE).
  UChar * dest = malloc(sizeof(UChar) * (str_len + 1));
  if(!dest) {
    free(str);
    return luaL_error(l, "Unable to allocate memory.");
  }

  // Convert the case
  UErrorCode status = U_ZERO_ERROR;
  int32_t out_len = u_strToLower(dest, str_len, str, str_len, "", &status);
  if(U_FAILURE(status)) {
    // Allocate more memory if necessary
    if(status == U_BUFFER_OVERFLOW_ERROR) {
      free(dest);
      dest = malloc(sizeof(UChar) * (out_len + 1));
      if(!dest) {
        free(str);
        return luaL_error(l, "Unable to allocate more memory.");
      }

      // Try again with more memory
      status = U_ZERO_ERROR;
      u_strToLower(dest, out_len, str, str_len, "", &status);
      if(U_FAILURE(status)) {
        free(str);
        return luaL_error(l, "Unable to convert string case. Error code: %d.", status);
      }
    } else {
      free(str);
      return luaL_error(l, "Unable to convert string case. Error code: %d.", status);
    }
  }

  // Push back the UTF-8 representation
  free(str);
  push_str(l, dest, out_len);
  free(dest);
  return 1;
}

// Converts string into upper case
int strtoupper(lua_State *l) {
  // Convert argument into a UTF-8 string
  const char * data = luaL_checkstring(l, 1);
  int32_t data_len = strlen(data);

  int32_t str_len;
  UChar * str = convert_str(data, data_len, &str_len);

  if(!str) {
    // Empty string
    if(!str_len) {
      lua_pushstring(l, "");
      return 1;
    } else {
      return luaL_error(l, "Unable to decode string from UTF-8.");
    }
  }

  // Its pretty good guess that the size of the strings will be the same
  // so we allocate some memory and try it. But we still need to check
  // because some characters may in fact make the resulting string longer
  // than the original. For example, U+00DF
  // (LATIN SMALL LETTER SHARP S) maps to "U+0053 U+0053"
  // (LATIN CAPITAL LETTER S and LATIN CAPITAL LETTER S).
  UChar * dest = malloc(sizeof(UChar) * (str_len + 1));
  if(!dest) {
    free(str);
    return luaL_error(l, "Unable to allocate memory.");
  }

  // Convert the case
  UErrorCode status = U_ZERO_ERROR;
  int32_t out_len = u_strToUpper(dest, str_len, str, str_len, "", &status);
  if(U_FAILURE(status)) {
    // Allocate more memory if necessary
    if(status == U_BUFFER_OVERFLOW_ERROR) {
      free(dest);
      dest = malloc(sizeof(UChar) * (out_len + 1));
      if(!dest) {
        free(str);
        return luaL_error(l, "Unable to allocate more memory.");
      }

      // Try again with more memory
      status = U_ZERO_ERROR;
      u_strToUpper(dest, out_len, str, str_len, "", &status);
      if(U_FAILURE(status)) {
        free(str);
        return luaL_error(l, "Unable to convert string case. Error code: %d.", status);
      }
    } else {
      free(str);
      return luaL_error(l, "Unable to convert string case. Error code: %d.", status);
    }
  }

  // Push back the UTF-8 representation
  free(str);
  push_str(l, dest, out_len);
  free(dest);
  return 1;
}

// Convert string into title case
int strtotitle(lua_State *l) {
  // Convert argument into a UTF-8 string
  const char * data = luaL_checkstring(l, 1);
  int32_t data_len = strlen(data);

  int32_t str_len;
  UChar * str = convert_str(data, data_len, &str_len);

  if(!str) {
    // Empty string
    if(!str_len) {
      lua_pushstring(l, "");
      return 1;
    } else {
      return luaL_error(l, "Unable to decode string from UTF-8.");
    }
  }

  // Its pretty good guess that the size of the strings will be the same
  // so we allocate some memory and try it. But we still need to check
  // because some characters may in fact make the resulting string longer
  // than the original. For example, U+00DF
  // (LATIN SMALL LETTER SHARP S) maps to "U+0053 U+0053"
  // (LATIN CAPITAL LETTER S and LATIN CAPITAL LETTER S).
  UChar * dest = malloc(sizeof(UChar) * (str_len + 1));
  if(!dest) {
    free(str);
    return luaL_error(l, "Unable to allocate memory.");
  }

  // Convert the case
  UErrorCode status = U_ZERO_ERROR;
  int32_t out_len = u_strToTitle(dest, str_len, str, str_len, NULL, "", &status);
  if(U_FAILURE(status)) {
    // Allocate more memory if necessary
    if(status == U_BUFFER_OVERFLOW_ERROR) {
      free(dest);
      dest = malloc(sizeof(UChar) * (out_len + 1));
      if(!dest) {
        free(str);
        return luaL_error(l, "Unable to allocate more memory.");
      }

      // Try again with more memory
      status = U_ZERO_ERROR;
      u_strToTitle(dest, out_len, str, str_len, NULL, "", &status);
      if(U_FAILURE(status)) {
        free(str);
        return luaL_error(l, "Unable to convert string case. Error code: %d.", status);
      }
    } else {
      free(str);
      return luaL_error(l, "Unable to convert string case. Error code: %d.", status);
    }
  }

  // Push back the UTF-8 representation
  free(str);
  push_str(l, dest, out_len);
  free(dest);
  return 1;
}
