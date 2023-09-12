#include "common/payload.h"
#include "common/log.h"
#include "common/sha1.h"
#include "common/base64.h"

#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>

#include <cstring>
#include <memory>

#if __BIG_ENDIAN__
    #define htonll(x)   (x)
    #define ntohll(x)   (x)
#else
    #define htonll(x)   ((((uint64_t)htonl(x&0xFFFFFFFF)) << 32) + htonl(x >> 32))
    #define ntohll(x)   ((((uint64_t)ntohl(x&0xFFFFFFFF)) << 32) + ntohl(x >> 32))
#endif

ErrorNo DecodeShakeHanderPkt(const std::string &pkt, std::unordered_map<std::string, std::string> &header)
{
    if (pkt.empty()) {
        LOG_E("pkt empty");
        return ErrorNo::DATA_INVALID;
    }
    size_t start = 0;
    size_t end = 0;
    while (end < pkt.size()) {
        if (pkt[end] != '\r') {
            end++;
            continue;
        }
        std::string content = pkt.substr(start, end - start);
        if (!content.empty()) {
            size_t pos = content.find_first_of(":");
            std::string key = content.substr(0, pos);
            pos = content.find_first_not_of(" ", pos + 1);  // trim the invalid empty space
            std::string value = content.substr(pos);
            header[key] = value;
            LOG_D("key:",key,"value:",value);
        }
        start = end + 2;    // skip \r\n
        end += 3; 
    }
    return ErrorNo::SUCCESS;
}

ErrorNo EncodeShakeHanderPkt(const std::unordered_map<std::string, std::string> &header, std::string &dstData)
{
    std::string sha1Hash = Sha1::GetSha1HashString(header.at("Sec-WebSocket-Key") + webSocketSecKey);
    if (sha1Hash.empty()) {
        LOG_E("get sec sha1 hash fail");
        return ErrorNo::FAILURE;
    }
    std::string secAcceptKey;
    if (Base64Encode(reinterpret_cast<const unsigned char*>(sha1Hash.c_str()),
        sha1Hash.size(), secAcceptKey) != true) {
        LOG_E("get base64 string fail");
        return ErrorNo::DATA_INVALID;
    }
    dstData.clear();
    dstData.append("HTTP/1.1 101 Switching Protocols\r\n");
    dstData.append("Connection: upgrade\r\n");
    dstData.append("Upgrade: websocket\r\n");
    dstData.append("Sec-WebSocket-Accept: " + secAcceptKey + "\r\n");

    // when add permessage-deflate the wscat can't process
    // if (header.count("Sec-WebSocket-Extensions")) {
    //     dstData.append("Sec-WebSocket-Extensions: permessage-deflate; server_no_context_takeover");
    //     // + header.at("Sec-WebSocket-Extensions") + "\r\n");  server_max_window_bits
    // }
    dstData.append("\r\n");  // last line
    return ErrorNo::SUCCESS;
}

ErrorNo DecodeDataPkt(const std::string &pkt, Payload &payload)
{
    if (pkt.size() <= MIN_PKT_SIZE) {
        LOG_E("invaild pkt");
        return ErrorNo::DATA_INVALID;
    }
    size_t pos = 0;
    // first byte get fin and opcode
    payload.fin = FIN(pkt[pos]);
    payload.rsv = RSV(pkt[pos]);
    // LOG_D("fin:", payload.fin, ",rsv:", payload.rsv);
    payload.opcode = OPCODE(pkt[pos]);
    // second byte get mask and payload len
    pos++;
    payload.mask = MASK(pkt[pos]);
    payload.payloadLen = (pkt[pos] & 0x07F);   // 0x07F : payload last 7 bits
    // check payload len
    pos++;
    uint8_t payloadValidBit = (payload.payloadLen == PAYLOAD_LEN_LIMIT126) ? 2 :
        ((payload.payloadLen == PAYLOAD_LEN_LIMIT127) ? 8 : 0);
    if (payloadValidBit) {
        uint64_t length = 0;
        if (memcpy(&length, pkt.c_str() + pos, payloadValidBit) == nullptr) {
            LOG_E("payload len decode fail");
            return ErrorNo::DATA_INVALID;
        }
        payload.payloadLen = (payload.payloadLen == PAYLOAD_LEN_LIMIT126) ? ntohs(length) : ntohll(length);
        pos += payloadValidBit;
    }

    if (payload.mask != 0) {
        if (memcpy(payload.maskKey, pkt.c_str() + pos, MASK_KEY_LEN) == nullptr) {
            LOG_E("get mask key fail");
            return ErrorNo::DATA_INVALID;
        }
        pos += MASK_KEY_LEN;
        for (size_t i = 0; i < payload.payloadLen; ++i) {
            size_t j = i % MASK_KEY_LEN;
            char data = pkt[pos + i] ^ payload.maskKey[j];
            payload.payload.push_back(data);
        }
    } else {
        payload.payload = pkt.substr(pos, payload.payloadLen);
    }
    return ErrorNo::SUCCESS;
}

ErrorNo EncodeDataPkt(const Payload &payload, std::string &dstData)
{
/*
    // WebSocket frame header
    unsigned char frame[10];
    frame[0] = 0x81;  // FIN + Text frame
    frame[1] = 0x05;  // Payload length (5 bytes)

    // WebSocket data payload
    const std::string message = "Hello"; // Your WebSocket message
    memcpy(&frame[2], message.c_str(), message.size());

    // Send the WebSocket frame
    if (send(connectFd_, frame, sizeof(frame), 0) == -1) {
        perror("Send failed");
        return ErrorNo::SUCCESS;
    }
    return ErrorNo::SUCCESS;
*/
    // 0x80 set the FIN means all data has been processed
    uint8_t header = (1 << 7) | (static_cast<uint8_t>(payload.opcode));
    dstData.push_back(static_cast<char>(header));
    size_t dataLen = payload.payload.size();
    uint8_t payloadLen = 0;
    uint8_t validPayloadBit = 0;
    if (dataLen < PAYLOAD_LEN_LIMIT126) {
        payloadLen = static_cast<uint8_t>(dataLen);
    } else if (dataLen >= PAYLOAD_LEN_LIMIT126 && dataLen < 0x0FFFF) {
        payloadLen = PAYLOAD_LEN_LIMIT126;
        validPayloadBit = 2;
    } else {
        payloadLen = PAYLOAD_LEN_LIMIT127;
        validPayloadBit = 8;
    }
    dstData.push_back(static_cast<char>(payloadLen));
    if (validPayloadBit) {
        std::unique_ptr<char[]> lenBuff(new char[validPayloadBit]);
        uint64_t len = (payloadLen = PAYLOAD_LEN_LIMIT126) ? htons(dataLen) : htonll(dataLen);
        if (memcpy(lenBuff.get(), &len, validPayloadBit) == nullptr) {
            LOG_E("payload len decode fail");
            return ErrorNo::DATA_INVALID;
        }
        dstData.append(lenBuff.get());
    }
    dstData.append(payload.payload);
    return ErrorNo::SUCCESS;
}
