#include <string>
#include <vector>

namespace libcoins {
    class HexEncoding {
        public:
            static std::string to_hex(std::vector<uint8_t> & input);
            static int from_hex(const std::string & input, std::vector<uint8_t> & result);
    };
}