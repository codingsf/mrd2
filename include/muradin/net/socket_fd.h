#ifndef MURADIN_NET_SOCKET_HANDLE_H__
#define MURADIN_NET_SOCKET_HANDLE_H__
#include <muradin/platform/predef.h>

namespace muradin{
namespace net{

#if defined(PREDEF_OS_WINDOWS)
	/// SOCKET handle on windows
	typedef		UINT_PTR		SOCKET_FD;	/* unsigned int  */
#elif ( defined(PREDEF_OS_LINUX) || defined(PREDEF_PLATFORM_UNIX) )
	/// SOCKET handle on linux/unix
	typedef		int				SOCKET_FD;
#endif
}
}
#endif//MURADIN_NET_SOCKET_HANDLE_H__