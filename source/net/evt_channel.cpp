#include <muradin/net/evt_channel.h>

namespace muradin{
namespace net{

			evt_channle::evt_channle(SOCKET_FD fd,io_service& ios)
			:m_fd(fd),
			m_last_evt_status(0),
			m_service(ios),
			m_error_cb(NULL),
			m_close_cb(NULL),
			m_read_cb(NULL),
			m_write_cb(NULL)
			{
				//
			}
			evt_channle::~evt_channle()
			{
				//
			}
			void	evt_channle::join_to_service()
			{
				//
			}
			void	evt_channle::remove_from_service()
			{
				m_service
			}
			void	evt_channle::subscrib_read_evt()
			{
				//
			}
			void	evt_channle::subscrib_write_evt()
			{
				//
			}
			void	evt_channle::process_work()
			{
				switch(m_last_evt_status){
				case 0:
					if(m_error_cb) m_error_cb();
					break;
				case 1:
					if(m_close_cb) m_close_cb();
					break;
				case 2:
					if(m_read_cb) m_read_cb();
					break;
				case 3:
					if(m_write_cb) m_write_cb();
					break;
					default:
					//
				}
			}

}
}