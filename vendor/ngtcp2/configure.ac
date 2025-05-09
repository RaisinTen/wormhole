# ngtcp2
#
# Copyright (c) 2016 ngtcp2 contributors
# Copyright (c) 2012 nghttp2 contributors
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
AC_PREREQ(2.61)
AC_INIT([ngtcp2], [0.15.0], [t-tujikawa@users.sourceforge.net])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_USE_SYSTEM_EXTENSIONS

LT_PREREQ([2.2.6])
LT_INIT()

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([subdir-objects])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# See versioning rule:
#  https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
AC_SUBST(LT_CURRENT, 12)
AC_SUBST(LT_REVISION, 0)
AC_SUBST(LT_AGE, 0)

AC_SUBST(CRYPTO_OPENSSL_LT_CURRENT, 4)
AC_SUBST(CRYPTO_OPENSSL_LT_REVISION, 1)
AC_SUBST(CRYPTO_OPENSSL_LT_AGE, 0)

AC_SUBST(CRYPTO_GNUTLS_LT_CURRENT, 4)
AC_SUBST(CRYPTO_GNUTLS_LT_REVISION, 1)
AC_SUBST(CRYPTO_GNUTLS_LT_AGE, 0)

AC_SUBST(CRYPTO_WOLFSSL_LT_CURRENT, 1)
AC_SUBST(CRYPTO_WOLFSSL_LT_REVISION, 1)
AC_SUBST(CRYPTO_WOLFSSL_LT_AGE, 0)

# from nghttp2
major=`echo $PACKAGE_VERSION |cut -d. -f1 | sed -e "s/[^0-9]//g"`
minor=`echo $PACKAGE_VERSION |cut -d. -f2 | sed -e "s/[^0-9]//g"`
patch=`echo $PACKAGE_VERSION |cut -d. -f3 | cut -d- -f1 | sed -e "s/[^0-9]//g"`

PACKAGE_VERSION_NUM=`printf "0x%02x%02x%02x" "$major" "$minor" "$patch"`

AC_SUBST(PACKAGE_VERSION_NUM)

# Checks for command-line options from ngtcp2
AC_ARG_ENABLE([werror],
    [AS_HELP_STRING([--enable-werror],
                    [Turn on compile time warnings])],
    [werror=$enableval], [werror=no])

AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
                    [Turn on debug output])],
    [debug=$enableval], [debug=no])

if test "x${debug}" = "xyes"; then
  DEBUGCFLAGS="-O0 -g3"
  AC_SUBST([DEBUGCFLAGS])
  AC_DEFINE([DEBUGBUILD], [1], [Define to 1 to enable debug output.])
fi

AC_ARG_ENABLE([memdebug],
    [AS_HELP_STRING([--enable-memdebug],
                    [Turn on memory allocation debug output])],
    [memdebug=$enableval], [memdebug=no])

AC_ARG_ENABLE([mempool],
    [AS_HELP_STRING([--enable-mempool], [Turn on memory pool [default=yes]])],
    [mempool=$enableval], [mempool=yes])

AC_ARG_ENABLE(asan,
    AS_HELP_STRING([--enable-asan],
                   [Enable AddressSanitizer (ASAN)]),
    [asan=$enableval], [asan=no])

AC_ARG_ENABLE([lib-only],
    [AS_HELP_STRING([--enable-lib-only],
                    [Build libngtcp2 and libngtcp2_crypto only.])],
    [lib_only=$enableval], [lib_only=no])

AC_ARG_WITH([jemalloc],
    [AS_HELP_STRING([--with-jemalloc],
                    [Use jemalloc [default=check]])],
    [request_jemalloc=$withval], [request_jemalloc=check])

AC_ARG_WITH([cunit],
    [AS_HELP_STRING([--with-cunit],
                    [Use cunit [default=check]])],
    [request_cunit=$withval], [request_cunit=check])

AC_ARG_WITH([libnghttp3],
    [AS_HELP_STRING([--with-libnghttp3],
                    [Use libnghttp3 [default=check]])],
    [request_libnghttp3=$withval], [request_libnghttp3=check])

AC_ARG_WITH([libev],
    [AS_HELP_STRING([--with-libev],
                    [Use libev [default=check]])],
    [request_libev=$withval], [request_libev=check])

AC_ARG_WITH([openssl],
    [AS_HELP_STRING([--with-openssl],
                    [Use openssl [default=check]])],
    [request_openssl=$withval], [request_openssl=check])

