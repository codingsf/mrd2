
/**
 * 
 * @file       byte_buffer.h 	
 * @author     cj@sunnysec.com.cn
 * @date       2012-10-30
 *  
 * 
 * @e Copyright sunnysec 2012. All right reserved
 */

#ifndef MURADIN_NET_BYTEBUFFER_H__
#define MURADIN_NET_BYTEBUFFER_H__
#include <muradin/config/integer_define.h>
#include <vector>
#include <string>

namespace muradin
{

namespace net
{

/** 
 * @class    bytebuffer
 *
 * @brief    A buffer like netty.ByteBuf.
 *
 * datastructure:
 *
 *     +---------------+-------------------+------------------+------------------+
 *     | reserve bytes | discardable bytes |  readable bytes  |  writable bytes  |
 *     |               |                   |     (CONTENT)    |                  |
 *     +---------------+-------------------+------------------+------------------+
 *     |               |                   |                  |                  |
 *      0         reserveIndex  <=      readerIndex   <=   writerIndex    <=    capacity
 *
 *  
 *
 * See netty/buffer/ByteBuf.java for more information. 
 *
 */
class bytebuffer
{
public:
	enum byteorder_t{
		/// 小端字节序
		lite_endian,
		/// 大端字节序(也叫网络字节序)
		big_endian
	};

	/// 当前系统的字节序
	static bytebuffer::byteorder_t system_byteorder(){return k_system_byteorder;};

	/// 本机字节序转换成指定的字节序
	/// 本机字节序在特定的平台上始终是固定的
	static uint16_t byteorder_value_s(bytebuffer::byteorder_t order, uint16_t val);

	/// 本机字节序转换成指定的字节序
	/// 本机字节序在特定的平台上始终是固定的
	static uint32_t byteorder_value_l(bytebuffer::byteorder_t order, uint32_t val);

	/// 本机字节序转换成指定的字节序
	/// 本机字节序在特定的平台上始终是固定的
	static uint64_t byteorder_value_ll(bytebuffer::byteorder_t order, uint64_t val);

	bytebuffer(byteorder_t byteorder = bytebuffer::lite_endian, size_t size=2048);


	/// 改变字节序设置,影响整型数据读取和存储函数的处理行为
	bytebuffer& byteorder(bytebuffer::byteorder_t order){m_byteorder=order; return *this;};

	/// 返回当前字节序设置
	bytebuffer::byteorder_t byteorder()const{return m_byteorder;};

	/// 是否为空
	bool	empty()const{return m_rd_index == m_wt_index;};

	/// 最大容量(含保留区域)
	size_t	storage()const{return m_contianer.size();};

	/// 最大容量(不含保留区域)
	size_t	space()const{return m_contianer.size()- m_resver_index;};

	/// 保留区域的大小
	static size_t	resver_size() {return m_resver_index;};



	/// 放入bytebuffer
	/// 返回可读数据量
	size_t		append(const bytebuffer& another);

	/// 放入任意数据
	/// 返回可读数据量
	size_t		append(const void* data_ptr,size_t size);

	/// 放入字符串
	/// 返回可读数据量
	size_t		append_str(const char* sz_str);

	/// 手动增加可读数据量大小
	/// 通过 wt_ptr() 手动进行memcopy 后需要调用本函数
	/// 返回可读数据量
	size_t		has_written(size_t bytes);

	/// 确保可写数据空间,可能会引起capacity 增大
	void	ensure_write_space(size_t bytes);

	/// 丢弃一些未读取的数据
	/// 返回剩余的可读数据量
	size_t	discard(size_t bytes);

	/// 回退写指针索引
	/// 返回剩余的可读数据量
	size_t	rollback_write(size_t bytes);

	/// 丢弃所有未读取的数据
	void	discard_all();

	/// 设置大小,可能会引起内存的重新分配,丢失数据
	void	resize(size_t new_capacity);

	/// 可读的数据量
	size_t	readable_bytes()const;

	/// 可写的数据量
	size_t	writeable_bytes()const;

	/// 可丢弃(已经被读取)的数据量
	size_t	discardable_bytes()const;

	/// 临时读指针
	const char* rd_ptr()const;

	/// 临时写指针
	const char* wt_ptr()const;

	/// 临时写指针
	char* wt_ptr();


