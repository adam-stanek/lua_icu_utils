#include "latinize.h"
#include <stdlib.h>
#include <string.h>
#include "lualib.h"
#include "lauxlib.h"
#include "unicode/ustring.h"
#include "unicode/utrans.h"
#include "utils.h"


// Performs string transliteration and converts any accented characters into
// their plain version.
int latinize(lua_State *l) {
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

  // Create transliterator
  DEF_UTF16_LITERAL(translit_id, "Any-Latin; Latin-ASCII");
  UErrorCode status = U_ZERO_ERROR;
  UTransliterator * trans = utrans_openU(translit_id, -1, UTRANS_FORWARD, NULL, 0, NULL, &status);

  if(!trans) {
    free(str);
    return luaL_error(l, "Unable to initialize ICU transliterator.");
  }

  // Transliterate the string
  int32_t out_len = str_len;
  int32_t limit = str_len;

  utrans_transUChars(trans, str, &out_len, str_len, 0, &limit, &status);
  if(U_FAILURE(status)) {
    // We might just need to resize the buffer because transliterated version
    // might be sometime longer than the original (ie. U+0152).
    if(status == U_BUFFER_OVERFLOW_ERROR && out_len != str_len) {
      UChar * new_str = realloc(str, sizeof(UChar) * (out_len + 1));
      if(!new_str) {
        free(str);
        utrans_close(trans);
        return luaL_error(l, "Unable to allocate more memory.");
      }

      int32_t out_len2 = str_len;
      limit = str_len;
      status = U_ZERO_ERROR;

      utrans_transUChars(trans, new_str, &out_len2, out_len, 0, &limit, &status);
      if(U_FAILURE(status)) {
        free(new_str);
        utrans_close(trans);
        return luaL_error(l, "Unable to translit the string. Error code: %d.", status);
      }

      str = new_str;

    } else {
      free(str);
      utrans_close(trans);
      return luaL_error(l, "Unable to translit the string.");
    }
  }

  // Close transliterator, we already have the result
  utrans_close(trans);

  // Allocate memory for coversion back to UTF-8
  // Note: since we have transliterated string into ASCII UTF-8 representation it will not
  // have more bytes than number of characters (all ASCII chars are interpreted as a single byte).
  char * result = malloc(out_len + 1);
  u_strToUTF8(result, out_len + 1, NULL, str, out_len, &status);

  free(str);

  if(U_FAILURE(status)) {
    free(str);
    free(result);
    return luaL_error(l, "Unable to convert string back to UTF-8.");
  }

  lua_pushstring(l, result);
  free(result);

  return 1;
}