AC_ARG_WITH([gnutls],
    [AS_HELP_STRING([--with-gnutls],
                    [Use gnutls [default=no]])],
    [request_gnutls=$withval], [request_gnutls=no])

AC_ARG_WITH([boringssl],
    [AS_HELP_STRING([--with-boringssl],
                    [Use boringssl [default=no]])],
    [request_boringssl=$withval], [request_boringssl=no])

AC_ARG_WITH([picotls],
    [AS_HELP_STRING([--with-picotls],
                    [Use picotls [default=no]])],
    [request_picotls=$withval], [request_picotls=no])

AC_ARG_WITH([wolfssl],
    [AS_HELP_STRING([--with-wolfssl],
                    [Use wolfSSL [default=no]])],
    [request_wolfssl=$withval], [request_wolfssl=no])

AC_ARG_VAR([BORINGSSL_CFLAGS], [C compiler flags for BORINGSSL])
AC_ARG_VAR([BORINGSSL_LIBS], [linker flags for BORINGSSL])

AC_ARG_VAR([PICOTLS_CFLAGS], [C compiler flags for PICOTLS])
AC_ARG_VAR([PICOTLS_LIBS], [linker flags for PICOTLS])

AC_ARG_VAR([WOLFSSL_CFLAGS], [C compiler flags for WOLFSSL])
AC_ARG_VAR([WOLFSSL_LIBS], [linker flags for WOLFSSL])

AC_ARG_VAR([LIBEV_CFLAGS], [C compiler flags for libev, skipping any checks])
AC_ARG_VAR([LIBEV_LIBS], [linker flags for libev, skipping any checks])

AC_ARG_VAR([JEMALLOC_CFLAGS],
           [C compiler flags for jemalloc, skipping any checks])
AC_ARG_VAR([JEMALLOC_LIBS], [linker flags for jemalloc, skipping any checks])

AC_ARG_VAR([LIBTOOL_LDFLAGS],
           [libtool specific flags (e.g., -static-libtool-libs)])

# Checks for programs
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P

PKG_PROG_PKG_CONFIG([0.20])

AX_CXX_COMPILE_STDCXX([20], [noext], [optional])

case "${build}" in
  *-apple-darwin*)
    EXTRA_DEFS="-D__APPLE_USE_RFC_3542"
    AC_SUBST([EXTRA_DEFS])
    ;;
esac

# Checks for libraries.

# cunit
have_cunit=no
if test "x${request_cunit}" != "xno"; then
  PKG_CHECK_MODULES([CUNIT], [cunit >= 2.1], [have_cunit=yes], [have_cunit=no])
  # If pkg-config does not find cunit, check it using AC_CHECK_LIB.  We
  # do this because Debian (Ubuntu) lacks pkg-config file for cunit.
  if test "x${have_cunit}" = "xno"; then
    AC_MSG_WARN([${CUNIT_PKG_ERRORS}])
    AC_CHECK_LIB([cunit], [CU_initialize_registry],
                 [have_cunit=yes], [have_cunit=no])
    if test "x${have_cunit}" = "xyes"; then
      CUNIT_LIBS="-lcunit"
      CUNIT_CFLAGS=""
      AC_SUBST([CUNIT_LIBS])
      AC_SUBST([CUNIT_CFLAGS])
    fi
  fi
  if test "x${have_cunit}" = "xyes"; then
    # cunit in Mac OS X requires ncurses. Note that in Mac OS X, test
    # program can be built without -lncurses, but it emits runtime
    # error.
    case "${build}" in
      *-apple-darwin*)
        CUNIT_LIBS="$CUNIT_LIBS -lncurses"
        AC_SUBST([CUNIT_LIBS])
        ;;
    esac
  fi
fi

if test "x${request_cunit}" = "xyes" &&
   test "x${have_cunit}" != "xyes"; then
  AC_MSG_ERROR([cunit was requested (--with-cunit) but not found])
fi

AM_CONDITIONAL([HAVE_CUNIT], [ test "x${have_cunit}" = "xyes" ])

AM_CONDITIONAL([ENABLE_SHARED], [ test "x${enable_shared}" = "xyes" ])

