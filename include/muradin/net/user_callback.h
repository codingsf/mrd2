/// 
/// 网络事件外部回调函数定义
///
///
#ifndef		MURADIN_NET_USER_CALL_BACK_H__	
#define 	MURADIN_NET_USER_CALL_BACK_H__

#include <boost/cstdint.hpp>
#include <boost/function.hpp>

#include <muradin/net/connection.h>
#include <muraidn/net/buffer.h>
#include <muradin/base/sys_error.h>

namespace muradin{
namespace net{
	/// 连接建立回调
	typedef	boost::function<void(connection_ptr)>		on_connect;
	/// 消息发送完成回调
	typedef boost::function<void(std::size_t)>			on_msg_complete;
	/// 消息接收回调
	typedef boost::function<void(muradin::net::buffer&)>		on_msg;
	/// 网络错误回调
	typedef boost::function<void(muradin::base::sys_error&)>	on_error;
	/// 连接关闭
	typedef boost::function<void()>								on_close;
}
}
#endif//MURADIN_NET_USER_CALL_BACK_H__