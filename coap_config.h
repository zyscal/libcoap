
#ifndef COAP_CONFIG_H_
#define COAP_CONFIG_H_

/* Define to 1 if you have <ws2tcpip.h> header file. */
/* #undef HAVE_WS2TCPIP_H */

/* Define if the system has small stack size */
/* #undef COAP_CONSTRAINED_STACK */

/* Define to 1 if you have <winsock2.h> header file. */
/* #undef HAVE_WINSOCK2_H */

/* Define if the system has epoll support */
#define COAP_EPOLL_SUPPORT "1"

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H "1"

/* Define to 1 if you have the <assert.h> header file. */
/* #undef HAVE_ASSERT_H */

/* Define to 1 if you have the <dlfcn.h> header file. */
/* #undef HAVE_DLFCN_H */

/* Define to 1 if you have the `getaddrinfo' function. */
#define HAVE_GETADDRINFO "1"

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H "1"

/* Define if the system has openssl */
#define HAVE_OPENSSL "1"

/* Define if the system has libgnutls28 */
/* #undef HAVE_LIBGNUTLS */

/* Define if the system has libtinydtls */
/* #undef HAVE_LIBTINYDTLS */

/* Define if the system has libmbedtls */
/* #undef HAVE_MBEDTLS */

/* Define to 1 to build without TCP support. */
#define COAP_DISABLE_TCP 0

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H "1"

/* Define to 1 if you have the `malloc' function. */
#define HAVE_MALLOC "1"

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H "1"

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET "1"

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H "1"

/* Define to 1 if you have the <net/if.h> header file. */
#define HAVE_NET_IF_H "1"

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H "1"

/* Define to 1 if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H "1"

/* Define to 1 if you have the `pthread_mutex_lock' function. */
#define HAVE_PTHREAD_MUTEX_LOCK "1"

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT "1"

/* Define to 1 if you have the `socket' function. */
#define HAVE_SOCKET "1"

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H "1"

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H "1"

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP "1"

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H "1"

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H "1"

/* Define to 1 if you have the `strnlen' function. */
#define HAVE_STRNLEN "1"

/* Define to 1 if you have the `strrchr' function. */
#define HAVE_STRRCHR "1"

/* Define to 1 if the system has the type `struct cmsghdr'. */
/* #undef HAVE_STRUCT_CMSGHDR */

/* Define to 1 if you have the <syslog.h> header file. */
#define HAVE_SYSLOG_H "1"

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H "1"

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H "1"

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H "1"

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H "1"

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H "1"

/* Define to 1 if you have the <sys/unistd.h> header file. */
#define HAVE_SYS_UNISTD_H "1"

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H "1"

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H "1"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "libcoap-developers@lists.sourceforge.net"

/* Define to the full name of this package. */
#define PACKAGE_NAME "libcoap"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "libcoap 4.3.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "libcoap"

/* Define to the home page for this package. */
#define PACKAGE_URL "https://libcoap.net/"

/* Define to the version of this package. */
#define PACKAGE_VERSION "4.3.0"

#if defined(_MSC_VER) && (_MSC_VER < 1900) && !defined(snprintf)
#define snprintf _snprintf
#endif

#endif /* COAP_CONFIG_H_ */
