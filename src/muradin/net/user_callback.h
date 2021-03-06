/// 
/// 网络事件外部回调函数定义
///
///
#ifndef		MURADIN_NET_USER_CALL_BACK_H__	
#define 	MURADIN_NET_USER_CALL_BACK_H__

#include <muradin/base/sys_error.h>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace muradin{
namespace net{
	
	class connection;
	typedef boost::shared_ptr<connection> conn_ptr;


	/// internal use
	typedef boost::function<void(const conn_ptr&) > 					close_callback;
	
	/// 连接建立或者关闭回调
	typedef	boost::function<void(const conn_ptr&)>						connect_callback;
	/// 消息发送完成回调
	typedef boost::function<void(const conn_ptr&,const muradin::base::sys_error&,size_t)>				write_callback;
	/// 消息接收回调
	typedef boost::function<void(const conn_ptr&,const muradin::base::sys_error&)>						read_callback;
	/// 网络错误回调
	typedef boost::function<void(const conn_ptr&,const muradin::base::sys_error&)>						error_callback;
}
}
#endif//MURADIN_NET_USER_CALL_BACK_H__