	/// 缩减容量,resver_bytes 表示希望保留的剩余空间(字节数)
	/// 执行后,容量为m_resver_index + readable_bytes() + resver_bytes
	void	shrink(size_t resver_bytes);

	/// 与另一个对象进行交换
	void	swap(bytebuffer& another);


	/// 放入整型数据(如果 byteorder_cov=true 函数会做字节序转换)
	/// 返回可读数据量
	size_t		append_uint8(uint8_t val);

	/// 放入整型数据(如果 byteorder_cov=true 函数会做字节序转换)
	/// 返回可读数据量
	size_t		append_uint16(uint16_t val,bool byteorder_cov=true);

	/// 放入整型数据(如果 byteorder_cov=true 函数会做字节序转换)
	/// 返回可读数据量
	size_t		append_uint32(uint32_t val,bool byteorder_cov=true);

	/// 放入整型数据(如果 byteorder_cov=true 函数会做字节序转换)
	/// 返回可读数据量
	size_t		append_uint64(uint64_t val,bool byteorder_cov=true);

	/// 读整型数据
	/// 调用方需要检查可读数据是否足够足
	uint8_t		read_uint8()const;

	/// 读整型数据(如果 byteorder_cov=true 取值跟字节序设置有关)
	/// 调用方需要检查可读数据是否足够足
	uint16_t	read_uint16(bool byteorder_cov=true)const;

	/// 读整型数据(如果 byteorder_cov=true 取值跟字节序设置有关)
	/// 调用方需要检查可读数据是否足够足
	uint32_t	read_uint32(bool byteorder_cov=true)const;

	/// 读整型数据(如果 byteorder_cov=true 取值跟字节序设置有关)
	/// 调用方需要检查可读数据是否足够足
	uint64_t	read_uint64(bool byteorder_cov=true)const;


	/// 取出整型数据
	/// 调用方需要检查可读数据是否足够足
	uint8_t		retrieve_uint8(){uint8_t val =read_uint8(); discard(sizeof(uint8_t));return val; };

	/// 取出整型数据(如果 byteorder_cov=true 取值跟字节序设置有关)
	/// 调用方需要检查可读数据是否足够足
	uint16_t	retrieve_uint16(bool byteorder_cov=true){uint16_t val =read_uint16(byteorder_cov); discard(sizeof(uint16_t));return val; };

	/// 取出整型数据(如果 byteorder_cov=true 取值跟字节序设置有关)
	/// 调用方需要检查可读数据是否足够足
	uint32_t	retrieve_uint32(bool byteorder_cov=true){uint32_t val =read_uint32(byteorder_cov); discard(sizeof(uint32_t));return val; };

	/// 取出整型数据(如果 byteorder_cov=true 取值跟字节序设置有关)
	/// 调用方需要检查可读数据是否足够足
	uint64_t	retrieve_uint64(bool byteorder_cov=true){uint64_t val =read_uint64(byteorder_cov); discard(sizeof(uint64_t));return val; };


	/// 将可读区域当成string取出
	std::string	retrieve_string(){std::string ret;size_t pos= find_char('\0'); if(pos !=0 ){ret=rd_ptr(); discard(pos);}return ret;} ;

	/// 在可读区域中查找c第一次出现的索引.
	/// 返回该字符距离rd_ptr() 的距离(字节数,从1开始)
	/// 如果c不存在,返回0
	/// 如果c存在,那么
	/// 这样写是正确的 :rd_ptr()[find_char('a') - 1] == 'a'
	size_t		find_char(char c)const;

	/// 将可读区域当成string返回
	std::string	as_string() const;


protected:
	template <class T>
	T	read_integer()const{
		return *((T*)(rd_ptr()) );
	};

	char*	begin(){return &*m_contianer.begin();};
	const char*	begin()const{return &*m_contianer.begin();};

	/// 获取bytes 字节数的的可用空间
	/// 如果当前可利用的空间大于等于bytes,则挪动数据,而不重新申请内存
	/// 如果当前可利用的空间小于bytes,则重新申请内存
	void	make_space(size_t bytes);

private:
	typedef	std::vector<char>	container_type;
	static const bytebuffer::byteorder_t k_system_byteorder;
	static const size_t		m_resver_index;

	bytebuffer::byteorder_t m_byteorder;
	//size_t		m_capacity;
	container_type	m_contianer;
	size_t		m_rd_index;
	size_t		m_wt_index;
};


};//net

};//muradin

#endif