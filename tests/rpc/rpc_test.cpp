#include <gtest/gtest.h>
#include "common/log.h"
#include "rpc/rpc_service.h"
#include "rpc/rpc_builder.h"
#include "server/rpc_server.h"


class TestRpc : public testing::Test {

};

class MathService : public RpcService {
public:
    MathService(const std::string &svc) : RpcService(svc) {}
    ErrorNo Process(Request &req, Response &rsp) override {
        std::string svc;
        req.GetValue("svc", svc);

        std::string method;
        req.GetValue("call", method);
        LOG_D("svc:", svc, ",call", method);
        if (method == "Add") {
            int p1;
            int p2;
            req.GetValue("p1", p1);
            req.GetValue("p2", p2);
            rsp.message = std::to_string(Add(p1, p2));
            LOG_D("rpc call return value:", rsp.message);
            EXPECT_EQ(3, Add(p1,p2));
        }
        return ErrorNo::SUCCESS;
    }
    int Add(int a, int b) {
        return a + b;
    }
};

TEST_F(TestRpc, MathAddcall) {
    // server
    MathService mathService("mathservice");
    EXPECT_EQ(ErrorNo::SUCCESS, RpcBuilder::GetRpcBuilder().RegisterService(&mathService));
    // {"svc" : "mathservice", "call" : "Add", "p1" : 1, "p2" : 2}
    const std::string json = "{ \"svc\" : \"mathservice\", \"call\" : \"Add\", \"p1\" : 1, \"p2\" : 2}";
    RpcServer server(8080);
    server.OnMessage(0, json);
}

TEST_F(TestRpc, StartService) {
    RpcServer server(8080);
    MathService mathService("mathservice");
    EXPECT_EQ(ErrorNo::SUCCESS, RpcBuilder::GetRpcBuilder().RegisterService(&mathService));
    EXPECT_EQ(server.Start(), ErrorNo::SUCCESS); // start the websocket
}

/* client
// class MathStub : public RpcClient {
// public:
//     ErrorNo CallAdd(Request &req, Response &rsp) {
//         req['call'] = "add";
//         req['userid'] = "test";
//         req['svc'] = "math"
           req["p1"] = 1
           req["p2"] = 2
//         return SendMsg(req, rsp);
//     }
// }

// class MathClient {
// public:
//     int Add(int a, int b) {
//         Request req;
//         req['funcId'] = 12;
//         req['a'] = 1;
//         req['b'] = 2;
//         Response rsp;
//         ErrorNo ret = stub->CallAdd(req, rsp);
//         if (ret != ErrorNo::SUCCESS) {
//             LOG_E("call rpc fail");
//             return -1;
//         }
//         LOG_D("add ans:", rsp['c']);
//         return rsp['c'];
//     }
// }
// MathClient mathClient;

// mathClient.Add(1, 2)
*/
