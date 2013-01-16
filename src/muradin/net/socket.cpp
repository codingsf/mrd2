#include <muradin/net/socket.h>
#include <sys/types.h>    // for socklen_t
#include <sys/socket.h>
#include <muradin/net/socket_ctl.h>

namespace muradin{
namespace net{

	socket::socket(SOCKET_FD fd)
	:m_fd(fd)
	{
		//
	}
	socket::~socket()
	{
		socket::close(m_fd);
	}
	SOCKET_FD socket::create()
	{
		// need Linux kernel 2.6.27
		return ::socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,0);
	}

	void	socket::close(SOCKET_FD fd)
	{
		::close(fd);
	}

	int	socket::shutdown(SOCKET_FD fd,tcp_close_flag how)
	{
		return ::shutdown(fd,how);
	}

	int socket::bind(SOCKET_FD fd,const endpoint_v4& addr)
	{
		INET_ADDR addr_inet = addr.address();
		return ::bind(fd,(struct sockaddr*)&addr_inet,static_cast<socklen_t>( sizeof(INET_ADDR) ) );
	}

	int socket::listen(SOCKET_FD fd,int backlog)
	{
		return ::listen(fd,backlog);
	}

	SOCKET_FD socket::accept(SOCKET_FD listen_fd,endpoint_v4& remote_addr)
	{
		INET_ADDR* addr_inet = &remote_addr.address();
		socklen_t len= static_cast<socklen_t>(sizeof(INET_ADDR));

		// accpet4 need Linux kernel 2.6.28
		return ::accept4(listen_fd,(struct sockaddr*)addr_inet,&len, SOCK_NONBLOCK | SOCK_CLOEXEC);
		
		/*
		SOCKET_FD fd=::accept(listen_fd,addr_inet,len);
		if(fd != -1){
			int ret=socket_ctl::set_nonblcok(fd,true);
			ret= socket_ctl::set_cloexec(fd);

			/// FIXME:check ret
		}
		return fd;
		*/
	}

	int socket::connect(SOCKET_FD fd,endpoint_v4& remote_addr)
	{
		INET_ADDR addr_inet = remote_addr.address();
		return ::connect(fd,(struct sockaddr*)&addr_inet,static_cast<socklen_t>( sizeof(INET_ADDR) ) );
	}

}
}