# OpenSSL (required for libngtcp2_crypto_openssl,
# libngtcp2_crypto_picotls and examples)
have_openssl=no
have_vanilla_openssl=no
if test "x${request_openssl}" != "xno"; then
  PKG_CHECK_MODULES([OPENSSL], [openssl >= 1.1.1],
                    [have_openssl=yes], [have_openssl=no])
  if test "x${have_openssl}" = "xno"; then
    AC_MSG_NOTICE($OPENSSL_PKG_ERRORS)
  else
    have_vanilla_openssl=yes
    VANILLA_OPENSSL_LIBS="$OPENSSL_LIBS"
    VANILLA_OPENSSL_CFLAGS="$OPENSSL_CFLAGS"

    AC_SUBST(VANILLA_OPENSSL_LIBS)
    AC_SUBST(VANILLA_OPENSSL_CFLAGS)

    # Until OpenSSL gains mainline support for QUIC, check for a
    # patched version.

    save_CFLAGS="$CFLAGS"
    save_LIBS="$LIBS"
    CFLAGS="$OPENSSL_CFLAGS $CFLAGS"
    LIBS="$OPENSSL_LIBS $LIBS"

    AC_MSG_CHECKING([for SSL_is_quic])
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[
      #include <openssl/ssl.h>
    ]], [[
      SSL *ssl = NULL;
      SSL_is_quic(ssl);
    ]])],
    [AC_MSG_RESULT([yes]); have_openssl_quic=yes],
    [AC_MSG_RESULT([no]); have_openssl_quic=no])

    CFLAGS="$save_CFLAGS"
    LIBS="$save_LIBS"

    if test "x${have_openssl_quic}" = "xno"; then
      AC_MSG_NOTICE([openssl does not have QUIC interface, disabling it])
      have_openssl=no
      OPENSSL_LIBS=
      OPENSSL_CFLAGS=
    fi
  fi
fi

if test "x${request_openssl}" = "xyes" &&
   test "x${have_openssl}" != "xyes"; then
  AC_MSG_ERROR([openssl was requested (--with-openssl) but not found])
fi

AM_CONDITIONAL([HAVE_OPENSSL], [ test "x${have_openssl}" = "xyes" ])

# GnuTLS (required for libngtcp2_crypto_gnutls)
have_gnutls=no
if test "x${request_gnutls}" != "xno"; then
  PKG_CHECK_MODULES([GNUTLS], [gnutls >= 3.7.2],
                    [have_gnutls=yes], [have_gnutls=no])
  if test "x${have_gnutls}" = "xno"; then
    AC_MSG_NOTICE($GNUTLS_PKG_ERRORS)
  fi
fi

if test "x${request_gnutls}" = "xyes" &&
   test "x${have_gnutls}" != "xyes"; then
  AC_MSG_ERROR([gnutls was requested (--with-gnutls) but not found])
fi

AM_CONDITIONAL([HAVE_GNUTLS], [ test "x${have_gnutls}" = "xyes" ])

# BoringSSL (required for libngtcp2_crypto_boringssl)
have_boringssl=no
if test "x${request_boringssl}" != "xno"; then
  save_CFLAGS="$CFLAGS"
  save_LIBS="$LIBS"
  CFLAGS="$BORINGSSL_CFLAGS $CFLAGS"
  LIBS="$BORINGSSL_LIBS $LIBS"

  AC_MSG_CHECKING([for SSL_set_quic_early_data_context])
  AC_LINK_IFELSE([AC_LANG_PROGRAM([[
    #include <openssl/ssl.h>
  ]], [[
    SSL *ssl = NULL;
    SSL_set_quic_early_data_context(ssl, NULL, 0);
  ]])],
  [AC_MSG_RESULT([yes]); have_boringssl=yes],
  [AC_MSG_RESULT([no]); have_boringssl=no])

  CFLAGS="$save_CFLAGS"
  LIBS="$save_LIBS"
fi

if test "x${request_boringssl}" = "xyes" &&
   test "x${have_boringssl}" != "xyes"; then
  AC_MSG_ERROR([boringssl was requested (--with-boringssl) but not found])
fi

AM_CONDITIONAL([HAVE_BORINGSSL], [ test "x${have_boringssl}" = "xyes" ])

# Picotls openssl backend (required for libngtcp2_crypto_picotls)
have_picotls=no
if test "x${request_picotls}" != "xno"; then
  save_CFLAGS="$CFLAGS"
  save_LIBS="$LIBS"
  CFLAGS="$PICOTLS_CFLAGS $VANILLA_OPENSSL_CFLAGS $CFLAGS"
  LIBS="$PICOTLS_LIBS $VANILLA_OPENSSL_LIBS $LIBS"

  AC_MSG_CHECKING([for ptls_openssl_random_bytes])
  AC_LINK_IFELSE([AC_LANG_PROGRAM([[
    #include <picotls.h>
    #include <picotls/openssl.h>
  ]], [[
    ptls_openssl_random_bytes(NULL, 0);
  ]])],
  [AC_MSG_RESULT([yes]); have_picotls=yes],
  [AC_MSG_RESULT([no]); have_picotls=no])

  CFLAGS="$save_CFLAGS"
  LIBS="$save_LIBS"
