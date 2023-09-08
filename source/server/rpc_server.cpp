#include "server/rpcserver.h"
#include "common/log.h"

void RpcServer::OnStart(int connectFd)
{
    LOG_D("connectfd:",connectFd);
}

void RpcServer::OnMessage(int connectFd, const std::string &message)
{
    LOG_D("connectfd:", connectFd, "msg:", message);
    Request req;
    ErrorNo err = req.doc.Parse(message);
    if (err != ErrorNo::SUCCESS) {
        LOG_E("service parse fail");
        return;
    }

    RpcBuilder &builder = RpcBuilder::GetRpcBuilder();
    if (builder.CheckService(req.doc["svc"]) != ErrorNo::SUCCESS) {
        LOG_E("service not exist");
        return;
    }

    Response rsp;
    err = builder.[req.doc["svc"]].Process(req, rsp);
    if (err != ErrorNo::SUCCESS) {
        LOG_E("service process fail");
        return;
    }

    GetConnecter()[connectFd].SendResponse(rsp.message);
}

void RpcServer::OnClosed(int connectFd)
{
    LOG_D("connectfd:",connectFd);
}

void RpcServer::OnError(int connectFd, ErrorNo errorNo)
{
    LOG_D("connectfd:",connectFd,"errorno:",(int)(errorNo));
}
