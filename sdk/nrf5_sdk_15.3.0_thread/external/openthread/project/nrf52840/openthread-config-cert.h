/* include/openthread-config-generic.h.  Generated from openthread-config-generic.h.in by configure.  */
/* include/openthread-config-generic.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if your C++ compiler doesn't accept -c and -o together. */
/* #undef CXX_NO_MINUS_C_MINUS_O */

/* Define to 1 if you have the <dlfcn.h> header file. */
/* #undef HAVE_DLFCN_H */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `edit' library (-ledit). */
/* #undef HAVE_LIBEDIT */

/* Define to 1 if you have the `readline' library (-lreadline). */
/* #undef HAVE_LIBREADLINE */

/* Define to 1 if you have the `memcpy' function. */
/* #undef HAVE_MEMCPY */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if stdbool.h conforms to C99. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define if strlcat exists. */
#define HAVE_STRLCAT 1

/* Define if strlcpy exists. */
#if defined ( __ICCARM__ )
#define HAVE_STRLCPY 0
#else
#define HAVE_STRLCPY 1
#endif

/* Define if strnlen exists. */
#if defined ( __CC_ARM )
#define HAVE_STRNLEN 0
#else
#define HAVE_STRNLEN 1
#endif

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if the system has the type `_Bool'. */
#define HAVE__BOOL 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Define to 1 if you want to enable support for bultin-mbedtls. */
#define OPENTHREAD_CONFIG_ENABLE_BUILTIN_MBEDTLS 1

/* Define to 1 if you want to enable CoAP to an application. */
#define OPENTHREAD_ENABLE_APPLICATION_COAP 1

/* Define to 1 if you want to enable CoAP Secure to an application. */
#define OPENTHREAD_ENABLE_APPLICATION_COAP_SECURE 1

/* Define to 1 to enable the border agent feature. */
#define OPENTHREAD_ENABLE_BORDER_AGENT 1

/* Define to 1 if you want to enable Border Router */
#define OPENTHREAD_ENABLE_BORDER_ROUTER 1

/* Define to 1 if you want to enable log for certification test */
#define OPENTHREAD_ENABLE_CERT_LOG 1

/* Define to 1 if you want to enable channel manager feature */
#define OPENTHREAD_ENABLE_CHANNEL_MANAGER 0

/* Define to 1 if you want to use channel monitor feature */
#define OPENTHREAD_ENABLE_CHANNEL_MONITOR 0

/* Define to 1 if you want to use child supervision feature */
#define OPENTHREAD_ENABLE_CHILD_SUPERVISION 0

/* Define to 1 to build CLI library. */
#define OPENTHREAD_ENABLE_CLI 1

/* Define to 1 to enable the commissioner role. */
#define OPENTHREAD_ENABLE_COMMISSIONER 1

/* Define to 1 if you want to enable DHCPv6 Client */
#define OPENTHREAD_ENABLE_DHCP6_CLIENT 1

/* Define to 1 if you want to enable DHCPv6 Server */
#define OPENTHREAD_ENABLE_DHCP6_SERVER 1

/* Define to 1 if you want to use diagnostics module */
#define OPENTHREAD_ENABLE_DIAG 1

/* Define to 1 if you want to enable DNS Client */
#define OPENTHREAD_ENABLE_DNS_CLIENT 1

/* Define to 1 to enable dtls support. */
#define OPENTHREAD_ENABLE_DTLS 1

/* Define to 1 if you want to enable ECDSA support */
#define OPENTHREAD_ENABLE_ECDSA 1

/* OpenThread examples */
#define OPENTHREAD_ENABLE_EXAMPLES nrf52840

/* Define to 1 to build executables. */
#define OPENTHREAD_ENABLE_EXECUTABLE 1

/* Define to 1 to build FTD library. */
#define OPENTHREAD_ENABLE_FTD 1

/* Define to 1 if you want to use jam detection feature */
#define OPENTHREAD_ENABLE_JAM_DETECTION 0

/* Define to 1 to enable the joiner role. */
#define OPENTHREAD_ENABLE_JOINER 1

/* Define to 1 if you want to use legacy network support */
#define OPENTHREAD_ENABLE_LEGACY 0

/* Define to 1 if you want to use MAC filter feature */
#define OPENTHREAD_ENABLE_MAC_FILTER 1

/* Define to 1 to build MTD library. */
#define OPENTHREAD_ENABLE_MTD 1

/* Define to 1 to enable network diagnostic for MTD. */
#define OPENTHREAD_ENABLE_MTD_NETWORK_DIAGNOSTIC 1

/* Define to 1 if you want to enable support for multiple OpenThread
   instances. */
#define OPENTHREAD_ENABLE_MULTIPLE_INSTANCES 0

/* Define to 1 to build NCP library. */
#define OPENTHREAD_ENABLE_NCP 1

/* Define to 1 to enable the NCP SPI interface. */
#ifndef OPENTHREAD_ENABLE_NCP_SPI
#define OPENTHREAD_ENABLE_NCP_SPI 0
#endif

/* Define to 1 if using NCP Spinel Encrypter */
#define OPENTHREAD_ENABLE_NCP_SPINEL_ENCRYPTER 0

/* Define to 1 to enable the NCP UART interface. */
#ifndef OPENTHREAD_ENABLE_NCP_UART
#define OPENTHREAD_ENABLE_NCP_UART 1
#endif

/* Define to 1 if using NCP vendor hook */
#define OPENTHREAD_ENABLE_NCP_VENDOR_HOOK 0

/* Define to 1 to enable platform Netif. */
#define OPENTHREAD_ENABLE_PLATFORM_NETIF 0

/* Define to 1 to enable platform UDP. */
#define OPENTHREAD_ENABLE_PLATFORM_UDP 0

/* Define to 1 to build posix application in daemon mode. */
#define OPENTHREAD_ENABLE_POSIX_APP_DAEMON 0

/* Define to 1 to build radio-only library. */
#define OPENTHREAD_ENABLE_RADIO_ONLY 1

/* Define to 1 if you want to enable raw link-layer API */
#define OPENTHREAD_ENABLE_RAW_LINK_API 1

/* Define to 1 if you want to enable Service */
#define OPENTHREAD_ENABLE_SERVICE 1

/* Define to 1 if you want to enable SNTP Client */
#define OPENTHREAD_ENABLE_SNTP_CLIENT 1

/* Define to 1 to enable the UDP forward feature. */
#define OPENTHREAD_ENABLE_UDP_FORWARD 1

/* Define to 1 if using vendor extension */
#define OPENTHREAD_ENABLE_VENDOR_EXTENSION 0

/* Define to 1 if you want to use cc1352 examples */
/* #undef OPENTHREAD_EXAMPLES_CC1352 */

/* Define to 1 if you want to use cc2538 examples */
/* #undef OPENTHREAD_EXAMPLES_CC2538 */

/* Define to 1 if you want to use cc2650 examples */
/* #undef OPENTHREAD_EXAMPLES_CC2650 */

/* Define to 1 if you want to use cc2652 examples */
/* #undef OPENTHREAD_EXAMPLES_CC2652 */

/* Define to 1 if you want to use efr32 examples */
/* #undef OPENTHREAD_EXAMPLES_EFR32 */

/* Define to 1 if you want to use emsk examples */
/* #undef OPENTHREAD_EXAMPLES_EMSK */

/* Define to 1 if you want to use gp712 examples */
/* #undef OPENTHREAD_EXAMPLES_GP712 */

/* Define to 1 if you want to use kw41z examples */
/* #undef OPENTHREAD_EXAMPLES_KW41Z */

/* Define to 1 if you want to use nrf52811 examples */
/* #undef OPENTHREAD_EXAMPLES_NRF52811 */

/* Define to 1 if you want to use nrf52840 examples */
#define OPENTHREAD_EXAMPLES_NRF52840 1

/* Define to 1 if you want to use posix examples */
/* #undef OPENTHREAD_EXAMPLES_POSIX */

/* Define to 1 if you want to use qpg6095 examples */
/* #undef OPENTHREAD_EXAMPLES_QPG6095 */

/* Define to 1 if you want to use samr21 examples */
/* #undef OPENTHREAD_EXAMPLES_SAMR21 */

/* Define to 1 to build posix application. */
#define OPENTHREAD_PLATFORM_POSIX_APP 0

/* Name of package */
#define PACKAGE "openthread"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "openthread-devel@googlegroups.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "OPENTHREAD"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "OPENTHREAD 20180926-00632-g2279ef61"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "openthread"

/* Define to the home page for this package. */
#define PACKAGE_URL "http://github.com/openthread/openthread"

/* Define to the version of this package. */
#define PACKAGE_VERSION "20180926-00632-g2279ef61"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "20180926-00632-g2279ef61"

/* Define for Solaris 2.5.1 so the uint32_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT32_T */

/* Define for Solaris 2.5.1 so the uint64_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT64_T */

/* Define for Solaris 2.5.1 so the uint8_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT8_T */

/* Define to the type of a signed integer type of width exactly 16 bits if
   such a type exists and the standard includes do not define it. */
/* #undef int16_t */

/* Define to the type of a signed integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
/* #undef int32_t */

/* Define to the type of a signed integer type of width exactly 64 bits if
   such a type exists and the standard includes do not define it. */
/* #undef int64_t */

/* Define to the type of a signed integer type of width exactly 8 bits if such
   a type exists and the standard includes do not define it. */
/* #undef int8_t */

/* Define to the type of an unsigned integer type of width exactly 16 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint16_t */

/* Define to the type of an unsigned integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint32_t */

/* Define to the type of an unsigned integer type of width exactly 64 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint64_t */

/* Define to the type of an unsigned integer type of width exactly 8 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint8_t */
