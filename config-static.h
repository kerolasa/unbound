#include <config.h>

#if !defined(_GNU_SOURCE)
# define _GNU_SOURCE 1
#endif

#if !defined(_BSD_SOURCE)
# define _BSD_SOURCE 1
#endif

#if !defined(_DEFAULT_SOURCE)
# define _DEFAULT_SOURCE 1
#endif

#if !defined(__EXTENSIONS__)
# define __EXTENSIONS__ 1
#endif

#if !defined(_POSIX_C_SOURCE)
# define _POSIX_C_SOURCE 200112
#endif

#if !defined(_XOPEN_SOURCE)
# define _XOPEN_SOURCE 600
#endif

#if !defined(_XOPEN_SOURCE_EXTENDED)
# define _XOPEN_SOURCE_EXTENDED 1
#endif

#if !defined(_ALL_SOURCE)
# define _ALL_SOURCE 1
#endif

#if !defined(_LARGEFILE_SOURCE)
# define _LARGEFILE_SOURCE 1
#endif

#ifndef UNBOUND_DEBUG
# define NDEBUG
#endif

/** Use small-ldns codebase */
#define USE_SLDNS 1
#ifdef HAVE_SSL
# define LDNS_BUILD_CONFIG_HAVE_SSL 1
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#if STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#endif

#ifdef HAVE_STDARG_H
# include <stdarg.h>
#endif

#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif

#include <errno.h>

#if HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif

#ifdef HAVE_SYS_UIO_H
# include <sys/uio.h>
#endif

#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif

#ifdef HAVE_NETINET_TCP_H
# include <netinet/tcp.h>
#endif

#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif

#ifdef HAVE_WINSOCK2_H
# include <winsock2.h>
#endif

#ifdef HAVE_WS2TCPIP_H
# include <ws2tcpip.h>
#endif

#ifndef USE_WINSOCK
# define ARG_LL "%ll"
#else
# define ARG_LL "%I64"
#endif

#ifndef AF_LOCAL
# define AF_LOCAL AF_UNIX
#endif

#ifdef HAVE_ATTR_FORMAT
# define ATTR_FORMAT(archetype, string_index, first_to_check) \
    __attribute__ ((format (archetype, string_index, first_to_check)))
#else				/* !HAVE_ATTR_FORMAT */
# define ATTR_FORMAT(archetype, string_index, first_to_check)	/* empty */
#endif				/* !HAVE_ATTR_FORMAT */

#if defined(DOXYGEN)
# define ATTR_UNUSED(x)  x
#elif defined(__cplusplus)
# define ATTR_UNUSED(x)
#elif defined(HAVE_ATTR_UNUSED)
# define ATTR_UNUSED(x)  x __attribute__((unused))
#else				/* !HAVE_ATTR_UNUSED */
# define ATTR_UNUSED(x)  x
#endif				/* !HAVE_ATTR_UNUSED */

#ifndef HAVE_FSEEKO
# define fseeko fseek
# define ftello ftell
#endif				/* HAVE_FSEEKO */

#ifndef MAXHOSTNAMELEN
# define MAXHOSTNAMELEN 256
#endif

#if !defined(HAVE_SNPRINTF) || defined(SNPRINTF_RET_BROKEN)
# define snprintf snprintf_unbound
# define vsnprintf vsnprintf_unbound
# include <stdarg.h>
int snprintf(char *str, size_t count, const char *fmt, ...);
int vsnprintf(char *str, size_t count, const char *fmt, va_list arg);
#endif				/* HAVE_SNPRINTF or SNPRINTF_RET_BROKEN */

#ifndef HAVE_INET_PTON
# define inet_pton inet_pton_unbound
int inet_pton(int af, const char *src, void *dst);
#endif				/* HAVE_INET_PTON */

#ifndef HAVE_INET_NTOP
# define inet_ntop inet_ntop_unbound
const char *inet_ntop(int af, const void *src, char *dst, size_t size);
#endif

#ifndef HAVE_INET_ATON
# define inet_aton inet_aton_unbound
int inet_aton(const char *cp, struct in_addr *addr);
#endif

#ifndef HAVE_MEMMOVE
# define memmove memmove_unbound
void *memmove(void *dest, const void *src, size_t n);
#endif

#ifndef HAVE_STRLCAT
# define strlcat strlcat_unbound
size_t strlcat(char *dst, const char *src, size_t siz);
#endif

#ifndef HAVE_STRLCPY
# define strlcpy strlcpy_unbound
size_t strlcpy(char *dst, const char *src, size_t siz);
#endif

#ifndef HAVE_GMTIME_R
# define gmtime_r gmtime_r_unbound
struct tm *gmtime_r(const time_t *timep, struct tm *result);
#endif

#ifndef HAVE_REALLOCARRAY
# define reallocarray reallocarrayunbound
void *reallocarray(void *ptr, size_t nmemb, size_t size);
#endif

#if !defined(HAVE_SLEEP) || defined(HAVE_WINDOWS_H)
# define sleep(x) Sleep((x)*1000)	/* on win32 */
#endif				/* HAVE_SLEEP */

#ifndef HAVE_USLEEP
# define usleep(x) Sleep((x)/1000 + 1)	/* on win32 */
#endif				/* HAVE_USLEEP */

#ifndef HAVE_RANDOM
# define random rand		/* on win32, for tests only (bad random) */
#endif				/* HAVE_RANDOM */

#ifndef HAVE_SRANDOM
# define srandom(x) srand(x)	/* on win32, for tests only (bad random) */
#endif				/* HAVE_SRANDOM */

/* detect if we need to cast to unsigned int for FD_SET to avoid warnings */
#ifdef HAVE_WINSOCK2_H
# define FD_SET_T (u_int)
#else
# define FD_SET_T
#endif

