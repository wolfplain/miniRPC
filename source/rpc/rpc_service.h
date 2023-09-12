#ifndef MINIRPC_RPC_SERVICE_H
#define MINIRPC_RPC_SERVICE_H
#include "common/errorno.h"
#include "common/log.h"

#include <type_traits>
#include <rapidjson/document.h>

class Request {
public:
    ErrorNo Parse(const std::string &message);

    template<typename V>
    ErrorNo GetValue(const char* key, V &value) {
        if (!doc.HasMember(key)) {
            LOG_E("doc doesn't have this ", key);
            return ErrorNo::INVALID_PARAM;
        }
        if constexpr(std::is_same<V, int>::value) {
            if (!doc[key].IsInt()) {
                LOG_E("type error");
                return ErrorNo::INVALID_PARAM;
            }
            value = doc[key].GetInt();
        }
        if constexpr(std::is_same<V, std::string>::value) {
            if (!doc[key].IsString()) {
                LOG_E("type error");
                return ErrorNo::INVALID_PARAM;
            }
            value = doc[key].GetString();
        }
        return ErrorNo::SUCCESS;
    }

private:
    rapidjson::Document doc;
};

struct Response {
    std::string message;
};


class RpcService {
public:
    virtual ErrorNo Process(Request &req, Response &rsp) = 0;
    std::string GetServiceName() const
    {
        return serviceName_;
    }

    RpcService(std::string serviceName) : serviceName_(serviceName) {}
private:
    std::string serviceName_;
};
#endif  // MINIRPC_RPC_SERVICE_H

