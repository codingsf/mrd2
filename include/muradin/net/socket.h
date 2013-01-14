#ifndef	MURADIN_NET_SOCKET_H__
#define MURADIN_NET_SOCKET_H__

#include <muradin/net/socket_fd.h>
#include <muradin/net/net_address.h>

namespace muradin{
namespace net{

	class socket
	{
	public:

		/// TCP half-close status code
		enum tcp_close_flag{
			k_tcp_close_read	=0x00,	/* 0x00 close read          */
			k_tcp_close_writ	=0x01,	/* 0x01 close write         */
			k_tcp_close_both	=0x02	/* 0x02 close read write    */
		};
	public:
		socket(SOCKET_FD fd);
		/// will close FD
		~socket();
		SOCKET_FD	fd(){return m_fd;};

		/// create a non-blocking socket
		/// return socket FD if succsee
		/// return -1 if fail. check errno
		static	SOCKET_FD create();

		/// close socket
		static void	close(SOCKET_FD fd);

		/// shutdwon tcp
		/// return 0 if success
		/// return -1 if fail.check errno
		static int	shutdown(SOCKET_FD fd,tcp_close_flag how = k_tcp_close_writ);

		/// bind
		/// return 0 if success
		/// return -1 if fail.check errno
		static	int bind(SOCKET_FD fd,const endpoint_v4& addr);

		/// listen
		/// return 0 if success
		/// return -1 if fail.check errno
		static	int listen(SOCKET_FD fd,int backlog);

		/// accept
		/// return socket FD and set remote_addr if succsee
		/// return -1 if fail.check errno
		static	SOCKET_FD accpet(SOCKET_FD listen_fd,endpoint_v4& remote_addr);

		/// connect
		/// return 0 if succsee
		/// return -1 if fail.check errno
		static	int connect(SOCKET_FD fd,endpoint_v4& remote_addr);

	private:
		SOCKET_FD	m_fd;
	};
}
}

#endif//MURADIN_NET_SOCKET_H__
