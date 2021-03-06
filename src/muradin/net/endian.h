#ifndef MURADIN_NET_ENDIAN_H_
#define MURADIN_NET_ENDIAN_H_

#include <muradin/platform/predef.h>
#include <boost/cstdint.hpp>

namespace muradin{
	namespace net{

		// 主机字节序转网络字节序(16位整数)
		boost::uint16_t	uint16_hton(boost::uint16_t val);

		// 网络字节序转主机字节序(16位整数)
		boost::uint16_t	uint16_ntoh(boost::uint16_t val);

		// 主机字节序转网络字节序(32位整数)
		boost::uint32_t	uint32_hton(boost::uint32_t val);

		// 网络字节序转主机字节序(32位整数)
		boost::uint32_t	uint32_ntoh(boost::uint32_t val);

		// 主机字节序转网络字节序(64位整数)
		boost::uint64_t	uint64_hton(boost::uint32_t val);

		// 网络字节序转主机字节序(64位整数)
		boost::uint64_t	uint64_ntoh(boost::uint32_t val);

		inline boost::uint16_t	swap_2_bytes(boost::uint16_t val)
		{
			return (val<<8 | val>>8);
		}
		inline boost::uint32_t	swap_4_bytes(boost::uint32_t val)
		{
			return (val<<24 | (val<<8)&0x00FF0000 | (val>>8)&0x0000FF00 | val>>24 );
		}

		inline boost::uint64_t	swap_8_bytes(boost::uint64_t val)
		{
			val = ((val & 0x00000000000000FFULL) << 56) | 
				((val & 0x000000000000FF00ULL) << 40) | 
				((val & 0x0000000000FF0000ULL) << 24) | 
				((val & 0x00000000FF000000ULL) <<  8) | 
				((val & 0x000000FF00000000ULL) >>  8) | 
				((val & 0x0000FF0000000000ULL) >> 24) | 
				((val & 0x00FF000000000000ULL) >> 40) | 
				((val & 0xFF00000000000000ULL) >> 56);
			return val;
		}

		// 当前环境是否是大尾字节序( runtime check )
		inline bool		is_big_endianess()
		{
			boost::uint16_t n16=0x1234;
			return ((boost::uint8_t*)(&n16))[0] == 0x12 ;	
		}

		// 当前环境是否是小尾字节序( runtime check )
		inline bool		is_lit_endianess()
		{
			return !is_big_endianess();
		}

		// 当前环境是否是网络字节序( runtime check )
		inline bool		is_net_endianess()
		{
			return is_big_endianess();
		}


#if defined(PREDEF_OS_LINUX)
#  include <endian.h>
#elif defined(PREDEF_COMPILER_VISUALC)


		inline boost::uint16_t htobe16(boost::uint16_t val)
		{
			return is_big_endianess() ? val : swap_2_bytes(val);
		}

		inline boost::uint32_t htobe32(boost::uint32_t val)
		{
			return is_big_endianess() ? val : swap_4_bytes(val);
		}

		inline boost::uint64_t htobe64(boost::uint64_t val)
		{
			return is_big_endianess() ? val : swap_8_bytes(val);
		}

		inline boost::uint16_t htole16(boost::uint16_t val)
		{
			return  is_big_endianess() ? swap_2_bytes(val) : val;
		}

		inline boost::uint32_t htole32(boost::uint32_t val)
		{
			return  is_big_endianess() ? swap_4_bytes(val) : val;
		}

		inline boost::uint64_t htole64(boost::uint64_t val)
		{
			return is_big_endianess() ? swap_8_bytes(val) : val;
		}


		/// big-endian to native
		inline boost::uint16_t be16toh(boost::uint16_t val)
		{
			return htobe16(val);
		}
		inline boost::uint32_t be32toh(boost::uint32_t val)
		{
			return htobe32(val);
		}
		inline boost::uint64_t be64toh(boost::uint64_t val)
		{
			return htobe64(val);
		}
#endif
	}//net
}//muradin



#endif // MURADIN_NET_ENDIAN_H_
