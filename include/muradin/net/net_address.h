#ifndef MURADIN_NET_NET_ADDRESS_H__
#define	MURADIN_NET_NET_ADDRESS_H__

#include <muradin/platform/predef.h>
/// for sockaddr_in
#if defined(PREDEF_OS_WINDOWS)
	#include <winsock.h>
	typedef  struct sockaddr_in	INET_ADDR;
#elif ( defined(PREDEF_OS_LINUX) || defined(PREDEF_PLATFORM_UNIX) )
	#include <netinet/in.h>
	typedef  struct sockaddr_in	INET_ADDR;
#endif

#include <string>

#include <boost/cstdint.hpp>

namespace muradin{
namespace net{
		
	class endpoint_v4
	{
	public:
		endpoint_v4();
		endpoint_v4(boost::uint16_t port);
		endpoint_v4(const std::string& ip);
		endpoint_v4(const std::string& ip,boost::uint16_t port);
		explicit endpoint_v4(const INET_ADDR& address );


		const INET_ADDR&	address()const;
		INET_ADDR&			address();
		const std::string			get_ip()const;
		boost::uint16_t				get_port()const;

		static std::string				address_any();
		static boost::uint16_t			port_any();
	private:
		enum {kPortAny=0};
		INET_ADDR		m_address;
	};
}
}
#endif//MURADIN_NET_NET_ADDRESS_H__
