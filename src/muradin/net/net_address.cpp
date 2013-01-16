#include <muradin/net/net_address.h>
#include <muradin/net/endian.h>
#include <arpa/inet.h>

#include <string.h>

namespace muradin{
namespace net{

		endpoint_v4::endpoint_v4(const std::string& ip,boost::uint16_t port)
		{
			m_address.sin_family=AF_INET;
			m_address.sin_port= uint16_hton(port);
			inet_pton(AF_INET,ip.c_str(),&m_address.sin_addr);
		}

		endpoint_v4::endpoint_v4(const std::string& ip)
		{
			m_address.sin_family=AF_INET;
			m_address.sin_port= uint16_hton(kPortAny);
			inet_pton(AF_INET,ip.c_str(),&m_address.sin_addr);
		}

		endpoint_v4::endpoint_v4(boost::uint16_t port)
		{
			m_address.sin_family=AF_INET;
			m_address.sin_port= uint16_hton(port);
			m_address.sin_addr.s_addr = INADDR_ANY;
		}

		endpoint_v4::endpoint_v4()
		{
			m_address.sin_family=AF_INET;
			m_address.sin_port= uint16_hton(kPortAny);
			m_address.sin_addr.s_addr= INADDR_ANY;
		}
		endpoint_v4::endpoint_v4(const INET_ADDR& address )
		{
			::memcpy(&m_address,&address,sizeof(INET_ADDR));
		}

	
		INET_ADDR&		endpoint_v4::address()
		{
			return m_address ;
		}
		const INET_ADDR&	endpoint_v4::address()const
		{
			return m_address ;
		}

		const std::string		endpoint_v4::get_ip()const
		{
			char addr_str[100]={0};
			inet_ntop(m_address.sin_family,&m_address.sin_addr,addr_str,sizeof(addr_str) );
			return std::string(addr_str);
		}

		boost::uint16_t			endpoint_v4::get_port()const
		{
			return uint16_ntoh(m_address.sin_port);
		}

		std::string				endpoint_v4::address_any()
		{
			char addr_str[100]={0};
			int32_t	ip_addr=INADDR_ANY;
			inet_ntop(AF_INET,&ip_addr,addr_str,sizeof(addr_str) );
			return std::string(addr_str);
		}

		boost::uint16_t				endpoint_v4::port_any()
		{
			return kPortAny;
		}

}//net
}//muradin
