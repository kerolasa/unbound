# Copyright 2018 NLnet Labs
# See the file LICENSE for the license

##########################################
### build system and compiler settings ###
##########################################
project('unbound', 'c',
	default_options : ['c_std=c99'],
	version : '1.7.0')
# Versions here are hardcoded.
libunbound_version = '2.5.6'

cc = meson.get_compiler('c')

c_flags = '''
	-O2
'''.split()
foreach arg : c_flags
	if cc.has_argument(arg)
		add_project_arguments(arg, language : 'c')
	endif
endforeach

##############################################
### external libraries search and config.h ###
##############################################
conf = configuration_data()
conf.set_quoted('PACKAGE_STRING', meson.project_name() + ' ' + meson.project_version())
conf.set_quoted('PACKAGE_VERSION', meson.project_version())
conf.set_quoted('PACKAGE_BUGREPORT', 'unbound-bugs@nlnetlabs.nl')

lib_ev = cc.find_library('ev')
bsd_dep = dependency('libbsd-overlay')
openssl_dep = dependency('openssl')
thread_dep = dependency('threads')

lib_crypto = get_option('crypto')
if lib_crypto == 'openssl'
	crypto_dep = dependency('openssl')
	conf.set('HAVE_SSL', 1, description : 'Define if you have the SSL libraries installed')
endif
if lib_crypto == 'nettle'
	crypto_dep = dependency('nettle')
	conf.set('HAVE_NETTLE', 1, description : 'Use libnettle for crypto')
	foreach h : [
		'nettle/dsa-compat.h',
		'nettle/eddsa.h',
	]
		if cc.has_header(h)
			conf.set('HAVE_' + h.to_upper().underscorify(), 1,
				description : 'Define to 1 if you have the <' + h + '> header file')
		endif
	endforeach
endif
if lib_crypto == 'nss'
	crypto_dep = dependency('nss')
	conf.set('HAVE_NSS', 1, description : 'Use libnss for crypto')
endif

if get_option('enable-libevent')
	event_dep = dependency('libevent')
	foreach f : [
		['event_base_free',	       '''#include <event2/event.h>'''],
		['event_base_new',	       '''#include <event2/event.h>'''],
		['event_base_once',	       '''#include <event2/event.h>'''],
		['event_base_get_method',      '''#include <event2/event.h>'''],
	]
		if cc.has_function(f[0], prefix : f[1], dependencies: event_dep)
			conf.set('HAVE_' + f[0].to_upper(), 1,
				description : 'Define to 1 if you have the ' + f[0] + ' function')
		endif
	endforeach
	h = 'event.h'
	if cc.has_header(h)
		conf.set('HAVE_' + h.to_upper().underscorify(), 1,
			description : 'Define to 1 if you have the <' + h + '> header file')
	endif
endif

if get_option('enable-libexpat')
	expat_dep = dependency('expat')
	h = 'expat.h'
	if cc.has_header(h)
		conf.set('HAVE_' + h.to_upper().underscorify(), 1,
			description : 'Define to 1 if you have the <' + h + '> header file')
	endif
endif

if get_option('enable-libfstrm')
	libfstrm_dep = dependency('libfstrm')
endif

if get_option('enable-libsodium')
	libsodium_dep = dependency('libsodium')
endif

if get_option('enable-protobuf-c')
	protobuf_c_dep = dependency('libprotobuf-c')
endif

conf.set('_POSIX_PTHREAD_SEMANTICS', 1, description : 'Enable threading extensions on Solaris')
conf.set('SIZEOF_TIME_T', cc.sizeof('time_t', prefix : '#include <sys/time.h>'))
conf.set('MAXSYSLOGMSGLEN', 10240, description : 'Define to the maximum message length to pass to syslog')

if host_machine.system() == 'darwin'
	# FIXME: add setreuid() brokenness check, and make conf.set()
	# conditional to results
	conf.set('DARWIN_BROKEN_SETREUID', 1, description : 'Define this if on macOSX10.4-darwin8 and setreuid and setregid do not work')
endif

if get_option('enable-dsa')
	conf.set('USE_DSA', 1, description : 'Define this to enable DSA support')
