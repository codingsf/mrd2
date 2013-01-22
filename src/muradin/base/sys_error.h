#ifndef		MURADIN_BASE_SYS_ERROR_H__	
#define 	MURADIN_BASE_SYS_ERROR_H__

#include <string.h> //for strerror_r,strncpy


namespace muradin{
namespace base{

	class sys_error
	{
		enum  { err_success=0,max_msg_length=256 };
	public:
		sys_error(int error_number=err_success)
		:m_error_number(error_number)
		{
			/// strerror_r : GNU version
			char buff[max_msg_length];
			::strncpy(m_msg,
				::strerror_r(m_error_number,buff,sizeof(buff)),
				max_msg_length);
		}
		int			err_code()const
		{
			return m_error_number;
		}
		const char* what()const
		{
			return m_msg;
		}
		const bool  operator==(const sys_error& rh)const
		{
			return m_error_number == rh.m_error_number;
		}
		operator bool()const
		{
			return m_error_number != err_success;
		}

		static sys_error no_error()
		{
			return sys_error(err_success);
		}
	private:

		const int	m_error_number;
		char		m_msg[max_msg_length];
	};

}
}
#endif//MURADIN_BASE_SYS_ERROR_H__