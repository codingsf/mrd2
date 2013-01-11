
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
		/// С���ֽ���
		lite_endian,
		/// ����ֽ���(Ҳ�������ֽ���)
		big_endian
	};

	/// ��ǰϵͳ���ֽ���
	static bytebuffer::byteorder_t system_byteorder(){return k_system_byteorder;};

	/// �����ֽ���ת����ָ�����ֽ���
	/// �����ֽ������ض���ƽ̨��ʼ���ǹ̶���
	static uint16_t byteorder_value_s(bytebuffer::byteorder_t order, uint16_t val);

	/// �����ֽ���ת����ָ�����ֽ���
	/// �����ֽ������ض���ƽ̨��ʼ���ǹ̶���
	static uint32_t byteorder_value_l(bytebuffer::byteorder_t order, uint32_t val);

	/// �����ֽ���ת����ָ�����ֽ���
	/// �����ֽ������ض���ƽ̨��ʼ���ǹ̶���
	static uint64_t byteorder_value_ll(bytebuffer::byteorder_t order, uint64_t val);

	bytebuffer(byteorder_t byteorder = bytebuffer::lite_endian, size_t size=2048);


	/// �ı��ֽ�������,Ӱ���������ݶ�ȡ�ʹ洢�����Ĵ�����Ϊ
	bytebuffer& byteorder(bytebuffer::byteorder_t order){m_byteorder=order; return *this;};

	/// ���ص�ǰ�ֽ�������
	bytebuffer::byteorder_t byteorder()const{return m_byteorder;};

	/// �Ƿ�Ϊ��
	bool	empty()const{return m_rd_index == m_wt_index;};

	/// �������(����������)
	size_t	storage()const{return m_contianer.size();};

	/// �������(������������)
	size_t	space()const{return m_contianer.size()- m_resver_index;};

	/// ��������Ĵ�С
	static size_t	resver_size() {return m_resver_index;};



	/// ����bytebuffer
	/// ���ؿɶ�������
	size_t		append(const bytebuffer& another);

	/// ������������
	/// ���ؿɶ�������
	size_t		append(const void* data_ptr,size_t size);

	/// �����ַ���
	/// ���ؿɶ�������
	size_t		append_str(const char* sz_str);

	/// �ֶ����ӿɶ���������С
	/// ͨ�� wt_ptr() �ֶ�����memcopy ����Ҫ���ñ�����
	/// ���ؿɶ�������
	size_t		has_written(size_t bytes);

	/// ȷ����д���ݿռ�,���ܻ�����capacity ����
	void	ensure_write_space(size_t bytes);

	/// ����һЩδ��ȡ������
	/// ����ʣ��Ŀɶ�������
	size_t	discard(size_t bytes);

	/// ����дָ������
	/// ����ʣ��Ŀɶ�������
	size_t	rollback_write(size_t bytes);

	/// ��������δ��ȡ������
	void	discard_all();

	/// ���ô�С,���ܻ������ڴ�����·���,��ʧ����
	void	resize(size_t new_capacity);

	/// �ɶ���������
	size_t	readable_bytes()const;

	/// ��д��������
	size_t	writeable_bytes()const;

	/// �ɶ���(�Ѿ�����ȡ)��������
	size_t	discardable_bytes()const;

	/// ��ʱ��ָ��
	const char* rd_ptr()const;

	/// ��ʱдָ��
	const char* wt_ptr()const;

	/// ��ʱдָ��
	char* wt_ptr();


	/// ��������,resver_bytes ��ʾϣ��������ʣ��ռ�(�ֽ���)
	/// ִ�к�,����Ϊm_resver_index + readable_bytes() + resver_bytes
	void	shrink(size_t resver_bytes);

	/// ����һ��������н���
	void	swap(bytebuffer& another);


	/// ������������(��� byteorder_cov=true ���������ֽ���ת��)
	/// ���ؿɶ�������
	size_t		append_uint8(uint8_t val);

	/// ������������(��� byteorder_cov=true ���������ֽ���ת��)
	/// ���ؿɶ�������
	size_t		append_uint16(uint16_t val,bool byteorder_cov=true);

	/// ������������(��� byteorder_cov=true ���������ֽ���ת��)
	/// ���ؿɶ�������
	size_t		append_uint32(uint32_t val,bool byteorder_cov=true);

	/// ������������(��� byteorder_cov=true ���������ֽ���ת��)
	/// ���ؿɶ�������
	size_t		append_uint64(uint64_t val,bool byteorder_cov=true);

	/// ����������
	/// ���÷���Ҫ���ɶ������Ƿ��㹻��
	uint8_t		read_uint8()const;

	/// ����������(��� byteorder_cov=true ȡֵ���ֽ��������й�)
	/// ���÷���Ҫ���ɶ������Ƿ��㹻��
	uint16_t	read_uint16(bool byteorder_cov=true)const;

	/// ����������(��� byteorder_cov=true ȡֵ���ֽ��������й�)
	/// ���÷���Ҫ���ɶ������Ƿ��㹻��
	uint32_t	read_uint32(bool byteorder_cov=true)const;

	/// ����������(��� byteorder_cov=true ȡֵ���ֽ��������й�)
	/// ���÷���Ҫ���ɶ������Ƿ��㹻��
	uint64_t	read_uint64(bool byteorder_cov=true)const;


	/// ȡ����������
	/// ���÷���Ҫ���ɶ������Ƿ��㹻��
	uint8_t		retrieve_uint8(){uint8_t val =read_uint8(); discard(sizeof(uint8_t));return val; };

	/// ȡ����������(��� byteorder_cov=true ȡֵ���ֽ��������й�)
	/// ���÷���Ҫ���ɶ������Ƿ��㹻��
	uint16_t	retrieve_uint16(bool byteorder_cov=true){uint16_t val =read_uint16(byteorder_cov); discard(sizeof(uint16_t));return val; };

	/// ȡ����������(��� byteorder_cov=true ȡֵ���ֽ��������й�)
	/// ���÷���Ҫ���ɶ������Ƿ��㹻��
	uint32_t	retrieve_uint32(bool byteorder_cov=true){uint32_t val =read_uint32(byteorder_cov); discard(sizeof(uint32_t));return val; };

	/// ȡ����������(��� byteorder_cov=true ȡֵ���ֽ��������й�)
	/// ���÷���Ҫ���ɶ������Ƿ��㹻��
	uint64_t	retrieve_uint64(bool byteorder_cov=true){uint64_t val =read_uint64(byteorder_cov); discard(sizeof(uint64_t));return val; };


	/// ���ɶ����򵱳�stringȡ��
	std::string	retrieve_string(){std::string ret;size_t pos= find_char('\0'); if(pos !=0 ){ret=rd_ptr(); discard(pos);}return ret;} ;

	/// �ڿɶ������в���c��һ�γ��ֵ�����.
	/// ���ظ��ַ�����rd_ptr() �ľ���(�ֽ���,��1��ʼ)
	/// ���c������,����0
	/// ���c����,��ô
	/// ����д����ȷ�� :rd_ptr()[find_char('a') - 1] == 'a'
	size_t		find_char(char c)const;

	/// ���ɶ����򵱳�string����
	std::string	as_string() const;


protected:
	template <class T>
	T	read_integer()const{
		return *((T*)(rd_ptr()) );
	};

	char*	begin(){return &*m_contianer.begin();};
	const char*	begin()const{return &*m_contianer.begin();};

	/// ��ȡbytes �ֽ����ĵĿ��ÿռ�
	/// �����ǰ�����õĿռ���ڵ���bytes,��Ų������,�������������ڴ�
	/// �����ǰ�����õĿռ�С��bytes,�����������ڴ�
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