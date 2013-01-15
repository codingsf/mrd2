#ifndef	MURADIN_NET_EVTCHANNLE_H__
#define	MURADIN_NET_EVTCHANNLE_H__


#include <muradin/net/socket_fd.h>
#include <muradin/net/io_service.h>

#include <boost/function.hpp>

namespace muradin {
	namespace net{
		class evt_channel{
		public:
			typedef	boost::function<void()>		evt_notify_functor;
		public:
			evt_channel(SOCKET_FD fd,io_service& ios);
			~evt_channel();
			
			void	join_to_service();
			void	remove_from_service();
			void	enable_read(bool enable);
			void	enable_write(bool enable);


			void	set_error_cb(const evt_notify_functor& func){m_error_cb = func;};
			void	set_close_cb(const evt_notify_functor& func){m_close_cb = func;};
			void	set_read_cb(const evt_notify_functor& func){m_read_cb = func;};
			void	set_write_cb(const evt_notify_functor& func){m_write_cb = func;};

			/// for evt-poller
			void	save_evt_status(int status){m_last_evt_status=status;};
			
			int		evt_status()const {return m_last_evt_status;};

			SOCKET_FD fd(){return m_fd;};
		private:
			void	process_work();
		private:
			SOCKET_FD m_fd;
			int		m_last_evt_status;
			io_service&	m_service;
			evt_notify_functor		m_error_cb;
			evt_notify_functor		m_close_cb;
			evt_notify_functor		m_read_cb;
			evt_notify_functor		m_write_cb;

		};
	}
}
#endif//MURADIN_NET_EVTCHANNLE_H__