#ifndef	MURADIN_NET_EVTPOLLER_H__
#define MURADIN_NET_EVTPOLLER_H__

#include <muradin/net/evt_channel.h>
#include <muradin/net/socket_fd.h>

#include <vector>
#include <map>

namespace muradin{
namespace net{

	class poller_epoll
	{
		typedef std::vector<evt_channel*>				channel_list;
		typedef std::map<SOCKET_FD,evt_channle*>		channel_map;
		typedef int							POLL_FD;
	public:
		poller_epoll();
		~poller_epoll();
	
		void	add_channel(evt_channle* channel);
		void	del_channle(evt_channle* channel);

		/// update events-code
		void	update_evt_code(evt_channle* channel);


		void	wait_for_evt(channle_list& active_channels,boost::uint32_t wait_timeout_ms);
	private:
		evt_channle*	find_channel(SOCKET_FD fd);
	private:
		enum  { kMaxPollEvt=256 };
		POLL_FD			m_fd;
		channel_map		m_channel_map; /* save added channels */

	};
}
}

#endif//MURADIN_NET_EVTPOLLER_H__