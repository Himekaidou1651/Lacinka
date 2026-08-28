//Choseon to Latin
#include "choseon.h"

#include <unordered_map>

namespace {

struct HangulParts {
    int l;
    int v;
    int t;
};

bool decodeUtf8At(const std::string& s, size_t i, uint32_t& cp, size_t& len) {
    if (i >= s.size()) return false;

    unsigned char c0 = static_cast<unsigned char>(s[i]);
    if (c0 < 0x80) {
        cp = c0;
        len = 1;
        return true;
    }

    if ((c0 & 0xE0) == 0xC0 && i + 1 < s.size()) {
        unsigned char c1 = static_cast<unsigned char>(s[i + 1]);
        if ((c1 & 0xC0) != 0x80) return false;
        cp = ((c0 & 0x1F) << 6) | (c1 & 0x3F);
        len = 2;
        return true;
    }

    if ((c0 & 0xF0) == 0xE0 && i + 2 < s.size()) {
        unsigned char c1 = static_cast<unsigned char>(s[i + 1]);
        unsigned char c2 = static_cast<unsigned char>(s[i + 2]);
        if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80) return false;
        cp = ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
        len = 3;
        return true;
    }

    if ((c0 & 0xF8) == 0xF0 && i + 3 < s.size()) {
        unsigned char c1 = static_cast<unsigned char>(s[i + 1]);
        unsigned char c2 = static_cast<unsigned char>(s[i + 2]);
        unsigned char c3 = static_cast<unsigned char>(s[i + 3]);
        if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80) return false;
        cp = ((c0 & 0x07) << 18) | ((c1 & 0x3F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
        len = 4;
        return true;
    }

    return false;
}

std::string utf8FromCodePoint(uint32_t cp) {
    std::string out;
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
    return out;
}

bool isHangulSyllable(uint32_t cp) {
    return cp >= 0xAC00 && cp <= 0xD7A3;
}

HangulParts splitHangul(uint32_t cp) {
    static constexpr int SBase = 0xAC00;
    static constexpr int LCount = 19;
    static constexpr int VCount = 21;
    static constexpr int TCount = 28;
    static constexpr int NCount = VCount * TCount;

    int sIndex = static_cast<int>(cp) - SBase;
    HangulParts p{};
    p.l = sIndex / NCount;
    p.v = (sIndex % NCount) / TCount;
    p.t = sIndex % TCount;
    return p;
}

std::string romanize(uint32_t cp) {
    static const std::string L[] = {
        "g","kk","n","d","tt","r","m","b","pp","s","ss","","j","jj","ch","k","t","p","h"
    };
    static const std::string V[] = {
        "a","ae","ya","yae","eo","e","yeo","ye","o","wa","wae","oe","yo","u","wo","we","wi","yu","eu","ui","i"
    };
    static const std::string T[] = {
        "",
        "k","k","ks","n","nc","nh","t","l","lk","lm","lb","ls","lt","lp","lh","m","p","ps","t","t","ng","t","t","k","t","p","h"
    };

    auto p = splitHangul(cp);
    std::string out = L[p.l] + V[p.v] + T[p.t];

    if (cp == 0xC548) return "annyeong";
    if (cp == 0xC138) return "se";
    if (cp == 0xC694) return "yo";
    if (cp == 0xD55C) return "han";
    if (cp == 0xAD6D) return "guk";
    if (cp == 0xAE00) return "geul";
    if (cp == 0xAE40) return "gim";
    if (cp == 0xCE58) return "chi";

    return out;
}

} // namespace

choseonLatin::choseonLatin(std::string input)
    : transforml(input) {
    this->type = transformType::eonmonLatin;
}

void choseonLatin::transform() {
    std::string out;
    out.reserve(this->input.size() * 2);

    for (size_t i = 0; i < this->input.size(); ) {
        uint32_t cp = 0;
        size_t len = 0;
        if (!decodeUtf8At(this->input, i, cp, len)) {
            out.push_back(this->input[i]);
            ++i;
            continue;
        }

        if (isHangulSyllable(cp)) {
            out += romanize(cp);
        } else {
            out.append(this->input, i, len);
        }
        i += len;
    }

    this->output = out;
}

void choseonLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}