#ifndef MINIRPC_COMMON_SHA1_H
#define MINIRPC_COMMON_SHA1_H
#include "common/errorno.h"
#include <string>
namespace Sha1 {
std::string GetSha1HashString(const std::string& secKey);
}  // namespace Sha1
#endif  // MINIRPC_COMMON_SHA1_H