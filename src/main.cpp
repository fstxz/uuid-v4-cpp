#include <array>
#include <cassert>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

class Uuid {
  public:
    Uuid() : m_bytes() {
        std::random_device seed;
        std::uniform_int_distribution<uint8_t> dist(0, 255);

        for (auto &byte : m_bytes) {
            byte = dist(seed);
        }

        m_bytes[6] = (m_bytes[6] >> 4) | 0x40;
        m_bytes[8] = (m_bytes[8] >> 2) | 0x80;
    }

    auto is_valid() -> bool {
        return (m_bytes[6] >> 4) == 4 && // version 4
               (m_bytes[8] >> 6) == 2;   // variant 1
    }

    auto to_string() const -> std::string {
        std::stringstream ss;

        int i = 0;
        for (const auto &byte : m_bytes) {
            ss << std::hex << std::setfill('0') << std::setw(2)
               << static_cast<int>(byte);

            switch (i) {
            case 3:
            case 5:
            case 7:
            case 9:
                ss << "-";
                break;
            }

            ++i;
        }

        return ss.str();
    }

    friend auto operator<<(std::ostream &os, const Uuid &uuid)
        -> std::ostream & {
        os << uuid.to_string();
        return os;
    }

  private:
    std::array<uint8_t, 16> m_bytes;
};

auto main() -> int {
    Uuid uuid;
    assert(uuid.is_valid()); // will never fail (hopefully)

    std::cout << uuid << std::endl;

    return 0;
}
