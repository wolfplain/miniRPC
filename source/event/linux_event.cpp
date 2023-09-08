#ifndef MAC
#include "event.h"
#include "common/log.h"
#include <unordered_map>
static std::unordered_map<EventType, int> g_eventMap = {
    {EventType::READ, EPOLLIN},
    {EventType::WRITE, EPOLLOUT},
    {EventType::RW, EPOLLRDHUP},
    {EventType::ET, EPOLLET},
    {EventType::HUP, EPOLLRDHUP},
};

static std::unordered_map<EventOpt, int> g_eventOptMap = {
    {EventOpt::ADD, EPOLL_CTL_ADD},
    {EventOpt::MOD, EPOLL_CTL_MOD},
    {EventOpt::DEL, EPOLL_CTL_DEL},
};

int Event::CreateEvent(int socketFd) {
    socketFd_ = socketFd;
    eventFd_ = epoll_create1(0);
    return eventFd_;
}

ErrorNo Event::SetEvents(int connectFd, EventType type, EventOpt optType) {
    struct epoll_event ev;
    ev.events = g_eventMap[type];   // EPOLLIN read; EPOLLOUT : write; EPOLLERR : error
    ev.data.fd = connectFd;

    int opType = g_eventOptMap[optType];  // EPOLL_CTL_ADD EPOLL_CTL_MOD EPOLL_CTL_DEL

    if (epoll_ctl(eventFd_, opType, connectFd, &ev) == -1) {
        LOG_E("epoll_ctl fail");
        return ErrorNo::FAILURE;
    }
    return ErrorNo::SUCCESS;
}

int Event::GetEventNumber() {
    // need set wait time
    return epoll_wait(eventFd_, events_, MAX_EVENTS, -1);
}

bool Event::IsReadEvent(int eventId) {
    if (eventId >= MAX_EVENTS) {
        return false;
    }
    return events_[eventId].events == EPOLLIN;
}

bool Event::IsWriteEvent(int eventId) {
    if (eventId >= MAX_EVENTS) {
        return false;
    }
    return events_[eventId].events == EPOLLOUT;
}

int Event::GetConnectFd(int eventId) {
    if (eventId >= MAX_EVENTS) {
        return -1;
    }
    return events_[eventId].data.fd;
}
#endif  // MAC