#ifdef MAC
#include "event.h"
#include "common/log.h"

int Event::CreateEvent(int socketFd) {

}

int Event::SetEvents(EventType type, EventOpt optType) {

}

int Event::GetEventNumber() {

}

bool Event::IsReadEvent(int connectFd) {

}

bool Event::IsWriteEvent(int connectFd) {

}
#endif  // MAC