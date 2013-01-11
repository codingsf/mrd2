#ifndef MURADIN_NET_BUFFER_H__
#define MURADIN_NET_BUFFER_H__
namespace muradin{
namespace net{
	
	class buffer{
	public:
		buffer(void* data,size_t size);
		void*		data(){return m_ptr;};
		const void*	data()const{return m_ptr;};
		size_t		size()const{return m_size;};
	private:
		void*	m_ptr;
		size_t	m_size;
	};
}
}

#endif//MURADIN_NET_BUFFER_H__