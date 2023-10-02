#ifndef MINIRPC_COMMON_PAYLOAD_H
#define MINIRPC_COMMON_PAYLOAD_H
#include <string>
#include <unordered_map>
#include "common/errorno.h"
/**
 *   websocket data frame format
 0                   1                   2                   3
  0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 +-+-+-+-+-------+-+-------------+-------------------------------+
 |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
 |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
 |N|V|V|V|       |S|             |   (if payload len==126/127)   |
 | |1|2|3|       |K|             |                               |
 +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
 |     Extended payload length continued, if payload len == 127  |
 + - - - - - - - - - - - - - - - +-------------------------------+
 |                               |Masking-key, if MASK set to 1  |
 +-------------------------------+-------------------------------+
 | Masking-key (continued)       |          Payload Data         |
 +-------------------------------- - - - - - - - - - - - - - - - +
 :                     Payload Data continued ...                :
 + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
 |                     Payload Data continued ...                |
 +---------------------------------------------------------------+
*/
const std::string webSocketSecKey = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"; // GUID
constexpr int MIN_PKT_SIZE = 2;
constexpr int PAYLOAD_LEN_LIMIT126 = 126;
constexpr int PAYLOAD_LEN_LIMIT127 = 127;
constexpr int MASK_KEY_LEN = 4;
enum class OpCode {
    CONTINUE_FRAME = 0x0,
    TEXT_FRAME = 0x01,
    UNCONTROL_FRAME,
    DISCONNECT_FRAME = 0x08,
    PING_FRAME = 0x09,
    PONG_FRAME = 0x0A,
    CONTROL_FRAME,
};

struct Payload {
    uint8_t fin;
    uint8_t rsv;
    OpCode opcode;
    uint8_t mask;
    uint64_t payloadLen;
    uint8_t maskKey[MASK_KEY_LEN];
    std::string payload;
};

inline uint8_t FIN(uint8_t data) {
    return ((data & 0x080) >> 7);
}

inline uint8_t RSV(uint8_t data) {
    return ((data & 0x070) >> 4);
}

inline OpCode OPCODE(uint8_t data) {
    return static_cast<OpCode>(data & 0x0F);
}

inline uint8_t MASK(uint8_t data) {
    return ((data & 0x080) >> 7);
}

ErrorNo DecodeShakeHanderPkt(const std::string &pkt, std::unordered_map<std::string, std::string> &header);
ErrorNo EncodeShakeHanderPkt(const std::unordered_map<std::string, std::string> &header, std::string &dstData);
ErrorNo DecodeDataPkt(const std::string &pkt, Payload &payload);
ErrorNo EncodeDataPkt(const Payload &payload, std::string &dstData);
#endif //  MINIRPC_COMMON_PAYLOAD_H