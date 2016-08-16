package = "icu_utils"
version = "scm-1"
source = {
  url = "https://github.com/v3lbloud/lua_icu_utils.git"
}

description = {
  summary = "Library providing interface to selected utilities from ICU project.",
  homepage = "https://github.com/v3lbloud/lua_icu_utils"
}

dependencies = {
  "lua == 5.1"
}

build = {
  type = "make",
  build_variables = {
    CFLAGS="$(CFLAGS)",
    LDFLAGS="$(LIBFLAG)",
    LUA_INCDIR="$(LUA_INCDIR)"
  },
  install_variables = {
    INST_LIBDIR="$(LIBDIR)"
  }
}
