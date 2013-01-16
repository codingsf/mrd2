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
		typedef std::map<SOCKET_FD,evt_channel*>		channel_map;
	public:
		poller_epoll();
		~poller_epoll();
	
		void	add_channel(evt_channel* channel);
		void	del_channel(evt_channel* channel);

		/// update events-code
		void	update_evt_code(evt_channel* channel);


		void	wait_for_evt(channel_list& active_channels,boost::uint32_t wait_timeout_ms);
	private:
		evt_channel*	find_channel(SOCKET_FD fd);
	private:
		enum  { kMaxPollEvt=256 };
		int				m_fd;
		channel_map		m_channel_map; /* save added channels */

	};
}
}

#endif//MURADIN_NET_EVTPOLLER_H__