endif
if get_option('enable-ecdsa')
	conf.set('USE_ECDSA', 1, description : 'Define this to enable ECDSA support')
endif
if get_option('enable-ed25519')
	conf.set('USE_ED25519', 1, description : 'Define this to enable ED25519 support')
endif
if get_option('enable-gost')
	conf.set('USE_GOST', 1, description : 'Define this to enable GOST support')
endif
if get_option('enable-sha1')
	conf.set('USE_SHA1', 1, description : 'Define this to enable SHA1 support')
endif
if get_option('enable-sha2')
	conf.set('USE_SHA2', 1, description : 'Define this to enable SHA2 support')
endif

if get_option('enable-alloc-checks')
	conf.set('UNBOUND_ALLOC_STATS', 1, description : 'use statistics for allocs and frees, for debug use')
endif
if get_option('enable-alloc-lite')
	conf.set('UNBOUND_ALLOC_LITE', 1, description : 'use to enable lightweight alloc assertions, for debug use')
endif
if get_option('enable-alloc-nonregional')
	conf.set('UNBOUND_ALLOC_NONREGIONAL', 1, description : 'use malloc not regions, for debug use')
endif

if get_option('enable-allsymbols')
	conf.set('EXPORT_ALL_SYMBOLS', 1, description : 'enabled-allsymbols from libunbound to link binaries')
endif
if get_option('enable-cachedb')
	conf.set('USE_CACHEDB', 1, description : 'Define to 1 to use cachedb support')
endif
if get_option('enable-dnscrypt')
	conf.set('USE_DNSCRYPT', 1, description : 'Define to 1 to enable dnscrypt support')
endif
if get_option('enable-dnstap')
	conf.set('USE_DNSTAP', 1, description : 'Define to 1 to enable dnstap support')
endif
# FIXME
#if get_option('enable-event-api')
#	build executable()?
#endif
if get_option('enable-ipsecmod')
	conf.set('USE_IPSECMOD', 1, description : 'Define to 1 to use ipsecmod support')
endif
if get_option('enable-lock-checks')
	conf.set('ENABLE_LOCK_CHECKS', 1, description : 'Define if you want to use debug lock checking (slow)')
endif
if get_option('enable-subnet')
	conf.set('CLIENT_SUBNET', 1, description : 'Define this to enable client subnet option')
endif
if get_option('enable-systemd')
	conf.set('HAVE_SYSTEMD', 1, description : 'If set use systemd socket activation')
endif

# FIXME: write feature check
#enable-tfo-client
#enable-tfo-server
# FIXME: add the following to library() -> 'pic': True,
#with-pic

if get_option('with-pythonmodule')
	conf.set('WITH_PYTHONMODULE', 1, description : 'Define if you want Python module')
endif
if get_option('with-pyunbound')
	conf.set('WITH_PYUNBOUND', 1, description : 'Define if you want PyUnbound')
endif

path = join_paths(get_option('prefix'), get_option('with-chroot-dir'))
conf.set_quoted('CHROOT_DIR', path, description : 'Directory to chroot to')
path = join_paths(get_option('prefix'), get_option('with-conf-file'))
conf.set_quoted('CONFIGFILE', path, description : 'Pathname to the Unbound configuration file')
path = join_paths(get_option('prefix'), get_option('with-pidfile'))
conf.set_quoted('PIDFILE', path, description : 'Default pidfile location')
path = join_paths(get_option('prefix'), get_option('with-dnstap-socket-path'))
conf.set_quoted('DNSTAP_SOCKET_PATH', path, description : 'Default dnstap socket path')
path = join_paths(get_option('prefix'), get_option('with-rootcert-file'))
conf.set_quoted('ROOT_CERT_FILE', path, description : 'Default rootcert location')
path = join_paths(get_option('prefix'), get_option('with-rootkey-file'))
conf.set_quoted('ROOT_ANCHOR_FILE', path, description : 'Default rootkey location')
path = join_paths(get_option('prefix'), get_option('with-run-dir'))
conf.set_quoted('RUN_DIR', path, description : 'Directory to chdir to')
username = get_option('with-username')
conf.set_quoted('UB_USERNAME', username, description : 'default username')

