#ifndef	MURADIN_NET_SOCKET_CTL_H__
#define MURADIN_NET_SOCKET_CTL_H__

#include <muradin/net/socket_fd.h>
#include <muradin/net/net_address.h>

namespace muradin{
namespace net{

	class socket_ctl
	{
	public:
		/// set NONBLOCK
		/// return 0 if succsee
		/// return -1 if fail. check errno
		static int set_nonblcok(SOCKET_FD fd,bool nonblcok);

		/// set CLOEXEC opt
		/// return 0 if succsee
		/// return -1 if fail. check errno
		static int set_cloexec(SOCKET_FD fd);

		/// set TCP_NODELAY
		/// return 0 if succsee
		/// return -1 if fail. check errno
		static int set_tcp_nodelay(SOCKET_FD fd,bool on);

		/// set SO_REUSEADDR
		/// return 0 if succsee
		/// return -1 if fail. check errno
		static int set_reuse_addr(SOCKET_FD fd,bool on);

	};
	
}
}
#endif//MURADIN_NET_SOCKET_CTL_H__