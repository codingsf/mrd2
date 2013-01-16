#include <muradin/net/socket_ctl.h>
#include <unistd.h>
#include <fcntl.h>

namespace muradin{
namespace net{

	int socket_ctl::set_nonblcok(SOCKET_FD fd,bool nonblcok)
	{
		// non-block
		int flags = ::fcntl(fd, F_GETFL, 0);
		if(nonblcok) 
			flags |= O_NONBLOCK;
		else 
			flags &= O_NONBLOCK;
		return ::fcntl(fd, F_SETFL, flags);
	}

	int socket_ctl::set_cloexec(SOCKET_FD fd)
	{
		// close-on-exec
		int flags = ::fcntl(fd, F_GETFD, 0);
		flags |= FD_CLOEXEC;
		return ::fcntl(fd, F_SETFD, flags);
	}
}
}