# FIXME: if no NSS && ac_cv_func_arc4random no and getentropy() no and linux
#	conf.set('COMPAT_SHA512', 1, description : 'Do sha512 definitions in config.h')
#	+ defines + struct _SHA512_CTX

if cc.has_function('daemon', prefix : '''#include <unistd.h>''', args : '-D_DEFAULT_SOURCE') != true
	conf.set('DEPRECATED_DAEMON', 1, description : 'Whether daemon is deprecated')
endif

# libc
foreach f : [
	['inet_ntop',		       '''#include <arpa/inet.h>'''],
	['inet_pton',		       '''#include <arpa/inet.h>'''],
	['isblank',		       '''#include <ctype.h>'''],
	['fcntl',		       '''#include <fcntl.h>'''],
	['glob',		       '''#include <glob.h>'''],
	['endprotoent',		       '''#include <netdb.h>'''],
	['endservent',		       '''#include <netdb.h>'''],
	['_beginthreadex',	       '''#include <process.h>'''],
	['sigprocmask',		       '''#include <signal.h>'''],
	['fseeko',		       '''#include <stdio.h>'''],
	['snprintf',		       '''#include <stdio.h>'''],
	['random',		       '''#include <stdlib.h>'''],
	['srandom',		       '''#include <stdlib.h>'''],
	['reallocarray',	       '''#include <stdlib.h>'''],
	['memmove',		       '''#include <string.h>'''],
	['strsep',		       '''#include <string.h>'''],
	['getauxval',		       '''#include <sys/auxv.h>'''],
	['getrlimit',		       '''#include <sys/resource.h>'''],
	['setrlimit',		       '''#include <sys/resource.h>'''],
	['shmget',		       '''#include <sys/shm.h>'''],
	['recvmsg',		       '''#include <sys/socket.h>'''],
	['sendmsg',		       '''#include <sys/socket.h>'''],
	['socketpair',		       '''#include <sys/socket.h>'''],
	['inet_aton',		       '''#include <sys/socket.h>
					  #include <netinet/in.h>
					  #include <arpa/inet.h>'''],
	['endpwent',		       '''#include <sys/types.h>
					  #include <pwd.h>'''],
	['fork',		       '''#include <sys/types.h>
					  #include <unistd.h>'''],
	['getpwnam',		       '''#include <sys/types.h>
					  #include <pwd.h>'''],
	['initgroups',		       '''#include <sys/types.h>
					  #include <grp.h>'''],
	['getaddrinfo',                '''#include <sys/types.h>
					  #include <sys/socket.h>
					  #include <netdb.h>'''],
	['kill',		       '''#include <sys/types.h>
					  #include <signal.h>'''],
	['setsid',		       '''#include <sys/types.h>
					  #include <unistd.h>'''],
	['vfork',		       '''#include <sys/types.h>
					  #include <unistd.h>'''],
	['setusercontext',	       '''#include <sys/types.h>
					  #include <login_cap.h>'''],
	['writev',		       '''#include <sys/uio.h>'''],
	['ctime_r',		       '''#include <time.h>'''],
	['gmtime_r',		       '''#include <time.h>'''],
	['localtime_r',		       '''#include <time.h>'''],
	['strftime',		       '''#include <time.h>'''],
	['strptime',		       '''#include <time.h>'''],
	['tzset',		       '''#include <time.h>'''],
	['chown',		       '''#include <unistd.h>'''],
	['chroot',		       '''#include <unistd.h>'''],
	['daemon',		       '''#include <unistd.h>'''],
	['fsync',		       '''#include <unistd.h>'''],
	['getentropy',		       '''#include <unistd.h>'''],
	['setregid',		       '''#include <unistd.h>'''],
	['setresgid',		       '''#include <unistd.h>'''],
	['setresuid',		       '''#include <unistd.h>'''],
	['setreuid',		       '''#include <unistd.h>'''],
	['sleep',		       '''#include <unistd.h>'''],
	['usleep',		       '''#include <unistd.h>'''],
]
	if cc.has_function(f[0], prefix : f[1], args : '-D_GNU_SOURCE')
		conf.set('HAVE_' + f[0].to_upper(), 1,
			description : 'Define to 1 if you have the ' + f[0] + ' function')
	endif
