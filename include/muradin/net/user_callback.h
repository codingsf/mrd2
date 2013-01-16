/// 
/// 网络事件外部回调函数定义
///
///
#ifndef		MURADIN_NET_USER_CALL_BACK_H__	
#define 	MURADIN_NET_USER_CALL_BACK_H__

#include <boost/function.hpp>

#include <muradin/net/connection.h>
#include <muradin/base/sys_error.h>

namespace muradin{
namespace net{
	/// 连接建立或者关闭回调
	typedef	boost::function<void(connection_ptr)>						on_connect;
	/// 消息发送完成回调
	typedef boost::function<void(connection_ptr,std::size_t)>				on_msg_complete;
	/// 消息接收回调
	typedef boost::function<void(connection_ptr)>						on_msg;
	/// 网络错误回调
	typedef boost::function<void(muradin::base::sys_error&)>				on_error;
}
}
#endif//MURADIN_NET_USER_CALL_BACK_H__
