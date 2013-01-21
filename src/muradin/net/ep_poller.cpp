#include <muradin/net/ep_poller.h>
#include <muradin/base/log_warper.h>

#include <sys/epoll.h>

#include <errno.h>
#include <assert.h>

	

namespace muradin{
namespace net{

poller_epoll::poller_epoll()
:m_fd(::epoll_create(EPOLL_CLOEXEC))
{
	assert(m_fd != -1);
}
poller_epoll::~poller_epoll()
{
	::close(m_fd);
	m_fd=-1;
}
void			poller_epoll::assert_channel( evt_channel* channel,bool need_exist)
{
	//LOG_INFO.stream()<< "check channel :"<<channel->get_name();
	evt_channel* ptr=find_channel(channel->fd());
	if(need_exist && !ptr){
		LOG_EROR.stream()<< channel->get_name()<<" not exists";
	}
	if(!need_exist && ptr){
		LOG_EROR.stream()<< channel->get_name()<<" alreay exists";
	}
}
evt_channel*	poller_epoll::find_channel(SOCKET_FD fd)
{
	evt_channel* found_ptr=NULL;
	channel_map::iterator it=m_channel_map.find(fd);
	if(it != m_channel_map.end())
		found_ptr= it->second;
	return found_ptr;
}	

void	poller_epoll::add_channel(evt_channel* channel)
{
	assert_channel(channel,false);
	struct epoll_event evt;
	evt.events=channel->evt_status();
	evt.data.ptr=channel;

	int ret = ::epoll_ctl(m_fd,EPOLL_CTL_ADD,channel->fd(),&evt);
	if (ret == 0){
		// SUCCESS
		m_channel_map[channel->fd()] = channel;
	}else{
		// got error
		LOG_EROR.stream()<<"epoll_ctl fail,errno:"<< errno;
	}
}

void	poller_epoll::del_channel(evt_channel* channel)
{
	assert_channel(channel,true);

	int ret = ::epoll_ctl(m_fd,EPOLL_CTL_DEL,channel->fd(),NULL);
	if (ret == 0){
		// SUCCESS
		m_channel_map.erase(channel->fd());
	}else{
		// got error
		LOG_EROR.stream()<<"epoll_ctl fail,errno:"<< errno;
	}
}

void	poller_epoll::update_evt_code(evt_channel* channel)
{
	assert_channel(channel,true);

	struct epoll_event evt;
	evt.events=channel->evt_status();
	evt.data.ptr=channel;

	int ret = ::epoll_ctl(m_fd,EPOLL_CTL_MOD,channel->fd(),&evt);
	if (ret == 0){
		// SUCCESS
	}else{
		// got error
		LOG_EROR.stream()<<"epoll_ctl fail,errno:"<< errno;
	}

}

void	poller_epoll::wait_for_evt(channel_list& active_channels,boost::uint32_t wait_timeout_ms)
{
	struct epoll_event evts[kMaxPollEvt];
	::memset(&evts,0x0,sizeof(evts));
	int ret = ::epoll_wait(m_fd,evts,kMaxPollEvt,static_cast<int>(wait_timeout_ms));

	if (ret == 0){
		// got nothing. timeout,interrupted
	}else if(ret > 0){
		for (int i = 0; i < ret; ++i){
			evt_channel* channel = static_cast<evt_channel*>(evts[i].data.ptr);
			assert_channel(channel,true);
			channel->save_evt_status( evts[i].events );
			active_channels.push_back(channel);
		}
	}else{
		// got error
		LOG_EROR.stream()<<"epoll_wait fail,errno:"<< errno;
	}
}

}
}
