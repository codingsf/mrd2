#include <muraidn/net/tcp_server.h>
#include <muraidn/net/connection.h>
#include <muraidn/net/io_service.h>
#include <muradin/net/net_address.h>

#include <iostream>

void on_conn(connection_ptr conn)
{
	//
	std::cout << __FUNCTION__  << " -> on_conn" <<std::endl;
}
void on_msg(connection_ptr conn)
{
	//
	std::cout  << __FUNCTION__ << " -> on_msg" <<std::endl;
}
void on_msg_complete(connection_ptr conn,std::size_t bytes)
{
	//
	std::cout << __FUNCTION__ << " -> on_msg_complete" <<std::endl;
}
int main(char** argc,int nagrv)
{
	muradin::netio_service ios;
	muradin::nettcp_server serv("127.0.0.1",4001,ios); 
	serv.set_conn_cb(on_conn);
	serv.set_msg_cb(on_msg);
	serv.set_msg_complete_cb(on_msg_complete);
	srv.run();
}