fi

if test "x${request_picotls}" = "xyes" &&
   test "x${have_picotls}" != "xyes"; then
  AC_MSG_ERROR([picotls was requested (--with-picotls) but not found])
fi

AM_CONDITIONAL([HAVE_PICOTLS], [ test "x${have_picotls}" = "xyes" ])

# wolfSSL (required for libngtcp2_crypto_wolfssl)
have_wolfssl=no
if test "x${request_wolfssl}" != "xno"; then
  PKG_CHECK_MODULES([WOLFSSL], [wolfssl >= 5.5.0],
                    [have_wolfssl=yes], [have_wolfssl=no])
  if test "x${have_wolfssl}" = "xno"; then
    AC_MSG_NOTICE($WOLFSSL_PKG_ERRORS)
  fi
fi

if test "x${request_wolfssl}" = "xyes" &&
   test "x${have_wolfssl}" != "xyes"; then
  AC_MSG_ERROR([wolfssl was requested (--with-wolfssl) but not found])
fi

AM_CONDITIONAL([HAVE_WOLFSSL], [ test "x${have_wolfssl}" = "xyes" ])

have_crypto=no
if test "x${have_openssl}" = "xyes" ||
   test "x${have_gnutls}" = "xyes" ||
   test "x${have_boringssl}" = "xyes" ||
   test "x${have_picotls}" = "xyes" ||
   test "x${have_wolfssl}" = "xyes"; then
  have_crypto=yes
fi

AM_CONDITIONAL([HAVE_CRYPTO], [ test "x${have_crypto}" = "xyes" ])

# libnghttp3 (required for examples)
have_libnghttp3=no
if test "x${request_libnghttp3}" != "xno"; then
  PKG_CHECK_MODULES([LIBNGHTTP3], [libnghttp3 >= 0.11.0],
                    [have_libnghttp3=yes], [have_libnghttp3=no])
  if test "${have_libnghttp3}" = "xno"; then
    AC_MSG_NOTICE($LIBNGHTTP3_PKG_ERRORS)
  fi
fi

if test "x${request_libnghttp3}" = "xyes" &&
   test "x${have_libnghttp3}" != "xyes"; then
  AC_MSG_ERROR([libnghttp3 was requested (--with-libnghttp3) but not found])
fi

AM_CONDITIONAL([HAVE_NGHTTP3], [ test "x${have_libnghttp3}" = "xyes" ])

# libev (required for examples)
have_libev=no
if test "x${request_libev}" != "xno"; then
  if test "x${LIBEV_LIBS}" = "x" && test "x${LIBEV_CFLAGS}" = "x"; then
    # libev does not have pkg-config file.  Check it in an old way.
    save_LIBS=$LIBS
    # android requires -lm for floor
    AC_CHECK_LIB([ev], [ev_time], [have_libev=yes], [have_libev=no], [-lm])
    if test "x${have_libev}" = "xyes"; then
      AC_CHECK_HEADER([ev.h], [have_libev=yes], [have_libev=no])
      if test "x${have_libev}" = "xyes"; then
        LIBEV_LIBS=-lev
        LIBEV_CFLAGS=
      fi
    fi
    LIBS=$save_LIBS
  else
    have_libev=yes
  fi
fi

if test "x${request_libev}" = "xyes" &&
   test "x${have_libev}" != "xyes"; then
  AC_MSG_ERROR([libev was requested (--with-libev) but not found])
fi

if test "x${lib_only}" = "xno" &&
   test "x${HAVE_CXX20}" != "x1"; then
  AC_MSG_WARN([C++ compiler is not capable of C++20.  Examples will not be built.])
fi

