#include <muradin/muradin.h>
#include <muradin/base/log_warper.h>
#include <muradin/base/blocking_counter.h>

#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <iostream>

muradin::base::blocking_counter g_counter(1);

void on_conn(const muradin::net::connection_ptr& conn)
{
	//
	//LOG_INFO.stream()  << " ***************** on_conn : connected = " << conn->connected() ;
}
void on_msg(const muradin::net::connection_ptr& conn,const muradin::base::sys_error& err)
{
	//
	//LOG_INFO.stream()   << " ***************** on_msg,size = "<<conn->read_buffer().readable_bytes() ;
	if (err){
		conn->shutdown();
		LOG_INFO.stream() << " get sys_error on read : err-code =" << err.err_code()<<",err msg = " << err.what();
	}else{
		conn->write( 
			muradin::net::buffer(conn->read_buffer().rd_ptr(),conn->read_buffer().readable_bytes() )
			);
		conn->read_buffer().discard_all();
	}
}
void on_msg_complete(const muradin::net::connection_ptr& conn,const muradin::base::sys_error& err,size_t bytes)
{
	//
	//LOG_INFO.stream() << " ***************** on_msg_complete,bytes=" << bytes ;
	if (err){
		conn->shutdown();
		//LOG_INFO.stream() << " get sys_error on write : err-code =" << err.err_code()<<",err msg = " << err.what();
	}
}
void on_err(const muradin::net::connection_ptr& conn,const muradin::base::sys_error& err)
{
	conn->shutdown();
	//LOG_INFO.stream() << " get sys_error on write : err-code =" << err.err_code()<<",err msg = " << err.what();
}
void	run_srv(const char* ip,unsigned short port,muradin::net::io_service& ios)
{
	muradin::net::tcp_server serv(ip,port,ios,2); 
	serv.set_conn_callback(on_conn);
	serv.set_read_callback(on_msg);
	serv.set_write_callback(on_msg_complete);
	serv.set_error_callback(on_err);

	serv.ready();
	LOG_INFO.stream() <<"####################### thread wait for exit ####################### ";
	g_counter.wait_for(0);
	LOG_INFO.stream() <<"####################### thread exit ####################### ";
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
