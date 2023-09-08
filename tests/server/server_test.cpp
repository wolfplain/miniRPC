#include <gtest/gtest.h>
#include "server/server.h"

using namespace Server;
class TestServer : public testing::Test {
public:
    void SetUp() {

    }
    void TearDown() {

    }
};

class TestWebSocketServer : public WebSocketServer {
public:
    void OnStart(int connectFd) {}
    void OnMessage(int connectFd, const std::string &message) {}
    void OnClosed(int connectFd) {}
    void OnError(int connectFd, ErrorNo errorNo) {}
};

TEST_F(TestServer, ServerStart) {
    TestWebSocketServer server;
    EXPECT_EQ(ErrorNo::SUCCESS, server.Start());
}
