//serbia to hrvatska
#include "jugoslav.h"

namespace {
using Codepoint = uint32_t;
using Text = std::vector<Codepoint>;

static std::string encodeUtf8(const Text& text) {
    std::string out;
    for (Codepoint cp : text) {
        if (cp <= 0x7F) {
            out.push_back(static_cast<char>(cp));
        } else if (cp <= 0x7FF) {
            out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        } else if (cp <= 0xFFFF) {
            out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        } else {
            out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
    }
    return out;
}

static Text decodeUtf8(const std::string& input) {
    Text out;
    for (size_t i = 0; i < input.size();) {
        unsigned char c = static_cast<unsigned char>(input[i]);
        if (c < 0x80) {
            out.push_back(c);
            ++i;
            continue;
        }

        size_t len = 0;
        Codepoint cp = 0;
        if ((c >> 5) == 0x6) {
            len = 2;
            cp = c & 0x1F;
        } else if ((c >> 4) == 0xE) {
            len = 3;
            cp = c & 0x0F;
        } else if ((c >> 3) == 0x1E) {
            len = 4;
            cp = c & 0x07;
        } else {
            out.push_back(0xFFFD);
            ++i;
            continue;
        }

        if (i + len > input.size()) {
            out.push_back(0xFFFD);
            ++i;
            continue;
        }

        bool valid = true;
        for (size_t j = 1; j < len; ++j) {
            unsigned char cc = static_cast<unsigned char>(input[i + j]);
            if ((cc >> 6) != 0x2) {
                valid = false;
                break;
            }
            cp = (cp << 6) | (cc & 0x3F);
        }

        if (!valid) {
            out.push_back(0xFFFD);
            ++i;
            continue;
        }

        if ((len == 2 && cp < 0x80) || (len == 3 && cp < 0x800) || (len == 4 && cp < 0x10000) ||
            cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF)) {
            out.push_back(0xFFFD);
            ++i;
            continue;
        }

        out.push_back(cp);
        i += len;
    }
    return out;
}

static Text transliterate(const Text& input) {
    Text out;
    out.reserve(input.size() * 2);
    for (Codepoint cp : input) {
        switch (cp) {
            case 0x0410: out.push_back(U'A'); break;
            case 0x0430: out.push_back(U'a'); break;
            case 0x0411: out.push_back(U'B'); break;
            case 0x0431: out.push_back(U'b'); break;
            case 0x0412: out.push_back(U'V'); break;
            case 0x0432: out.push_back(U'v'); break;
            case 0x0413: out.push_back(U'G'); break;
            case 0x0433: out.push_back(U'g'); break;
            case 0x0414: out.push_back(U'D'); break;
            case 0x0434: out.push_back(U'd'); break;
            case 0x0402: out.push_back(0x0110); break;
            case 0x0452: out.push_back(0x0111); break;
            case 0x0415: out.push_back(U'E'); break;
            case 0x0435: out.push_back(U'e'); break;
            case 0x0416: out.push_back(0x017D); break;
            case 0x0436: out.push_back(0x017E); break;
            case 0x0417: out.push_back(U'Z'); break;
            case 0x0437: out.push_back(U'z'); break;
            case 0x0418: out.push_back(U'I'); break;
            case 0x0438: out.push_back(U'i'); break;
            case 0x0408: out.push_back(U'J'); break;
            case 0x0458: out.push_back(U'j'); break;
            case 0x041A: out.push_back(U'K'); break;
            case 0x043A: out.push_back(U'k'); break;
            case 0x041B: out.push_back(U'L'); break;
            case 0x043B: out.push_back(U'l'); break;
            case 0x0409: out.push_back(U'L'); out.push_back(U'j'); break;
            case 0x0459: out.push_back(U'l'); out.push_back(U'j'); break;
            case 0x041C: out.push_back(U'M'); break;
            case 0x043C: out.push_back(U'm'); break;
            case 0x041D: out.push_back(U'N'); break;
            case 0x043D: out.push_back(U'n'); break;
            case 0x040A: out.push_back(U'N'); out.push_back(U'j'); break;
            case 0x045A: out.push_back(U'n'); out.push_back(U'j'); break;
            case 0x041E: out.push_back(U'O'); break;
            case 0x043E: out.push_back(U'o'); break;
            case 0x041F: out.push_back(U'P'); break;
            case 0x043F: out.push_back(U'p'); break;
            case 0x0420: out.push_back(U'R'); break;
            case 0x0440: out.push_back(U'r'); break;
            case 0x0421: out.push_back(U'S'); break;
            case 0x0441: out.push_back(U's'); break;
            case 0x0422: out.push_back(U'T'); break;
            case 0x0442: out.push_back(U't'); break;
            case 0x040B: out.push_back(0x0106); break;
            case 0x045B: out.push_back(0x0107); break;
            case 0x0423: out.push_back(U'U'); break;
            case 0x0443: out.push_back(U'u'); break;
            case 0x0424: out.push_back(U'F'); break;
            case 0x0444: out.push_back(U'f'); break;
            case 0x0425: out.push_back(U'H'); break;
            case 0x0445: out.push_back(U'h'); break;
            case 0x0426: out.push_back(U'C'); break;
            case 0x0446: out.push_back(U'c'); break;
            case 0x0427: out.push_back(0x010C); break;
            case 0x0447: out.push_back(0x010D); break;
            case 0x040F: out.push_back(U'D'); out.push_back(0x017E); break;
            case 0x045F: out.push_back(U'd'); out.push_back(0x017E); break;
            case 0x0428: out.push_back(0x0160); break;
            case 0x0448: out.push_back(0x0161); break;
            default: out.push_back(cp); break;
        }
    }
    return out;
}
} // namespace

serbiaLatin::serbiaLatin(std::string input)
    : transforml(input) {
    this->type = transformType::jugoslav;
}

void serbiaLatin::transform() {
    Text input = decodeUtf8(this->input);
    Text output = transliterate(input);
    this->output = encodeUtf8(output);
}

void serbiaLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}
