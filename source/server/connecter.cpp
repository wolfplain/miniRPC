#include "server/connecter.h"
#include "common/log.h"
#include <cstring>
#include <unistd.h>

ErrorNo Connecter::ProcessMessage(const std::string &content) {
    ErrorNo errcodeNo = ErrorNo::SUCCESS;
    if (status_ == ConnectStatus::WAITING_SHAKE_MSG) {
        errcodeNo = ProcessShakeHanderMessage(content);
        if (errcodeNo == ErrorNo::SUCCESS) {
            status_ = ConnectStatus::WAITING_DATA_PKT;
        }
        return errcodeNo;
    } else if (status_ == ConnectStatus::WAITING_DATA_PKT) {
        errcodeNo = ProcessDataPktMessage(content);
        if (payload_.fin == 1) {
            status_ = ConnectStatus::DATA_PKT_DONE;
        }
        if (payload_.opcode == OpCode::DISCONNECT_FRAME) {
            return ErrorNo::DISCONNECT;
        }
    }
    return errcodeNo;
}

ErrorNo Connecter::ProcessShakeHanderMessage(const std::string &content) {
    ErrorNo errcodeNo = DecodeShakeHanderPkt(content, headerInfo_);
    if (errcodeNo != ErrorNo::SUCCESS) {
        LOG_E("decode shake hander msg fail");
        return errcodeNo;
    }
    std::cout << "try response shake hander" << std::endl;
    std::string responseMsg;
    errcodeNo = EncodeShakeHanderPkt(headerInfo_, responseMsg);
    if (errcodeNo != ErrorNo::SUCCESS) {
        return errcodeNo;
    }
    return Write(responseMsg);
}

ErrorNo Connecter::Write(const std::string &responseMsg) {
    if (::write(connectFd_, responseMsg.c_str(), responseMsg.size()) < 0) {
        LOG_E("send msg fail");
        return ErrorNo::FAILURE;
    }
    return ErrorNo::SUCCESS;
}

ErrorNo Connecter::ProcessDataPktMessage(const std::string &content) {
    LOG_D("start process data package");
    return DecodeDataPkt(content, payload_);
}

ErrorNo Connecter::SendResponse(const Response &rsp) {
    LOG_D("start send rsp");

    std::string dstData;
    Payload payload;
    payload.payload = rsp.message;
    payload.opcode = OpCode::TEXT_FRAME;
    ErrorNo erorNo = EncodeDataPkt(payload, dstData);
    if (erorNo != ErrorNo::SUCCESS) {
        LOG_E("process rsp encode fail");
        return erorNo;
    }

    return Write(dstData);
}

void Connecter::ResetPayload() {
    payload_ = {0};
}