#include <muradin/net/endian.h>
#include <muradin/config/predef.h>





namespace muradin{
namespace net{

	boost::uint16_t	uint16_hton(boost::uint16_t val)
	{
		return be16toh(val);
	}

	boost::uint16_t	uint16_ntoh(boost::uint16_t val)
	{
		return be16toh(val);
	}

	boost::uint32_t	uint32_hton(boost::uint32_t val)
	{
		return be32toh(val);
	}

	boost::uint32_t	uint32_ntoh(boost::uint32_t val)
	{
		return be32toh(val);
	}

	boost::uint64_t	uint64_hton(boost::uint32_t val)
	{
		return be64toh(val);
	}

	boost::uint64_t	uint64_ntoh(boost::uint32_t val)
	{
		return be64toh(val);
	}
}//net
}//muradin
