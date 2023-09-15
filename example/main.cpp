#include "server/rpc_server.h"
#include "mathservice.h"

int main(int argn, char** argc) {
    if (RegisterService() != 0) {
        return -1;
    }
    // create rpc server and start the server, 
    RpcServer server(8081);  // 8081 : the server port
    server.Start();
    return 0;
}