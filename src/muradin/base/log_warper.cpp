#include  <muradin/base/log_warper.h>
#include  <muradin/base/timestamp.h>

#include <iostream>
#include <stdlib.h> // for abort()
#include <string.h> // for strrchr strerror
#include <stdio.h> // for strrchr strerror


#define		ENDLN 	("\r\n")

namespace muradin
{
namespace base
{

OutputFunc	g_out_put_func=NULL;
FlushFunc	g_flush_func=NULL;

static loging_level	g_log_level=ll_debug;
static const	std::string	g_crlf="\r\n";
static const	std::string g_str_spliter="|";
const	char*	LOG_LVL_NAME[ll_end_palce_]=
{"DEBUG ","INFO  ","WARN  ","ERROR ","FATAL "};


void default_writ_func(const char* msg, size_t len)
{
	std::cout<<msg;
}

void default_flush_func()
{
	std::cout.flush ();
}

std::string	file_base_name(const std::string& full_file_name)
{
	const char slash_1='\\';
	const char slash_2='/';

	std::string base_file(full_file_name);
	std::size_t pos = base_file.find_last_of(slash_1);
	if(pos == std::string::npos)
		pos = base_file.find_last_of(slash_2);

	if(pos != std::string::npos)
	{
		base_file.erase (0,pos+1);
	}
	return base_file;
}


class loger
{
public:

	virtual	~loger(void)
	{
		finish();
		if(log_levl_==ll_fatal_err) ::abort ();
	}

	explicit	loger(loging_level log_lvl):log_levl_(log_lvl)
	{
		prepare("","",-1);
	}

	loger(loging_level log_lvl,const std::string& source_file,const std::string& func_name,int line_number):log_levl_(log_lvl)
	{
		prepare(file_base_name(source_file),func_name,line_number);
	}

	loger(loging_level log_lvl,const std::string& source_file,const std::string& func_name,int line_number,int error_code):log_levl_(log_lvl)
	{
		prepare( file_base_name(source_file),func_name,line_number,error_code);
	}

	loger&		reset(loging_level log_lvl,const std::string& source_file,const std::string& func_name,int line_number)
	{
		ostream_.clear();
		ostream_.str ("");
		prepare(file_base_name(source_file),func_name,line_number);
		log_levl_= log_lvl;
		return *this;
	}
	void		prepare(const std::string& source_file,const std::string& func_name,int line_number)
	{
		ostream_<<timestamp::now ().to_formatted_string ()<< " ";
		ostream_<<LOG_LVL_NAME[log_levl_]<< " | ";
#if defined(ENABLE_LOG_FUNCTION_NAME)
		sprintf(log_src_info_,"%s %s:%d",func_name.c_str(), source_file.c_str(),line_number);
#else
		sprintf(log_src_info_,"%s:%d", source_file.c_str(),line_number);
#endif
	}
	void		prepare(const std::string& source_file,const std::string& func_name,int line_number,int error_no)
	{
		ostream_<<timestamp::now ().to_formatted_string ();
		ostream_<<LOG_LVL_NAME[log_levl_]<< " | ";
#if defined(ENABLE_LOG_FUNCTION_NAME)
		sprintf(log_src_info_,"%s %s:%d | errno : %d",func_name.c_str(), source_file.c_str(),line_number,error_no);
#else
		sprintf(log_src_info_,"%s:%d | errno : %d", source_file.c_str(),line_number,error_no);
#endif

	}
	virtual	void		finish()
	{
		ostream_<<" @ "<<log_src_info_<<ENDLN;
		std::string out = ostream_.str ();
		ostream_.clear();
		ostream_.str ("");
		if(log_levl_ >= g_log_level){
			if(g_out_put_func)
				g_out_put_func(out.c_str (),out.length ());
			else
				default_writ_func(out.c_str (),out.length ());
		}
	}

	virtual void		flush()
	{
		if(g_flush_func) g_flush_func();
		else default_flush_func ();
	}
	virtual	std::ostream&			get_ostream()
	{return ostream_;}

private:

private:
	std::ostringstream			ostream_;
	loging_level				log_levl_;
	char						log_src_info_[512];
};

typedef	loger	ostream_loger;


/************************************************************************/
/*                                                                      */
/************************************************************************/

template<class T_>
log_warper&	append_val(log_warper& where_i,const T_& val)
{
	where_i.get_impl ().get_ostream ()<<val;
	return where_i;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//log_warper::log_warper(void):impl_(new ostream_loger())
//{}

log_warper::~log_warper(void)
{
	if(impl_) delete impl_;
	impl_=NULL;
}

log_warper::log_warper(loging_level log_lvl):impl_(new ostream_loger(log_lvl))
{}
log_warper::log_warper(loging_level log_lvl,const std::string& source_file,const std::string& func_name,int line_number,int error_code)
:impl_(new ostream_loger(log_lvl,source_file,func_name,line_number,error_code))
{}

log_warper::log_warper(loging_level log_lvl,const std::string& source_file,const std::string& func_name,int line_number)
:impl_(new ostream_loger(log_lvl,source_file,func_name,line_number))
{
}

void		log_warper::finish()
{
	impl_->finish ();
}

void		log_warper::flush()
{
	impl_->flush ();
}

loger&			log_warper::get_impl()
{return	*impl_;}

std::ostream& log_warper::stream()
{return impl_->get_ostream ();}

loging_level log_warper::logLevel()
{
	return g_log_level;
}
void log_warper::setLogLevel(loging_level level)
{
	g_log_level = level;
}


void log_warper::setOutput(OutputFunc  func)
{
	g_out_put_func = func;
}
void log_warper::setFlush(FlushFunc  func)
{
	g_flush_func = func;
}
}//base
}//muradin
