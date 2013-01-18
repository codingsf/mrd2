#include <muradin/net/tcp_server.h>
#include <muradin/net/connection.h>
#include <muradin/net/io_service.h>
#include <muradin/net/net_address.h>
#include <muradin/net/buffer.h>
#include <muradin/base/log_warper.h>

#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <iostream>

void on_conn(const muradin::net::connection_ptr& conn)
{
	//
	LOG_INFO.stream()  << " ***************** on_conn : connected = " << conn->connected() ;
}
void on_msg(const muradin::net::connection_ptr& conn)
{
	//
	LOG_INFO.stream()   << " ***************** on_msg" ;
	conn->write( 
		muradin::net::buffer(conn->read_buffer().rd_ptr(),conn->read_buffer().readable_bytes() )
		);
	conn->read_buffer().discard_all();
}
void on_msg_complete(const muradin::net::connection_ptr& conn,size_t bytes)
{
	//
	LOG_INFO.stream() << " ***************** on_msg_complete,bytes=" << bytes ;
}

void	run_srv(const char* ip,unsigned short port,muradin::net::io_service& ios)
{
	muradin::net::tcp_server serv(ip,port,ios); 
	serv.set_conn_cb(on_conn);
	serv.set_msg_cb(on_msg);
	serv.set_msg_complete_cb(on_msg_complete);

	serv.ready();
	muradin::net::io_service place_holer;
	place_holer.run();
}
int main(int nagrv,char** argc)
{
	const char* bind_ip= "";
	unsigned short bind_port= 4001;
	muradin::net::io_service ios;

	
	LOG_INFO.stream() <<" going to run ";
	LOG_INFO.stream() <<bind_ip << " : " << bind_port ;
	boost::thread run_thread( boost::bind(run_srv,bind_ip,bind_port,boost::ref(ios)) );
	LOG_INFO.stream() <<" running ";
	ios.run();
	run_thread.join();
	LOG_INFO.stream() <<" exit ";
}
