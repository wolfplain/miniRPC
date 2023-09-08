#include <gtest/gtest.h>
#include "common/log.h"
#include "rpc/rpc_service.h"
#include "rpc/rpc_builder.h"

class MathService : public RpcService, testing::Test {
public:
    ErrorNo Process(const Request &req, Response &rsp) {
        if (req["service"] != "math") {
            return ErrorNo::INVAILD;
        }
        if (req["funcid"] == "add") {
            int res = add(req["a"], req["b"]);
            rsp["ans"] = res;
            rsp["code"] = ErrorNo::SUCCESS;
        }
    }
    int Add(int a, int b) {
        return a + b;
    }
}

TEST_F(MathService, addcall) {
    // server
    MathService mathService;
    EXPECT_EQ(ErrorNo::SUCCESS, RpcBuilder::GetRpcBuilder().RegisterService(&mathService));
}


// // client
// class MathStub : public RpcClient {
// public:
//     ErrorNo CallAdd(Request &req, Response &rsp) {
//         req['funcid'] = "add";
//         req['userid'] = "test";
//         req['service'] = "math"
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
