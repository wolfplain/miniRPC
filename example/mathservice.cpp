#include "server/rpc_server.h"
#include "rpc/rpc_service.h"
#include "rpc/rpc_builder.h"


/*

for client you just connect to the server
and send message based on Json format, such as
{"svc" : "mathservice", "call" : "Add", "p1" : 1, "p2" : 2}

then just watch the response

*/
class MathService : public RpcService {
public:
    MathService(const std::string &svc) : RpcService(svc) {}
    ErrorNo Process(Request &req, Response &rsp) override {
        std::string svc;
        req.GetValue("svc", svc);

        std::string method;
        req.GetValue("call", method);
        LOG_D("svc:", svc, ",call", method);

        // based on the method type to choose the function to process the request
        if (method == "Add") {
            int p1;
            int p2;
            req.GetValue("p1", p1);
            req.GetValue("p2", p2);
            rsp.message = std::to_string(Add(p1, p2));
            LOG_D("rpc call return value:", rsp.message);
        }
        return ErrorNo::SUCCESS;
    }
    int Add(int a, int b) {
        return a + b;
    }
};

int RegisterService() {
    MathService mathservice("mathservice");  // mathservice : the name of service
    // register the service to service center
    ErrorNo err = RpcBuilder::GetRpcBuilder().RegisterService(&mathservice);
    if (err != ErrorNo::SUCCESS) {
        LOG_E("register service error");
        return -1;
    }
    return 0;
}

int main(int argn, char** argc) {
    if (RegisterService() != 0) {
        return -1;
    }
    // create rpc server and start the server, 
    RpcServer server(8081);  // 8081 : the server port
    server.Start();
    return StartService();
}
