#ifndef MINIRPC_RPC_BUILDER_H
#define MINIRPC_RPC_BUILDER_H
#include "common/errorno.h"
#include "rpc/rpc_service.h"

#include "rapidjson/document.h"
#include <unordered_map>
#include <string>

class RpcBuilder {
public:
    static RpcBuilder &GetRpcBuilder() {
        static RpcBuilder instance;
        return instance;
    }
    ErrorNo RegisterService(RpcService *rpc);
    ErrorNo CheckService(const std::string &service) const {
        return (serviceLists_.count(service) > 0) ? ErrorNo::SUCCESS : ErrorNo::DATA_INVALID;
    }

    RpcService *operator[](const std::string &serviceName) {
        if (serviceLists_.count(serviceName) == 0) { return nullptr; }
        return serviceLists_[serviceName];
    }

    RpcBuilder(RpcBuilder &) = delete;
    RpcBuilder &operator=(RpcBuilder &) = delete;
    ~RpcBuilder() = default;

private:
    RpcBuilder() = default;
    std::unordered_map<std::string, RpcService *> serviceLists_;
};
#endif // MINIRPC_RPC_BUILDER_H