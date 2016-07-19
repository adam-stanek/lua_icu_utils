#include "normalize.h"
#include <stdlib.h>
#include <string.h>
#include "lualib.h"
#include "lauxlib.h"
#include "unicode/ustring.h"
#include "unicode/unorm2.h"
#include "utils.h"

// Normalize string into Unicode NFC form
// https://en.wikipedia.org/wiki/Unicode_equivalence
int normalize(lua_State *l) {
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

  // Get the normalizer instance
  UErrorCode status = U_ZERO_ERROR;
  const UNormalizer2 * norm = unorm2_getNFCInstance(&status);
  if(!norm) {
    return luaL_error(l, "Unable to create normalizer instance.");
  }

  // Quickly check string for denormalized segments and stop before any
  int32_t idx = unorm2_spanQuickCheckYes(norm, str, str_len, &status);

  // If whole string is already normalized, just return it
  if(idx == str_len) {
    lua_pushstring(l, data);
    return 1;
  }

  // Copy the denormalized suffix so that it does not get ovewritten
  UChar * second = (UChar *) malloc(sizeof(UChar) * (str_len - idx + 1));
  if(!second) {
    free(str);
    return luaL_error(l, "Unable to allocate memory.");
  }

  memcpy(second, str + idx, sizeof(UChar) * (str_len - idx + 1));

  // Normalize the suffix and append it to the already normalized prefix
  // Note: we are normalizing to NFC so the result should be shorter so we
  // should have enough space in the allocated memory.
  // Function returns length of the merged text.
  int32_t idx2 = unorm2_normalizeSecondAndAppend(norm, str, idx, str_len + 1, second, str_len - idx, &status);

  // Free up the denormalized suffix, we dont need it anymore
  free(second);

  if(U_FAILURE(status)) {
    free(str);
    return luaL_error(l, "Unable to normalize string.");
  }

  // Push back the UTF-8 representation
  push_str(l, str, idx2);
  free(str);
  return 1;
}
