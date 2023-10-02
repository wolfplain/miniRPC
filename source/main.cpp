/*
user can use it as dynamic library or as a standalone server
for dynamic library, user need start server by itself
for standalone server, user need compile a dynamic library and pass it to server to start service
like minirpc /absolute/path/to/user/dynamic/library 8081
*/
#include "common/log.h"
#include "server/rpc_server.h"

#include <dlfcn.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        LOG_E("input parameters error");
        return -1;
    }
    char *libName = argv[1];
    int port = std::stoi(argv[2], nullptr, 10);
    LOG_D("user library:", libName, " port:", port);

    char *error;
    void *handle = dlopen(libName, RTLD_LAZY);
    if (!handle) {
        LOG_E("error:", dlerror());
        return -1;
    }

    dlerror(); /* Clear any existing error */

    int (*RegisterService)();
    RegisterService = (int (*)())dlsym(handle, "RegisterService");

    if ((error = dlerror()) != NULL) {
        LOG_E("error:", error);
        return -1;
    }

    if ((*RegisterService)() == -1) {
        LOG_E("start service fail");
    }

    // create rpc server and start the server,
    RpcServer server(port); // 8081 : the server port
    server.Start();

    dlclose(handle);
    return 0;
}