#include <muradin/net/tcp_server.h>
#include <muradin/net/connection.h>
#include <muradin/net/io_service.h>
#include <muradin/net/net_address.h>

#include <iostream>

void on_conn(const muradin::net::connection_ptr& conn)
{
	//
	std::cout << __FUNCTION__  << " -> on_conn" <<std::endl;
}
void on_msg(const muradin::net::connection_ptr& conn)
{
	//
	std::cout  << __FUNCTION__ << " -> on_msg" <<std::endl;
}
void on_msg_complete(const muradin::net::connection_ptr& conn,size_t bytes)
{
	//
	std::cout << __FUNCTION__ << " -> on_msg_complete" <<std::endl;
}
int main(int nagrv,char** argc)
{
	const char* bind_ip= "127.0.0.1";
	unsigned short bind_port= 4001;
	muradin::net::io_service ios;
	muradin::net::tcp_server serv(bind_ip,bind_port,ios); 
	std::cout <<" runing "<<std::endl;
	std::cout <<bind_ip << " : " << bind_port <<std::endl;
	serv.set_conn_cb(on_conn);
	serv.set_msg_cb(on_msg);
	serv.set_msg_complete_cb(on_msg_complete);
	serv.start();
}