enable_examples=no
if test "x${lib_only}" = "xno" &&
   test "x${have_libnghttp3}" = "xyes" &&
   test "x${have_crypto}" = "xyes" &&
   test "x${have_libev}" = "xyes" &&
   test "x${HAVE_CXX20}" = "x1"; then
  enable_examples=yes

  if test "x${have_openssl}" = "xyes"; then
    AC_DEFINE([ENABLE_EXAMPLE_OPENSSL], [1],
              [Define to 1 in order to build examples/{client,server}])
  fi

  if test "x${have_gnutls}" = "xyes"; then
    AC_DEFINE([ENABLE_EXAMPLE_GNUTLS], [1],
              [Define to 1 in order to build examples/{gtlsclient,gtlsserver}])
  fi

  if test "x${have_boringssl}" = "xyes"; then
    AC_DEFINE([ENABLE_EXAMPLE_BORINGSSL], [1],
              [Define to 1 in order to build examples/{bsslclient,bsslserver}])
  fi

  if test "x${have_picotls}" = "xyes"; then
    AC_DEFINE([ENABLE_EXAMPLE_PICOTLS], [1],
              [Define to 1 in order to build examples/{ptlsclient,ptlsserver}])
  fi

  if test "x${have_wolfssl}" = "xyes"; then
    AC_DEFINE([ENABLE_EXAMPLE_WOLFSSL], [1],
              [Define to 1 in order to build examples/{wsslclient,wsslserver}])
  fi
fi

AM_CONDITIONAL([ENABLE_EXAMPLES], [ test "x${enable_examples}" = "xyes" ])
AM_CONDITIONAL([ENABLE_EXAMPLE_OPENSSL], [ test "x${enable_examples}" = "xyes" && test "x${have_openssl}" = "xyes" ])
AM_CONDITIONAL([ENABLE_EXAMPLE_GNUTLS], [ test "x${enable_examples}" = "xyes" && test "x${have_gnutls}" = "xyes" ])
AM_CONDITIONAL([ENABLE_EXAMPLE_BORINGSSL], [ test "x${enable_examples}" = "xyes" && test "x${have_boringssl}" = "xyes" ])
AM_CONDITIONAL([ENABLE_EXAMPLE_PICOTLS], [ test "x${enable_examples}" = "xyes" && test "x${have_picotls}" = "xyes" ])
AM_CONDITIONAL([ENABLE_EXAMPLE_WOLFSSL], [ test "x${enable_examples}" = "xyes" && test "x${have_wolfssl}" = "xyes" ])

AC_SUBST([EXAMPLES_ENABLED], "${enable_examples}")
AC_SUBST([EXAMPLES_OPENSSL], "${have_openssl}")
AC_SUBST([EXAMPLES_GNUTLS], "${have_gnutls}")
AC_SUBST([EXAMPLES_BORINGSSL], "${have_boringssl}")
AC_SUBST([EXAMPLES_PICOTLS], "${have_picotls}")
AC_SUBST([EXAMPLES_WOLFSSL], "${have_wolfssl}")

# jemalloc
have_jemalloc=no
if test "x${request_jemalloc}" != "xno"; then
  if test "x${JEMALLOC_LIBS}" = "x" && test "x${JEMALLOC_CFLAGS}" = "x"; then
    save_LIBS=$LIBS
    AC_SEARCH_LIBS([malloc_stats_print], [jemalloc], [have_jemalloc=yes], [],
                   [$PTHREAD_LDFLAGS])

    if test "x${have_jemalloc}" = "xyes"; then
      jemalloc_libs=${ac_cv_search_malloc_stats_print}
    else
      # On Darwin, malloc_stats_print is je_malloc_stats_print
      AC_SEARCH_LIBS([je_malloc_stats_print], [jemalloc], [have_jemalloc=yes],
                     [], [$PTHREAD_LDFLAGS])

      if test "x${have_jemalloc}" = "xyes"; then
        jemalloc_libs=${ac_cv_search_je_malloc_stats_print}
      fi
    fi

    LIBS=$save_LIBS

    if test "x${have_jemalloc}" = "xyes" &&
       test "x${jemalloc_libs}" != "xnone required"; then
      JEMALLOC_LIBS=${jemalloc_libs}
    fi
  else
    have_jemalloc=yes
  fi
fi

if test "x${request_jemalloc}" = "xyes" &&
   test "x${have_jemalloc}" != "xyes"; then
  AC_MSG_ERROR([jemalloc was requested (--with-jemalloc) but not found])
fi

# Checks for header files.
AC_CHECK_HEADERS([ \
  arpa/inet.h \
  netinet/in.h \
  stddef.h \
  stdint.h \
  stdlib.h \
  string.h \
  unistd.h \
  sys/endian.h \
  endian.h \
  byteswap.h \
  asm/types.h \
  linux/netlink.h \
  linux/rtnetlink.h
])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_UID_T
AC_CHECK_TYPES([ptrdiff_t])
AC_C_BIGENDIAN
AC_C_INLINE
AC_SYS_LARGEFILE

