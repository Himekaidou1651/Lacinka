//Hellas to Latin
#include "hellas.h"

namespace {
using Codepoint = uint32_t;
using Text = std::vector<Codepoint>;

enum class CaseStyle {
    lower,
    title,
    upper
};

static bool isGreekUpper(Codepoint cp) {
    return (cp >= 0x0391 && cp <= 0x03A9 && cp != 0x03A2);
}

static bool isGreekLower(Codepoint cp) {
    return (cp >= 0x03B1 && cp <= 0x03C9) || cp == 0x03C2;
}

static Codepoint toGreekLower(Codepoint cp) {
    if (cp >= 0x0391 && cp <= 0x03A9 && cp != 0x03A2) {
        return cp + 0x20;
    }
    return cp;
}

static CaseStyle detectCase(const Text& text, size_t pos, size_t len) {
    bool allUpper = true;
    bool allLower = true;
    bool firstUpper = isGreekUpper(text[pos]);

    for (size_t i = 0; i < len; ++i) {
        Codepoint cp = text[pos + i];
        if (isGreekUpper(cp)) {
            allLower = false;
        } else if (isGreekLower(cp)) {
            allUpper = false;
        } else {
            allUpper = false;
            allLower = false;
        }
    }

    if (allLower) {
        return CaseStyle::lower;
    }
    if (allUpper) {
        return CaseStyle::upper;
    }
    if (firstUpper) {
        return CaseStyle::title;
    }
    return CaseStyle::lower;
}

static Codepoint toLatinUpper(Codepoint cp) {
    if (cp >= U'a' && cp <= U'z') {
        return cp - 0x20;
    }
    switch (cp) {
        case 0x0113: return 0x0112;
        case 0x014D: return 0x014C;
        case 0x0161: return 0x0160;
        default: return cp;
    }
}

static Text applyCase(const Text& base, CaseStyle style) {
    if (style == CaseStyle::lower) {
        return base;
    }

    Text out;
    out.reserve(base.size());
    for (size_t i = 0; i < base.size(); ++i) {
        Codepoint cp = base[i];
        if (i > 0) {
            out.push_back(cp);
        } else {
            out.push_back(toLatinUpper(cp));
        }
    }
    return out;
}

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

static bool matchAt(const Text& text, size_t pos, const Text& pattern) {
    if (pos + pattern.size() > text.size()) {
        return false;
    }
    for (size_t i = 0; i < pattern.size(); ++i) {
        if (toGreekLower(text[pos + i]) != pattern[i]) {
            return false;
        }
    }
    return true;
}
} // namespace

hellasLatin::hellasLatin(std::string input)
    : transforml(input) {
    this->type = transformType::hellasLatin;
}

void hellasLatin::transform() {
    static const struct Rule {
        Text pattern;
        Text replacement;
    } rules[] = {
        {{0x03BC, 0x03C0, 0x03C1}, {0x0062, 0x0072}},
        {{0x03BD, 0x03C4, 0x03C1}, {0x0064, 0x0072}},
        {{0x03B3, 0x03BA, 0x03C1}, {0x0067, 0x0072}},
        {{0x03BF, 0x03C5}, {0x006F, 0x0075}},
        {{0x03B1, 0x03C5}, {0x0061, 0x0075}},
        {{0x03B5, 0x03C5}, {0x0065, 0x0075}},
        {{0x03B7, 0x03C5}, {0x0113, 0x0075}},
        {{0x03B3, 0x03B3}, {0x006E, 0x0067}},
        {{0x03B3, 0x03C7}, {0x006E, 0x0078}},
        {{0x03C3, 0x03C7}, {0x0161}},
        {{0x03C4, 0x03C3}, {0x0063}},
        {{0x03C4, 0x03B6}, {0x0064, 0x007A}},
        {{0x03BC, 0x03C0}, {0x006D, 0x0062}},
        {{0x03BD, 0x03C4}, {0x006E, 0x0064}},
        {{0x03B3, 0x03BA}, {0x006E, 0x0067}},
        {{0x03B1}, {0x0061}},
        {{0x03B2}, {0x0062}},
        {{0x03B3}, {0x0067}},
        {{0x03B4}, {0x0064}},
        {{0x03B5}, {0x0065}},
        {{0x03B6}, {0x007A}},
        {{0x03B7}, {0x0113}},
        {{0x03B8}, {0x0074, 0x0068}},
        {{0x03B9}, {0x0069}},
        {{0x03BA}, {0x006B}},
        {{0x03BB}, {0x006C}},
        {{0x03BC}, {0x006D}},
        {{0x03BD}, {0x006E}},
        {{0x03BE}, {0x006B, 0x0073}},
        {{0x03BF}, {0x006F}},
        {{0x03C0}, {0x0070}},
        {{0x03C1}, {0x0072}},
        {{0x03C3}, {0x0073}},
        {{0x03C2}, {0x0073}},
        {{0x03C4}, {0x0074}},
        {{0x03C5}, {0x0079}},
        {{0x03C6}, {0x0070, 0x0068}},
        {{0x03C7}, {0x0078}},
        {{0x03C8}, {0x0070, 0x0073}},
        {{0x03C9}, {0x014D}},
        {{0x0391}, {0x0061}},
        {{0x0392}, {0x0062}},
        {{0x0393}, {0x0067}},
        {{0x0394}, {0x0064}},
        {{0x0395}, {0x0065}},
        {{0x0396}, {0x007A}},
        {{0x0397}, {0x0113}},
        {{0x0398}, {0x0074, 0x0068}},
        {{0x0399}, {0x0069}},
        {{0x039A}, {0x006B}},
        {{0x039B}, {0x006C}},
        {{0x039C}, {0x006D}},
        {{0x039D}, {0x006E}},
        {{0x039E}, {0x006B, 0x0073}},
        {{0x039F}, {0x006F}},
        {{0x03A0}, {0x0070}},
        {{0x03A1}, {0x0072}},
        {{0x03A3}, {0x0073}},
        {{0x03A4}, {0x0074}},
        {{0x03A5}, {0x0079}},
        {{0x03A6}, {0x0070, 0x0068}},
        {{0x03A7}, {0x0078}},
        {{0x03A8}, {0x0070, 0x0073}},
        {{0x03A9}, {0x014D}}
    };

    Text input = decodeUtf8(this->input);
    Text output;
    for (size_t i = 0; i < input.size();) {
        bool matched = false;
        for (const auto& rule : rules) {
            if (matchAt(input, i, rule.pattern)) {
                CaseStyle style = detectCase(input, i, rule.pattern.size());
                Text replacement = applyCase(rule.replacement, style);
                output.insert(output.end(), replacement.begin(), replacement.end());
                i += rule.pattern.size();
                matched = true;
                break;
            }
        }
        if (!matched) {
            output.push_back(input[i]);
            ++i;
        }
    }
    this->output = encodeUtf8(output);
}

void hellasLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}
