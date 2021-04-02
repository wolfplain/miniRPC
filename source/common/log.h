#ifndef MINIRPC_COMMON_LOG_H
#define MINIRPC_COMMON_LOG_H
#include <iostream>
#define LOG_D(content) \
std::cout << "[DEBUG][" << __FUNCTION__ << "]["<< __LINE__<<"]" << content << std::endl

#define LOG_E(content) \
std::cout << "[ERROR][" << __FUNCTION__ << "]["<< __LINE__<<"]" << content << std::endl

#endif  //  MINIRPC_COMMON_LOG_H