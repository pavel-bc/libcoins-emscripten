#include <string>
#include <vector>

#include "HexEncoding.hpp"

namespace libcoins {
    // byte array to hex string
    std::string HexEncoding::to_hex(std::vector<uint8_t> & input) {
        static const char* const lut = "0123456789abcdef";
        std::string in(input.begin(), input.end());
        size_t len = in.length();

        std::string output;
        output.reserve(2 * len);
        for (size_t i = 0; i < len; ++i) {
            const unsigned char c = in[i];
            output.push_back(lut[c >> 4]);
            output.push_back(lut[c & 15]);
        }
        return output;
    }

    // hex string to byte array
    int HexEncoding::from_hex(const std::string & input, std::vector<uint8_t> & result) {
        static const char* const lut = "0123456789abcdef";
        size_t len = input.length();
        if (len & 1) {
            //throw std::invalid_argument("odd length");
            return 0;
        }

        std::string output;
        output.reserve(len / 2);
        for (size_t i = 0; i < len; i += 2) {
            char a = input[i];
            const char* p = std::lower_bound(lut, lut + 16, a);
            if (*p != a) {
                //throw std::invalid_argument("not a hex digit");
                return 0;
            }

            char b = input[i + 1];
            const char* q = std::lower_bound(lut, lut + 16, b);
            if (*q != b) {
                //throw std::invalid_argument("not a hex digit");
                return 0;
            }

            output.push_back(((p - lut) << 4) | (q - lut));
        }

        result.assign(std::begin(output), std::end(output));
        return output.size();
    }
}