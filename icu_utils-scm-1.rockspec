package = "icu_utils"
version = "scm-1"
source = {
  url = "..."
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
