#include <muradin/ep_poller.h>

#include <sys/epoll.h>

#include <errno.h>
#include <assert.h>

namespace{
	POLL_FD	create_epoll_fd()
	{
		return ::epoll_create(EPOLL_CLOEXEC);
	}
}

namespace muradin{
namespace net{

poller_epoll::poller_epoll()
:m_fd(create_epoll_fd())
{
	assert(m_fd != -1);
}
poller_epoll::~poller_epoll()
{
	::close(m_fd);
	m_fd=-1;
}

evt_channle*	poller_epoll::find_channel(SOCKET_FD fd)
{
	evt_channle* found_ptr=NULL;
	channel_map::iterator it=m_channel_map.find(fd);
	if(it != m_channel_map.end())
		found_ptr= &*it;
	return found_ptr;
}	

void	poller_epoll::add_channel(evt_channle* channel)
{
	assert( NULL == find_channel(channel.fd()));

	struct epoll_event evt;
	evt.events=channel->evt_status();
	evt.data.ptr=channel;

	int ret = ::epoll_ctl(m_fd,EPOLL_CTL_ADD,channel.fd(),&evt);
	if (ret == 0){
		// SUCCESS
		m_channel_map.insert(std::paire(channel.fd(),channel);
	}else{
		// got error
	}
}

void	poller_epoll::del_channle(evt_channle* channel)
{
	assert( NULL != find_channel(channel.fd()));

	int ret = ::epoll_ctl(m_fd,EPOLL_CTL_DEL,channel.fd(),NULL);
	if (ret == 0){
		// SUCCESS
		m_channel_map.earse(channel.fd());
	}else{
		// got error
	}
}

void	poller_epoll::update_evt_code(evt_channle* channel)
{
	assert( NULL != find_channel(channel.fd()));

	struct epoll_event evt;
	evt.events=channel->evt_status();
	evt.data.ptr=channel;

	int ret = ::epoll_ctl(m_fd,EPOLL_CTL_MOD,channel.fd(),&evt);
	if (ret == 0){
		// SUCCESS
	}else{
		// got error
	}

}

void	poller_epoll::wait_for_evt(channle_list& active_channels,boost::uint32_t wait_timeout_ms)
{
	struct epoll_event evts[kMaxPollEvt];
	::memset(&evts,0x0,sizeof(evts));
	int ret = epoll_wait(m_fd,&evts,kMaxPollEvt,static_cast<int>(wait_timeout_ms));

	if (ret == 0){
		// got nothing. timeout,interrupted
	}else(ret > 0){
		for (int i = 0; i < ret; ++i){
			evt_channle* channel = static_cast<evt_channle*>(evts[i].data.ptr);
			assert( NULL != find_channel(channel.fd()));
			channel.save_evt_status( evts[i].events );
			active_channels.push_back(channel);
		}
	}else{
		// got error
	}
}

}
}