endforeach

# libbsd
foreach f : [
	['arc4random',		       '''#include <stdlib.h>'''],
	['arc4random_uniform',	       '''#include <stdlib.h>'''],
	['strlcat',		       '''#include <string.h>'''],
	['strlcpy',		       '''#include <string.h>'''],
]
	if cc.has_function(f[0], dependencies: bsd_dep)
		conf.set('HAVE_' + f[0].to_upper(), 1,
			description : 'Define to 1 if you have the ' + f[0] + ' function')
	endif
endforeach

# libev
foreach f : [
	['ev_loop',		       '''#include <ev.h>'''],
	['ev_default_loop',	       '''#include <ev.h>'''],
]
	if cc.has_function(f[0], prefix : f[1], dependencies: lib_ev)
		conf.set('HAVE_' + f[0].to_upper(), 1,
			description : 'Define to 1 if you have the ' + f[0] + ' function')
	endif
endforeach

# openssl
foreach f : [
	['OPENSSL_config',	       '''#include <openssl/conf.h>'''],
	['FIPS_mode',		       '''#include <openssl/crypto.h>'''],
	['OPENSSL_init_crypto',	       '''#include <openssl/crypto.h>'''],
	['CRYPTO_cleanup_all_ex_data', '''#include <openssl/crypto.h>'''],
	['DSA_SIG_set0',	       '''#include <openssl/dsa.h>'''],
	['ERR_free_strings',	       '''#include <openssl/err.h>'''],
	['ERR_load_crypto_strings',    '''#include <openssl/err.h>'''],
	['EVP_cleanup',		       '''#include <openssl/evp.h>'''],
	['EVP_dss1',		       '''#include <openssl/evp.h>'''],
	['EVP_sha1',		       '''#include <openssl/evp.h>'''],
	['EVP_sha256',		       '''#include <openssl/evp.h>'''],
	['EVP_sha512',		       '''#include <openssl/evp.h>'''],
	['EVP_DigestVerify',	       '''#include <openssl/evp.h>'''],
	['EVP_MD_CTX_new',	       '''#include <openssl/evp.h>'''],
	['OpenSSL_add_all_digests',    '''#include <openssl/evp.h>'''],
	['HMAC_Update',		       '''#include <openssl/hmac.h>'''],
	['RAND_cleanup',	       '''#include <openssl/rand.h>'''],
	['SHA512_Update',	       '''#include <openssl/sha.h>'''],
	['OPENSSL_init_ssl',	       '''#include <openssl/ssl.h>'''],
	['SSL_CTX_set_security_level', '''#include <openssl/ssl.h>'''],
]
	if cc.has_function(f[0], prefix : f[1], dependencies: openssl_dep, args : '-D_GNU_SOURCE')
		conf.set('HAVE_' + f[0].to_upper(), 1,
			description : 'Define to 1 if you have the ' + f[0] + ' function')
	endif
endforeach

foreach h : [
	'arpa/inet.h',
	'dlfcn.h',
	'endian.h',
	'getopt.h',
	'glob.h',
	'grp.h',
	'inttypes.h',
	'iphlpapi.h',
	'login_cap.h',
	'memory.h',
	'netdb.h',
	'netinet/in.h',
	'netinet/tcp.h',
	'nettle/dsa-compat.h',
	'nettle/eddsa.h',
	'openssl/bn.h',
	'openssl/conf.h',
	'openssl/dh.h',
	'openssl/dsa.h',
	'openssl/engine.h',
	'openssl/err.h',
	'openssl/rand.h',
	'openssl/rsa.h',
	'openssl/ssl.h',
	'pwd.h',
	'stdarg.h',
	'stdbool.h',
	'stdint.h',
	'stdlib.h',
	'strings.h',
	'string.h',
	'syslog.h',
	'sys/ipc.h',
	'sys/param.h',
	'sys/resource.h',
	'sys/sha2.h',
	'sys/shm.h',
	'sys/socket.h',
	'sys/stat.h',
	'sys/sysctl.h',
	'sys/types.h',
	'sys/uio.h',
	'sys/un.h',
	'sys/wait.h',
	'time.h',
	'unistd.h',
	'vfork.h',
	'windows.h',
	'winsock2.h',
	'ws2tcpip.h',
]
	if cc.has_header(h)
		conf.set('HAVE_' + h.to_upper().underscorify(), 1,
			description : 'Define to 1 if you have the <' + h + '> header file')
	endif
