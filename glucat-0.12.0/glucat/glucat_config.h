#ifndef _GLUCAT_GLUCAT_CONFIG_H
#define _GLUCAT_GLUCAT_CONFIG_H 1
 
/* glucat/glucat_config.h. Generated automatically at end of configure. */
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to dummy `main' function (if any) required to link to the Fortran
   libraries. */
/* #undef F77_DUMMY_MAIN */

/* Define if F77 and FC dummy `main' functions are identical. */
/* #undef FC_DUMMY_MAIN_EQ_F77 */

/* Define if you have a BLAS library. */
/* #undef HAVE_BLAS */

/* define if the compiler supports basic C++11 syntax */
#ifndef GLUCAT_HAVE_CXX11
#define GLUCAT_HAVE_CXX11 1
#endif

/* define if the compiler supports basic C++14 syntax */
/* #undef HAVE_CXX14 */

/* Define to 1 if you have the <inttypes.h> header file. */
#ifndef GLUCAT_HAVE_INTTYPES_H
#define GLUCAT_HAVE_INTTYPES_H 1
#endif

/* Define if you have LAPACK library. */
/* #undef HAVE_LAPACK */

/* Define to 1 if you have the `imf' library (-limf). */
/* #undef HAVE_LIBIMF */

/* Define to 1 if you have the <stdint.h> header file. */
#ifndef GLUCAT_HAVE_STDINT_H
#define GLUCAT_HAVE_STDINT_H 1
#endif

/* Define to 1 if you have the <stdio.h> header file. */
#ifndef GLUCAT_HAVE_STDIO_H
#define GLUCAT_HAVE_STDIO_H 1
#endif

/* Define to 1 if you have the <stdlib.h> header file. */
#ifndef GLUCAT_HAVE_STDLIB_H
#define GLUCAT_HAVE_STDLIB_H 1
#endif

/* Define to 1 if you have the <strings.h> header file. */
#ifndef GLUCAT_HAVE_STRINGS_H
#define GLUCAT_HAVE_STRINGS_H 1
#endif

/* Define to 1 if you have the <string.h> header file. */
#ifndef GLUCAT_HAVE_STRING_H
#define GLUCAT_HAVE_STRING_H 1
#endif

/* Define to 1 if you have the <sys/stat.h> header file. */
#ifndef GLUCAT_HAVE_SYS_STAT_H
#define GLUCAT_HAVE_SYS_STAT_H 1
#endif

/* Define to 1 if you have the <sys/types.h> header file. */
#ifndef GLUCAT_HAVE_SYS_TYPES_H
#define GLUCAT_HAVE_SYS_TYPES_H 1
#endif

/* Define to 1 if you have the <unistd.h> header file. */
#ifndef GLUCAT_HAVE_UNISTD_H
#define GLUCAT_HAVE_UNISTD_H 1
#endif

/* Name of package */
#ifndef GLUCAT_PACKAGE
#define GLUCAT_PACKAGE "glucat"
#endif

/* Define to the address where bug reports for this package should be sent. */
#ifndef GLUCAT_PACKAGE_BUGREPORT
#define GLUCAT_PACKAGE_BUGREPORT ""
#endif

/* Define to the full name of this package. */
#ifndef GLUCAT_PACKAGE_NAME
#define GLUCAT_PACKAGE_NAME "glucat"
#endif

/* Define to the full name and version of this package. */
#ifndef GLUCAT_PACKAGE_STRING
#define GLUCAT_PACKAGE_STRING "glucat 0.12.0"
#endif

/* Define to the one symbol short name of this package. */
#ifndef GLUCAT_PACKAGE_TARNAME
#define GLUCAT_PACKAGE_TARNAME "glucat"
#endif

/* Define to the home page for this package. */
#ifndef GLUCAT_PACKAGE_URL
#define GLUCAT_PACKAGE_URL ""
#endif

/* Define to the version of this package. */
#ifndef GLUCAT_PACKAGE_VERSION
#define GLUCAT_PACKAGE_VERSION "0.12.0"
#endif

/* Define to 1 if all of the C90 standard headers exist (not just the ones
   required in a freestanding environment). This macro is provided for
   backward compatibility; new code need not use it. */
#ifndef GLUCAT_STDC_HEADERS
#define GLUCAT_STDC_HEADERS 1
#endif

/* Version number of package */
#ifndef GLUCAT_VERSION
#define GLUCAT_VERSION "0.12.0"
#endif
 
/* once: _GLUCAT_GLUCAT_CONFIG_H */
#endif
