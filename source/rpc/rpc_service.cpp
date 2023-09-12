#include "rpc/rpc_service.h"

ErrorNo Request::Parse(const std::string &message) {
    doc.Parse(message.c_str());
    return ErrorNo::SUCCESS;
}
