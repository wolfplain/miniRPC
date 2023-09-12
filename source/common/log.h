#ifndef MINIRPC_COMMON_LOG_H
#define MINIRPC_COMMON_LOG_H
#include <iostream>
#include <sstream>
#include <ostream>
enum LOG_LEVEL:int {
    DEBUG,
    ERROR,
    ALERT,
};

inline int g_logLevel = LOG_LEVEL::DEBUG;
inline void SetLogLevel(int level) {
    g_logLevel = level;
}

class LOG {
public:
    LOG(LOG_LEVEL level) : level_(level){};
    LOG() : level_(DEBUG) {};
    ~LOG() = default;
    template<typename ...Args>
    void Print(bool withComma, const std::string &funcName, int lineNo, const Args &...args) {
        if (level_ < g_logLevel) {
            return;
        }
        std::cout << "[DEBUG][" << funcName << "]["<< lineNo<<"] ";
        if (withComma) {
            PrintLog(args...);
        } else {
            PrintWithoutComma(args...);
        }
    }
    void PrintLog() {
        std::cout<<std::endl;
    }

    template<typename T, typename... Ts>
    void PrintLog(T arg, Ts... args) {
        if constexpr(sizeof...(args) > 0){
            std::cout << arg <<", ";
        } else {
            std::cout << arg << std::endl;
        }
        PrintLog(args...);
    }

    template<typename... Ts>
    void PrintWithoutComma(Ts... args){
        (std::cout << ... << args) << std::endl;
    }
private:
    int level_;
};

#define LOG_DX(args...) \
LOG(DEBUG).Print(true, __FUNCTION__, __LINE__, ##args)

#define LOG_EX(args...) \
LOG(ERROR).Print(true, __FUNCTION__, __LINE__, ##args)

#define LOG_T(args...) \
LOG(ERROR).Print(false, __FUNCTION__, __LINE__, ##args)

#define LOG_D(args...) \
LOG(DEBUG).Print(false, __FUNCTION__, __LINE__, ##args)

#define LOG_E(args...) \
LOG(ERROR).Print(false, __FUNCTION__, __LINE__, ##args)


// #define LOG_D(content) \
// std::cout << "[DEBUG][" << __FUNCTION__ << "]["<< __LINE__<<"] " << content << std::endl

// #define LOG_E(content) \
// std::cout << "[ERROR][" << __FUNCTION__ << "]["<< __LINE__<<"] " << content << std::endl

#endif  //  MINIRPC_COMMON_LOG_H