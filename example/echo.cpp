#include <muradin/muradin.h>

#include <muradin/base/log_warper.h>

#include <iostream>

void on_conn(const muradin::net::connection_ptr& conn)
{
	//
	LOG_INFO.stream()  << " ***************** on_conn : connected = " << conn->connected() ;
}
void on_msg(const muradin::net::connection_ptr& conn,const muradin::base::sys_error&)
{
	//
	LOG_INFO.stream()   << " ***************** on_msg" ;
	conn->write( 
		muradin::net::buffer(conn->read_buffer().rd_ptr(),conn->read_buffer().readable_bytes() )
		);
	conn->read_buffer().discard_all();
}
void on_msg_complete(const muradin::net::connection_ptr& conn,const muradin::base::sys_error&,size_t bytes)
{
	//
	LOG_INFO.stream() << " ***************** on_msg_complete,bytes=" << bytes ;
}
void on_err(const muradin::net::connection_ptr& conn,const muradin::base::sys_error&)
{
	//
}
int main(int nagrv,char** argc)
{
	const char* bind_ip= "";
	unsigned short bind_port= 4001;
	muradin::net::io_service ios;
	muradin::net::tcp_server serv(bind_ip,bind_port,ios,1); 
	LOG_INFO.stream() <<" runing ";
	LOG_INFO.stream() <<bind_ip << " : " << bind_port ;
	serv.set_conn_callback(on_conn);
	serv.set_read_callback(on_msg);
	serv.set_write_callback(on_msg_complete);
	serv.set_error_callback(on_err);
	serv.ready();
	ios.run();
}