#ifndef IPV6_MIN_MTU
# define IPV6_MIN_MTU 1280
#endif				/* IPV6_MIN_MTU */

#ifdef MEMCMP_IS_BROKEN
# include "compat/memcmp.h"
# define memcmp memcmp_unbound
int memcmp(const void *x, const void *y, size_t n);
#endif

#ifndef HAVE_CTIME_R
# define ctime_r unbound_ctime_r
char *ctime_r(const time_t *timep, char *buf);
#endif

#ifndef HAVE_STRSEP
# define strsep unbound_strsep
char *strsep(char **stringp, const char *delim);
#endif

#ifndef HAVE_ISBLANK
# define isblank unbound_isblank
int isblank(int c);
#endif

#if !defined(HAVE_STRPTIME) || !defined(STRPTIME_WORKS)
# define strptime unbound_strptime
struct tm;
char *strptime(const char *s, const char *format, struct tm *tm);
#endif

#ifdef HAVE_LIBRESSL
# if !HAVE_DECL_STRLCPY
size_t strlcpy(char *dst, const char *src, size_t siz);
# endif
# if !HAVE_DECL_STRLCAT
size_t strlcat(char *dst, const char *src, size_t siz);
# endif
# if !HAVE_DECL_ARC4RANDOM && defined(HAVE_ARC4RANDOM)
uint32_t arc4random(void);
# endif
# if !HAVE_DECL_ARC4RANDOM_UNIFORM && defined(HAVE_ARC4RANDOM_UNIFORM)
uint32_t arc4random_uniform(uint32_t upper_bound);
# endif
# if !HAVE_DECL_REALLOCARRAY
void *reallocarray(void *ptr, size_t nmemb, size_t size);
# endif
#endif				/* HAVE_LIBRESSL */
#ifndef HAVE_ARC4RANDOM
void explicit_bzero(void *buf, size_t len);
int getentropy(void *buf, size_t len);
uint32_t arc4random(void);
void arc4random_buf(void *buf, size_t n);
void _ARC4_LOCK(void);
void _ARC4_UNLOCK(void);
#endif
#ifndef HAVE_ARC4RANDOM_UNIFORM
uint32_t arc4random_uniform(uint32_t upper_bound);
#endif
#ifdef COMPAT_SHA512
# ifndef SHA512_DIGEST_LENGTH
#  define SHA512_BLOCK_LENGTH		128
#  define SHA512_DIGEST_LENGTH		64
#  define SHA512_DIGEST_STRING_LENGTH	(SHA512_DIGEST_LENGTH * 2 + 1)
typedef struct _SHA512_CTX {
	uint64_t state[8];
	uint64_t bitcount[2];
	uint8_t buffer[SHA512_BLOCK_LENGTH];
} SHA512_CTX;
# endif				/* SHA512_DIGEST_LENGTH */
void SHA512_Init(SHA512_CTX *);
void SHA512_Update(SHA512_CTX *, void *, size_t);
void SHA512_Final(uint8_t[SHA512_DIGEST_LENGTH], SHA512_CTX *);
unsigned char *SHA512(void *data, unsigned int data_len, unsigned char *digest);
#endif				/* COMPAT_SHA512 */

#if defined(HAVE_EVENT_H) && !defined(HAVE_EVENT_BASE_ONCE) && !(defined(HAVE_EV_LOOP) || defined(HAVE_EV_DEFAULT_LOOP)) && (defined(HAVE_PTHREAD) || defined(HAVE_SOLARIS_THREADS))
   /* using version of libevent that is not threadsafe. */
# define LIBEVENT_SIGNAL_PROBLEM 1
#endif

#ifndef CHECKED_INET6
# define CHECKED_INET6
# ifdef AF_INET6
#  define INET6
# else
#  define AF_INET6        28
# endif
#endif				/* CHECKED_INET6 */

#ifndef HAVE_GETADDRINFO
struct sockaddr_storage;

# include "compat/fake-rfc2553.h"
#endif

#ifdef UNBOUND_ALLOC_STATS
# define malloc(s) unbound_stat_malloc_log(s, __FILE__, __LINE__, __func__)
# define calloc(n,s) unbound_stat_calloc_log(n, s, __FILE__, __LINE__, __func__)
# define free(p) unbound_stat_free_log(p, __FILE__, __LINE__, __func__)
# define realloc(p,s) unbound_stat_realloc_log(p, s, __FILE__, __LINE__, __func__)
void *unbound_stat_malloc(size_t size);
void *unbound_stat_calloc(size_t nmemb, size_t size);
void unbound_stat_free(void *ptr);
void *unbound_stat_realloc(void *ptr, size_t size);
void *unbound_stat_malloc_log(size_t size, const char *file, int line,
			      const char *func);
void *unbound_stat_calloc_log(size_t nmemb, size_t size, const char *file,
			      int line, const char *func);
void unbound_stat_free_log(void *ptr, const char *file, int line,
			   const char *func);
void *unbound_stat_realloc_log(void *ptr, size_t size, const char *file,
			       int line, const char *func);
#elif defined(UNBOUND_ALLOC_LITE)
# include "util/alloc.h"
#endif				/* UNBOUND_ALLOC_LITE and UNBOUND_ALLOC_STATS */

/** default port for DNS traffic. */
#define UNBOUND_DNS_PORT 53
/** default port for unbound control traffic, registered port with IANA,
    ub-dns-control  8953/tcp    unbound dns nameserver control */
#define UNBOUND_CONTROL_PORT 8953
/** the version of unbound-control that this software implements */
#define UNBOUND_CONTROL_VERSION 1
