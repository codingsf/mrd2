#ifndef		MURADIN_BASE_SYS_ERROR_H__	
#define 	MURADIN_BASE_SYS_ERROR_H__

#include <string>

namespace muradin{
namespace base{

	class sys_error
	{
	public:
		sys_error(int error_number,const std::string& msg);
		int		err_code()const{return m_error_number;};
		std::string what()const{return m_msg;};
	private:
		const int	m_error_number;
		const std::string	m_msg;
	};

}
}
#endif//MURADIN_BASE_SYS_ERROR_H__