# Checks for library functions.
AC_CHECK_FUNCS([ \
  memmove \
  memset \
])

# Checks for symbols.
AC_CHECK_DECLS([be64toh], [], [], [[
#ifdef HAVE_ENDIAN_H
#  include <endian.h>
#endif
#ifdef HAVE_SYS_ENDIAN_H
#  include <sys/endian.h>
#endif
]])

AC_CHECK_DECLS([bswap_64], [], [], [[
#include <byteswap.h>
]])

# More compiler flags from nghttp2.
save_CFLAGS=$CFLAGS
save_CXXFLAGS=$CXXFLAGS

CFLAGS=
CXXFLAGS=

if test "x$werror" != "xno"; then
    # For C compiler
    AX_CHECK_COMPILE_FLAG([-Wall], [CFLAGS="$CFLAGS -Wall"])
    AX_CHECK_COMPILE_FLAG([-Wextra], [CFLAGS="$CFLAGS -Wextra"])
    AX_CHECK_COMPILE_FLAG([-Werror], [CFLAGS="$CFLAGS -Werror"])
    AX_CHECK_COMPILE_FLAG([-Wmissing-prototypes], [CFLAGS="$CFLAGS -Wmissing-prototypes"])
    AX_CHECK_COMPILE_FLAG([-Wstrict-prototypes], [CFLAGS="$CFLAGS -Wstrict-prototypes"])
    AX_CHECK_COMPILE_FLAG([-Wmissing-declarations], [CFLAGS="$CFLAGS -Wmissing-declarations"])
    AX_CHECK_COMPILE_FLAG([-Wpointer-arith], [CFLAGS="$CFLAGS -Wpointer-arith"])
    AX_CHECK_COMPILE_FLAG([-Wdeclaration-after-statement], [CFLAGS="$CFLAGS -Wdeclaration-after-statement"])
    AX_CHECK_COMPILE_FLAG([-Wformat-security], [CFLAGS="$CFLAGS -Wformat-security"])
    AX_CHECK_COMPILE_FLAG([-Wwrite-strings], [CFLAGS="$CFLAGS -Wwrite-strings"])
    AX_CHECK_COMPILE_FLAG([-Wshadow], [CFLAGS="$CFLAGS -Wshadow"])
    AX_CHECK_COMPILE_FLAG([-Winline], [CFLAGS="$CFLAGS -Winline"])
    AX_CHECK_COMPILE_FLAG([-Wnested-externs], [CFLAGS="$CFLAGS -Wnested-externs"])
    AX_CHECK_COMPILE_FLAG([-Wfloat-equal], [CFLAGS="$CFLAGS -Wfloat-equal"])
    AX_CHECK_COMPILE_FLAG([-Wundef], [CFLAGS="$CFLAGS -Wundef"])
    AX_CHECK_COMPILE_FLAG([-Wendif-labels], [CFLAGS="$CFLAGS -Wendif-labels"])
    AX_CHECK_COMPILE_FLAG([-Wempty-body], [CFLAGS="$CFLAGS -Wempty-body"])
    AX_CHECK_COMPILE_FLAG([-Wcast-align], [CFLAGS="$CFLAGS -Wcast-align"])
    AX_CHECK_COMPILE_FLAG([-Wclobbered], [CFLAGS="$CFLAGS -Wclobbered"])
    AX_CHECK_COMPILE_FLAG([-Wvla], [CFLAGS="$CFLAGS -Wvla"])
    AX_CHECK_COMPILE_FLAG([-Wpragmas], [CFLAGS="$CFLAGS -Wpragmas"])
    AX_CHECK_COMPILE_FLAG([-Wunreachable-code], [CFLAGS="$CFLAGS -Wunreachable-code"])
    AX_CHECK_COMPILE_FLAG([-Waddress], [CFLAGS="$CFLAGS -Waddress"])
    AX_CHECK_COMPILE_FLAG([-Wattributes], [CFLAGS="$CFLAGS -Wattributes"])
    AX_CHECK_COMPILE_FLAG([-Wdiv-by-zero], [CFLAGS="$CFLAGS -Wdiv-by-zero"])
    AX_CHECK_COMPILE_FLAG([-Wshorten-64-to-32], [CFLAGS="$CFLAGS -Wshorten-64-to-32"])

    AX_CHECK_COMPILE_FLAG([-Wconversion], [CFLAGS="$CFLAGS -Wconversion"])
    AX_CHECK_COMPILE_FLAG([-Wextended-offsetof], [CFLAGS="$CFLAGS -Wextended-offsetof"])
    AX_CHECK_COMPILE_FLAG([-Wformat-nonliteral], [CFLAGS="$CFLAGS -Wformat-nonliteral"])
    AX_CHECK_COMPILE_FLAG([-Wlanguage-extension-token], [CFLAGS="$CFLAGS -Wlanguage-extension-token"])
    AX_CHECK_COMPILE_FLAG([-Wmissing-field-initializers], [CFLAGS="$CFLAGS -Wmissing-field-initializers"])
    AX_CHECK_COMPILE_FLAG([-Wmissing-noreturn], [CFLAGS="$CFLAGS -Wmissing-noreturn"])
    AX_CHECK_COMPILE_FLAG([-Wmissing-variable-declarations], [CFLAGS="$CFLAGS -Wmissing-variable-declarations"])
    # Not used because we cannot change public structs
    # AX_CHECK_COMPILE_FLAG([-Wpadded], [CFLAGS="$CFLAGS -Wpadded"])
    AX_CHECK_COMPILE_FLAG([-Wsign-conversion], [CFLAGS="$CFLAGS -Wsign-conversion"])
    # Not used because this basically disallows default case
    # AX_CHECK_COMPILE_FLAG([-Wswitch-enum], [CFLAGS="$CFLAGS -Wswitch-enum"])
    AX_CHECK_COMPILE_FLAG([-Wunreachable-code-break], [CFLAGS="$CFLAGS -Wunreachable-code-break"])
    AX_CHECK_COMPILE_FLAG([-Wunused-macros], [CFLAGS="$CFLAGS -Wunused-macros"])
    AX_CHECK_COMPILE_FLAG([-Wunused-parameter], [CFLAGS="$CFLAGS -Wunused-parameter"])
    AX_CHECK_COMPILE_FLAG([-Wredundant-decls], [CFLAGS="$CFLAGS -Wredundant-decls"])
    # Only work with Clang for the moment
    AX_CHECK_COMPILE_FLAG([-Wheader-guard], [CFLAGS="$CFLAGS -Wheader-guard"])
    AX_CHECK_COMPILE_FLAG([-Wsometimes-uninitialized], [CFLAGS="$CFLAGS -Wsometimes-uninitialized"])

    # Only work with gcc7 for the moment
    AX_CHECK_COMPILE_FLAG([-Wduplicated-branches], [CFLAGS="$CFLAGS -Wduplicated-branches"])

    # This is required because we pass format string as "const char*.
    AX_CHECK_COMPILE_FLAG([-Wno-format-nonliteral], [CFLAGS="$CFLAGS -Wno-format-nonliteral"])

    # For C++ compiler
    AC_LANG_PUSH(C++)
    AX_CHECK_COMPILE_FLAG([-Wall], [CXXFLAGS="$CXXFLAGS -Wall"])
    AX_CHECK_COMPILE_FLAG([-Werror], [CXXFLAGS="$CXXFLAGS -Werror"])
    AX_CHECK_COMPILE_FLAG([-Wformat-security], [CXXFLAGS="$CXXFLAGS -Wformat-security"])
    AX_CHECK_COMPILE_FLAG([-Wsometimes-uninitialized], [CXXFLAGS="$CXXFLAGS -Wsometimes-uninitialized"])
    # Disable noexcept-type warning of g++-7.  This is not harmful as
    # long as all source files are compiled with the same compiler.
    AX_CHECK_COMPILE_FLAG([-Wno-noexcept-type], [CXXFLAGS="$CXXFLAGS -Wno-noexcept-type"])
    AC_LANG_POP()
