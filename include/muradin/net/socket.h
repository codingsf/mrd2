#ifndef	MURADIN_NET_SOCKET_H__
#define MURADIN_NET_SOCKET_H__

#include <muradin/net/socket_fd.h>

namespace muradin{
namespace net{

	class socket
	{
	public::
		socket(SOCKET_FD fd);
		/// will close FD
		~socket();
		SOCKET_FD	fd(){return m_fd;};
	private:
		SOCKET_FD	m_fd;
	};
}
}
#endif
