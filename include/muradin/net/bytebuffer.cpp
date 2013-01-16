#include <muradin/net/bytebuffer.h>
#include <muradin/net/endian.h>


#include <assert.h>

#include <algorithm>


namespace muradin
{
namespace net
{

	/*
	// boost::uint8_t  must be 1 byte 
	MRD_STATIC_ASSERT(sizeof(boost::uint8_t)  == 1);
	// boost::uint16_t must be 2 bytes
	MRD_STATIC_ASSERT(sizeof(boost::uint16_t) == 2);
	// boost::uint32_t must be 4 bytes
	MRD_STATIC_ASSERT(sizeof(boost::uint32_t) == 4);
	// boost::uint64_t must be 8 bytes
	MRD_STATIC_ASSERT(sizeof(boost::uint64_t) == 8);
	*/


	const  bytebuffer::byteorder_t bytebuffer::k_system_byteorder= muradin::net::is_big_endianess()? bytebuffer::big_endian : bytebuffer::lite_endian;
	const  size_t bytebuffer::m_resver_index=8;

	boost::uint16_t bytebuffer::byteorder_value_s(bytebuffer::byteorder_t order, boost::uint16_t val)
	{
		if(bytebuffer::big_endian == order) return htobe16(val);
		else return htole16(val);
	}
	unsigned int bytebuffer::byteorder_value_l(bytebuffer::byteorder_t order, boost::uint32_t val)
	{
		if(bytebuffer::big_endian == order) return htobe32(val);
		else return htole32(val);
	}
	boost::uint64_t bytebuffer::byteorder_value_ll(bytebuffer::byteorder_t order, boost::uint64_t val)
	{
		if(bytebuffer::big_endian == order) return htobe64(val);
		else return htole64(val);
	}


	bytebuffer::bytebuffer(byteorder_t byteorder , size_t size)
		:m_byteorder(byteorder),
		//m_capacity(size+m_resver_index),
		m_contianer(size+m_resver_index),
		m_rd_index(m_resver_index),
		m_wt_index(m_resver_index)
	{
		//
	}



	unsigned char	bytebuffer::read_uint8()const
	{
		assert(readable_bytes() >= sizeof(boost::uint8_t) );
		return read_integer<unsigned char>() ;
	}

	boost::uint16_t	bytebuffer::read_uint16(bool byteorder_cov)const
	{
		assert(readable_bytes() >= sizeof(boost::uint16_t) );
		return byteorder_cov?byteorder_value_s(m_byteorder, read_integer<boost::uint16_t>()) : read_integer<boost::uint16_t>();
	}

	boost::uint32_t	bytebuffer::read_uint32(bool byteorder_cov)const
	{
		assert(readable_bytes() >= sizeof(boost::uint32_t) );
		return byteorder_cov?byteorder_value_l(m_byteorder,read_integer<boost::uint32_t>()) : read_integer<boost::uint32_t>();
	}

	boost::uint64_t	bytebuffer::read_uint64(bool byteorder_cov)const
	{
		assert(readable_bytes() >= sizeof(boost::uint64_t) );
		return byteorder_cov?byteorder_value_ll(m_byteorder,read_integer<boost::uint64_t>()) : read_integer<boost::uint64_t>();
	}


	size_t		bytebuffer::append_uint8(boost::uint8_t val)
	{
		return append(&val,sizeof(boost::uint8_t));
	}

	size_t		bytebuffer::append_uint16(boost::uint16_t val,bool byteorder_cov)
	{
		if(byteorder_cov) {
			if(bytebuffer::big_endian == m_byteorder) val= htobe16(val);
			else val= htole16(val);
		}
		return append(&val,sizeof(boost::uint16_t));
	}

	size_t		bytebuffer::append_uint32(boost::uint32_t val,bool byteorder_cov)
	{
		if(byteorder_cov) {
			if(bytebuffer::big_endian == m_byteorder) val= htobe32(val);
			else val= htole32(val);
		}
		return append(&val,sizeof(boost::uint32_t));
	}

	size_t		bytebuffer::append_uint64(boost::uint64_t val,bool byteorder_cov)
	{
		if(byteorder_cov) {
			if(bytebuffer::big_endian == m_byteorder) val= htobe64(val);
			else val= htole64(val);
		}
		return append(&val,sizeof(boost::uint64_t));
	}