fi

WARNCFLAGS=$CFLAGS
WARNCXXFLAGS=$CXXFLAGS

CFLAGS=$save_CFLAGS
CXXFLAGS=$save_CXXFLAGS

AC_SUBST([WARNCFLAGS])
AC_SUBST([WARNCXXFLAGS])

if test "x$asan" != "xno"; then
    # Assume both C and C++ compiler either support ASAN or not.
    save_LDFLAGS="$LDFLAGS"
    LDFLAGS="$LDFLAGS -fsanitize=address"
    AX_CHECK_COMPILE_FLAG([-fsanitize=address],
                          [CFLAGS="$CFLAGS -fsanitize=address"; CXXFLAGS="$CXXFLAGS -fsanitize=address"],
                          [LDFLAGS="$save_LDFLAGS"])
fi

if test "x${memdebug}" = "xyes"; then
  AC_DEFINE([MEMDEBUG], [1],
            [Define to 1 to enable memory allocation debug output.])
fi

if test "x${mempool}" != "xyes"; then
  AC_DEFINE([NOMEMPOOL], [1], [Define to 1 to disable memory pool.])
fi

# extra flags for API function visibility
EXTRACFLAG=
AX_CHECK_COMPILE_FLAG([-fvisibility=hidden], [EXTRACFLAG="-fvisibility=hidden"])

