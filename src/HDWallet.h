#include <string>

#include "proto/libcoins.pb.h"
#include "TrezorCrypto/bip32.h"

#define VERSION_PUBLIC 0x0488b21e
#define VERSION_PRIVATE 0x0488ade4

namespace libcoins {
    class HDWallet {
        public:
            static int hdnode_new(const std::vector<uint8_t> & seed, std::vector<uint8_t> & result);
            static std::string serialize_xprv(std::vector<uint8_t> hdNode);

        private:
            static HDNodeMessage hdnode_to_message(HDNode* cNode);
            static HDNode hdnode_to_native(HDNodeMessage* message);
    };
}