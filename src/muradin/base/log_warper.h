#ifndef MURADIN_BASE_LOG_WAPPER_H__
#define MURADIN_BASE_LOG_WAPPER_H__

//#include <muradin/platform/predef.h>
#include <string>
#include <sstream>
#include <iomanip>


namespace muradin
{
namespace base{

class	loger;

enum	loging_level{
	ll_debug,
	ll_info,
	ll_warning,
	ll_error,
	ll_fatal_err,
	ll_end_palce_
};

typedef void (*OutputFunc)(const char* msg, size_t len);
typedef void (*FlushFunc)();



class log_warper
{
public:
	explicit	log_warper(loging_level log_lvl);
	log_warper(loging_level log_lvl,const std::string& source_file,const std::string& func_name,int line_number);
	log_warper(loging_level log_lvl,const std::string& source_file,const std::string& func_name,int line_number,int error_code);
	std::ostream&	stream();
	void		finish();
	void		flush();
	loger&		get_impl();
	~log_warper(void);

	static loging_level logLevel();
	static void setLogLevel(loging_level level);

	static void setOutput(OutputFunc func);
	static void setFlush(FlushFunc func);
private:
	log_warper(const log_warper&);
	log_warper&	operator=(const log_warper&);
private:
	loger*		impl_;
};


}//base
}//muradin

#define		LOG_DEBG					(muradin::base::log_warper(muradin::base::ll_debug,__FILE__,__FUNCTION__,__LINE__))
#define		LOG_INFO					(muradin::base::log_warper(muradin::base::ll_info,__FILE__,__FUNCTION__,__LINE__))
#define		LOG_WARN					(muradin::base::log_warper(muradin::base::ll_warning,__FILE__,__FUNCTION__,__LINE__))
#define		LOG_EROR					(muradin::base::log_warper(muradin::base::ll_error,__FILE__,__FUNCTION__,__LINE__))
// write log and call abort()
#define		LOG_FATL					(muradin::base::log_warper(muradin::base::ll_fatal_err,__FILE__,__FUNCTION__,__LINE__))

#define		ENDLN 	("\r\n")



#endif // MURADIN_BASE_LOG_WAPPER_H__
