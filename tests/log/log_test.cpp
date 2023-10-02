#include <gtest/gtest.h>
#include "common/log.h"

class TestLOG : public testing::Test {
public:
    void SetUp() {
    }
    void TearDown() {
    }
};

TEST_F(TestLOG, ControlV) {
    LOG log;
    log.Print(false, __FUNCTION__, __LINE__, "hello", 20230907);
    LOG_T("my log");
    LOG_T("my log", 20230901);
}
