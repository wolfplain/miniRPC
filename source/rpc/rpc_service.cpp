#include "rpc/rpc_service.h"

ErrorNo Request::Parse(const std::string &message) {
    if (doc.Parse(message.c_str()).HasParseError()) {
        LOG_E("invalid package");
        return ErrorNo::FAILURE;
    }
    return ErrorNo::SUCCESS;
}