endforeach

if cc.has_header('stdlib.h') and cc.has_header('stddef.h')
	conf.set('STDC_HEADERS', 1, description : 'Define to 1 if you have the ANSI C header files')
endif

check_function_attributes = [
	['HAVE_ATTR_FORMAT', 'format'],
	['HAVE_ATTR_UNUSED', 'unused'],
	['HAVE_ATTR_WEAK', 'weak'],
]
foreach p : check_function_attributes
	attr = p.get(1)
	if cc.compiles('
		#include <stdlib.h>
		__attribute__((' + attr + ')) void x(void) {exit(1);}
		int main(void) {x(); return 0;}
	')
		conf.set(p.get(0), 1,
			description : 'Whether the C compiler accepts the "' + attr + '" attribute')
	endif
endforeach

config_h = configure_file(
	output : 'config.h',
	configuration : conf)
add_project_arguments('-include', 'config.h', '-include', 'config-static.h', language : 'c')

###############################
### source file collections ###
###############################
libunbound_sources = files('''
	cachedb/cachedb.c
	cachedb/cachedb.h
	config-static.h
	dns64/dns64.c
	dns64/dns64.h
	dnscrypt/cert.h
	dnscrypt/dnscrypt.h
	dnstap/dnstap.h
	edns-subnet/addrtree.c
	edns-subnet/addrtree.h
	edns-subnet/edns-subnet.c
	edns-subnet/edns-subnet.h
	edns-subnet/subnet-whitelist.c
	edns-subnet/subnetmod.c
	edns-subnet/subnetmod.h
	ipsecmod/ipsecmod-whitelist.c
	ipsecmod/ipsecmod.c
	ipsecmod/ipsecmod.h
	iterator/iter_delegpt.c
	iterator/iter_delegpt.h
	iterator/iter_donotq.c
	iterator/iter_donotq.h
	iterator/iter_fwd.c
	iterator/iter_fwd.h
	iterator/iter_hints.c
	iterator/iter_hints.h
	iterator/iter_priv.c
	iterator/iter_priv.h
	iterator/iter_resptype.c
	iterator/iter_resptype.h
	iterator/iter_scrub.c
	iterator/iter_scrub.h
	iterator/iter_utils.c
	iterator/iter_utils.h
	iterator/iterator.c
	iterator/iterator.h
	libunbound/context.c
	libunbound/context.h
	libunbound/libunbound.c
	libunbound/libworker.c
	libunbound/libworker.h
	libunbound/unbound-event.h
	libunbound/unbound.h
	libunbound/worker.h
	respip/respip.c
	respip/respip.h
	services/authzone.c
	services/authzone.h
	services/cache/dns.c
	services/cache/dns.h
	services/cache/infra.c
	services/cache/infra.h
	services/cache/rrset.c
	services/cache/rrset.h
	services/listen_dnsport.c
	services/listen_dnsport.h
	services/localzone.c
	services/localzone.h
	services/mesh.c
	services/mesh.h
	services/modstack.c
	services/modstack.h
	services/outbound_list.c
	services/outbound_list.h
	services/outside_network.c
	services/outside_network.h
	services/view.c
	services/view.h
	sldns/keyraw.c
	sldns/keyraw.h
	sldns/parse.c
	sldns/parse.h
	sldns/parseutil.c
	sldns/parseutil.h
	sldns/pkthdr.h
	sldns/rrdef.c
	sldns/rrdef.h
	sldns/sbuffer.c
	sldns/sbuffer.h
	sldns/str2wire.c
	sldns/str2wire.h
	sldns/wire2str.c
	sldns/wire2str.h
	testcode/checklocks.c
	util/alloc.c
	util/alloc.h
	util/as112.c
	util/as112.h
	util/config_file.c
	util/config_file.h
	util/configlexer.c
	util/configparser.c
	util/configyyrename.h
	util/data/dname.c
	util/data/dname.h
	util/data/msgencode.c
	util/data/msgencode.h
	util/data/msgparse.c
	util/data/msgparse.h
	util/data/msgreply.c
	util/data/msgreply.h
	util/data/packed_rrset.c
	util/data/packed_rrset.h
	util/fptr_wlist.c
	util/fptr_wlist.h
	util/locks.c
	util/locks.h
	util/log.c
	util/log.h
	util/mini_event.c
	util/mini_event.h
	util/module.c
	util/module.h
	util/net_help.c
	util/net_help.h
	util/netevent.c
	util/netevent.h
	util/random.c
	util/random.h
	util/rbtree.c
	util/rbtree.h
	util/regional.c
	util/regional.h
	util/rtt.c
	util/rtt.h
	util/storage/dnstree.c
	util/storage/dnstree.h
	util/storage/lookup3.c
	util/storage/lookup3.h
	util/storage/lruhash.c
	util/storage/lruhash.h
	util/storage/slabhash.c
	util/storage/slabhash.h
	util/timehist.c
	util/timehist.h
	util/tube.c
	util/tube.h
	util/ub_event.h
	util/ub_event_pluggable.c
	util/winsock_event.c
	validator/autotrust.c
	validator/autotrust.h
	validator/val_anchor.c
	validator/val_anchor.h
	validator/val_kcache.c
	validator/val_kcache.h
	validator/val_kentry.c
	validator/val_kentry.h
	validator/val_neg.c
	validator/val_neg.h
	validator/val_nsec.c
	validator/val_nsec.h
	validator/val_nsec3.c
	validator/val_nsec3.h
	validator/val_secalgo.c
	validator/val_secalgo.h
	validator/val_sigcrypt.c
	validator/val_sigcrypt.h
	validator/val_utils.c
	validator/val_utils.h
	validator/validator.c
	validator/validator.h
'''.split())
if conf.has('WITH_PYTHONMODULE')
	libunbound_sources += ['pythonmod/pythonmod_utils.c']
endif

ubanchor_sources = files('''
	compat/arc4_lock.c
	compat/arc4random.c
	compat/arc4random_uniform.c
	compat/chacha_private.h
	compat/explicit_bzero.c
	compat/strlcat.c
	compat/strlcpy.c
	compat/strptime.c
	sldns/parseutil.c
	sldns/parseutil.h
	smallapp/unbound-anchor.c
	util/locks.h
'''.split())

######################################################
### builds, the libunbound and various executables ###
######################################################
libunbound = library('unbound',
	libunbound_sources,
	dependencies : [
		bsd_dep,
		crypto_dep,
		event_dep,
		lib_ev,
		libfstrm_dep,
		protobuf_c_dep,
		thread_dep,
	],
	version : libunbound_version,
	install : true
)

executable('unbound-anchor', ubanchor_sources,
	dependencies : [
		bsd_dep,
		crypto_dep,
		expat_dep,
		lib_ev,
		libfstrm_dep,
		protobuf_c_dep,
		thread_dep,
	],
	link_with : libunbound,
	install : true)

# FIXME: build libunbound first, then these
#if get_option('enable-commands')
#	executable('unbound', link_depends : [libunbound], install : true)
#	executable('unbound-anchor', link_depends : [libunbound], install : true)
#	executable('unbound-checkconf', link_depends : [libunbound], install : true)
#	executable('unbound-control', link_depends : [libunbound], install : true)
#	executable('unbound-control-setup', link_depends : [libunbound], install : true)
#	executable('unbound-host', link_depends : [libunbound], install : true)
#endif

#############################################################
### manuals, conf examples, and other supplementary files ###
#############################################################

# FIXME
# Doxygen

####################################
### test command builds and runs ###
####################################

# FIXME

### EOF ###
