#ifndef MINIRPC_COMMON_EVENT_H
#define MINIRPC_COMMON_EVENT_H
#include "common/errorno.h"

#ifdef MAC

#else
#include <sys/epoll.h>
#endif  // MAC

enum class EventType {
    READ,
    WRITE,
    RW,
    ET,
    HUP,
};

enum class EventOpt {
    ADD,
    MOD,
    DEL,
};

const int MAX_EVENTS = 10;
class Event final {
public:
    int CreateEvent(int socketFd);
    ErrorNo SetEvents(int connectFd, EventType type, EventOpt optType);
    int GetEventNumber();
    bool IsReadEvent(int eventId);
    bool IsWriteEvent(int eventId);
    int GetConnectFd(int eventId);
private:
    int socketFd_ = -1;
    int eventFd_ = -1;
#ifdef MAC
    struct kevent events_[MAX_EVENTS] = {0};
#else
    struct epoll_event events_[MAX_EVENTS] = {0};
#endif  // MAC
};
#endif  // MINIRPC_COMMON_EVENT_H