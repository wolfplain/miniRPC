#include "common/sha1.h"
#include "common/log.h"

#include <openssl/sha.h>
namespace Sha1 {
// if not safe using string to store key, need fix
std::string GetSha1HashString(const std::string& secKey)
{
    unsigned char *ret = SHA1(reinterpret_cast<const unsigned char *>(secKey.c_str()), secKey.size(), nullptr);
    if (ret == nullptr) {
        LOG_E("get sha1 fail");
        return "";
    }

    return std::string(reinterpret_cast<char*>(ret));
}
}  // namespace Sha1