	size_t		bytebuffer::append(const bytebuffer& another)
	{
		return append(another.rd_ptr(),another.readable_bytes());
	}

	size_t		bytebuffer::append(const void* data_ptr,size_t size)
	{
		//assert(writeable_bytes()>= size );
		ensure_write_space(size);
		const char* ptr=static_cast<const char*>(data_ptr);
		std::copy(ptr, ptr+size, wt_ptr());
		has_written(size);
		return readable_bytes();
	}

	size_t		bytebuffer::append_str(const char* sz_str)
	{
		return append(sz_str,strlen(sz_str));
	}

	size_t		bytebuffer::has_written(size_t bytes)
	{
		assert(storage()>= m_wt_index+bytes);
		m_wt_index+=bytes;
		return readable_bytes();
	}

	void	bytebuffer::ensure_write_space(size_t bytes)
	{
		make_space(bytes);
	}

	size_t	bytebuffer::discard(size_t bytes)
	{
		assert(readable_bytes() >= bytes);
		if(readable_bytes() <= bytes ) {
			discard_all();
		}else{
			m_rd_index+=bytes;
		}
		return readable_bytes();
	}

	size_t	bytebuffer::rollback_write(size_t bytes)
	{
		assert(readable_bytes() >= bytes);
		if(readable_bytes() <= bytes ) {
			discard_all();
		}else{
			m_wt_index-= bytes;
		}
		return readable_bytes();
	}

	void	bytebuffer::discard_all()
	{
		m_rd_index=m_resver_index;
		m_wt_index=m_resver_index;
	}

	void	bytebuffer::resize(size_t new_capacity)
	{
		m_contianer.resize(new_capacity+m_resver_index);
		//m_capacity=m_contianer.capacity();
	}

	size_t	bytebuffer::readable_bytes()const
	{
		assert(m_rd_index <= m_wt_index);
		return m_wt_index - m_rd_index;
	}

	size_t	bytebuffer::writeable_bytes()const
	{
		assert(m_wt_index <= m_contianer.capacity());
		return m_contianer.size() - m_wt_index;
	}

	size_t	bytebuffer::discardable_bytes()const
	{
		assert(m_rd_index >= m_resver_index);
		return m_rd_index - m_resver_index;
	}
	const char* bytebuffer::rd_ptr()const
	{
		return begin()+m_rd_index;
	}

	const char* bytebuffer::wt_ptr()const
	{
		return begin()+m_wt_index;
	}

	char* bytebuffer::wt_ptr()
	{
		return begin()+m_wt_index;
	}

	void	bytebuffer::swap(bytebuffer& another)
	{
		m_contianer.swap(another.m_contianer);
		std::swap(m_wt_index, another.m_wt_index);
		std::swap(m_rd_index, another.m_rd_index);
		std::swap(m_byteorder, another.m_byteorder);
		//std::swap(m_capacity, another.m_capacity);
	}
	void	bytebuffer::make_space(size_t bytes)
	{
		if(discardable_bytes() + writeable_bytes() < bytes){
			m_contianer.resize(bytes + m_wt_index);
			//m_capacity=m_contianer.capacity();
			/// move readable data ?
		}else{
			size_t readable = readable_bytes();
			std::copy(begin()+m_rd_index,
				begin()+m_wt_index,
				begin()+m_resver_index);
			m_rd_index = m_resver_index;
			m_wt_index = m_rd_index + readable;
			assert(readable == readable_bytes());
		}
	}
	void	bytebuffer::shrink(size_t resver_bytes)
	{
		size_t readable = readable_bytes();
		container_type buf(m_resver_index +readable+resver_bytes);
		std::copy(rd_ptr(), rd_ptr()+readable, buf.begin()+ m_resver_index);
		m_contianer.swap(buf);
		m_rd_index = m_resver_index;
		m_wt_index = m_rd_index + readable;
		//m_capacity = buf.capacity();
	}

	size_t		bytebuffer::find_char(char c)const
	{
		const char* ptr = std::find(rd_ptr(),wt_ptr(),c);
		if(ptr == wt_ptr()) return 0;
		return  ptr - rd_ptr() +1;
	}

	std::string	bytebuffer::as_string() const
	{
		return std::string(rd_ptr());
	}
}
}