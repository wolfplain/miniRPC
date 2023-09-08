#include "rpc/rpc_builder.h"

ErrorNo RpcBuilder::RegisterService(RpcService *rpc)
{
    if (rpc == nullptr) {
        LOG_EX("invalid para");
        return ErrorNo::FAILURE;
    }
    std::string svName = rpc->GetServiceName();
    if (serviceLists_.count(svName) == 0) {
        LOG_EX("service ", svName, " already exist");
        return ErrorNo::FAILURE;
    }

    serviceLists_[svName] = rpc;
    return ErrorNo::SUCCESS;
}


ErrorNo RpcBuilder::CheckService(const std::string &service)
{
    return (serviceLists_.count(service) > 0) ? ErrorNo::SUCCESS : ErrorNo::DATA_INVALID;
}