AC_SUBST([EXTRACFLAG])

AC_CONFIG_FILES([
  Makefile
  lib/Makefile
  lib/libngtcp2.pc
  lib/includes/Makefile
  lib/includes/ngtcp2/version.h
  tests/Makefile
  crypto/Makefile
  crypto/openssl/Makefile
  crypto/openssl/libngtcp2_crypto_openssl.pc
  crypto/includes/Makefile
  crypto/gnutls/Makefile
  crypto/gnutls/libngtcp2_crypto_gnutls.pc
  crypto/boringssl/Makefile
  crypto/boringssl/libngtcp2_crypto_boringssl.pc
  crypto/picotls/Makefile
  crypto/picotls/libngtcp2_crypto_picotls.pc
  crypto/wolfssl/Makefile
  crypto/wolfssl/libngtcp2_crypto_wolfssl.pc
  doc/Makefile
  doc/source/conf.py
  third-party/Makefile
  examples/Makefile
  examples/tests/config.ini
])
AC_OUTPUT

AC_MSG_NOTICE([summary of build options:

    Package version: ${VERSION}
    Library version: $LT_CURRENT:$LT_REVISION:$LT_AGE
    Install prefix:  ${prefix}
    System types:
      Build:         ${build}
      Host:          ${host}
      Target:        ${target}
    Compiler:
      C preprocessor: ${CPP}
      CPPFLAGS:       ${CPPFLAGS}
      C compiler:     ${CC}
      CFLAGS:         ${CFLAGS}
      C++ compiler:   ${CXX}
      CXXFLAGS:       ${CXXFLAGS}
      LDFLAGS:        ${LDFLAGS}
      WARNCFLAGS:     ${WARNCFLAGS}
      WARNCXXFLAGS:   ${WARNCXXFLAGS}
      EXTRACFLAG:     ${EXTRACFLAG}
      LIBS:           ${LIBS}
    Library:
      Shared:         ${enable_shared}
      Static:         ${enable_static}
    Libtool:
      LIBTOOL_LDFLAGS: ${LIBTOOL_LDFLAGS}
    Crypto helper libraries:
      libngtcp2_crypto_openssl:   ${have_openssl}
      libngtcp2_crypto_gnutls:    ${have_gnutls}
      libngtcp2_crypto_boringssl: ${have_boringssl}
      libngtcp2_crypto_picotls:   ${have_picotls}
      libngtcp2_crypto_wolfssl:   ${have_wolfssl}
    Test:
      CUnit:          ${have_cunit} (CFLAGS='${CUNIT_CFLAGS}' LIBS='${CUNIT_LIBS}')
    Debug:
      Debug:          ${debug} (CFLAGS='${DEBUGCFLAGS}')
    Libs:
      OpenSSL:        ${have_openssl} (CFLAGS='${OPENSSL_CFLAGS}' LIBS='${OPENSSL_LIBS}')
      Libev:          ${have_libev} (CFLAGS='${LIBEV_CFLAGS}' LIBS='${LIBEV_LIBS}')
      Libnghttp3:     ${have_libnghttp3} (CFLAGS='${LIBNGHTTP3_CFLAGS}' LIBS='${LIBNGHTTP3_LIBS}')
      Jemalloc:       ${have_jemalloc} (CFLAGS='${JEMALLOC_CFLAGS}' LIBS='${JEMALLOC_LIBS}')
      GnuTLS:         ${have_gnutls} (CFLAGS='${GNUTLS_CFLAGS}' LIBS='${GNUTLS_LIBS}')
      BoringSSL:      ${have_boringssl} (CFLAGS='${BORINGSSL_CFLAGS}' LIBS='${BORINGSSL_LIBS}')
      Picotls:        ${have_picotls} (CFLAGS='${PICOTLS_CFLAGS}' LIBS='${PICOTLS_LIBS}')
      wolfSSL:        ${have_wolfssl} (CFLAGS='${WOLFSSL_CFLAGS}' LIBS='${WOLFSSL_LIBS}')
    Examples:         ${enable_examples}
])
