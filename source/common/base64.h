#ifndef MINIRPC_COMMON_BASE64_H
#define MINIRPC_COMMON_BASE64_H
#include <string>
bool Base64Encode(const unsigned char *pIn, unsigned long uInLen, std::string& strOut);
bool Base64Decode(const std::string& strIn, unsigned char *pOut, unsigned long *uOutLen);
#endif  //  MINIRPC_COMMON_BASE64_H