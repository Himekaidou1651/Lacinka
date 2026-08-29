//Bielarus old Lacinka to Latin
#include "biela_old.h"

#include <cctype>

namespace {

static size_t oldUtf8Len(unsigned char lead) {
    if (lead < 0x80) {
        return 1;
    }
    if ((lead >> 5) == 0x6) {
        return 2;
    }
    if ((lead >> 4) == 0xE) {
        return 3;
    }
    if ((lead >> 3) == 0x1E) {
        return 4;
    }
    return 1;
}

static bool oldDecodeUtf8(const std::string& s, size_t pos, char32_t& cp, size_t& len) {
    const unsigned char lead = static_cast<unsigned char>(s[pos]);
    len = oldUtf8Len(lead);
    if (pos + len > s.size()) {
        cp = lead;
        len = 1;
        return false;
    }
    switch (len) {
    case 1:
        cp = lead;
        return true;
    case 2:
        cp = (static_cast<char32_t>(lead & 0x1F) << 6)
           | static_cast<char32_t>(static_cast<unsigned char>(s[pos + 1]) & 0x3F);
        return true;
    case 3:
        cp = (static_cast<char32_t>(lead & 0x0F) << 12)
           | (static_cast<char32_t>(static_cast<unsigned char>(s[pos + 1]) & 0x3F) << 6)
           | static_cast<char32_t>(static_cast<unsigned char>(s[pos + 2]) & 0x3F);
        return true;
    case 4:
        cp = (static_cast<char32_t>(lead & 0x07) << 18)
           | (static_cast<char32_t>(static_cast<unsigned char>(s[pos + 1]) & 0x3F) << 12)
           | (static_cast<char32_t>(static_cast<unsigned char>(s[pos + 2]) & 0x3F) << 6)
           | static_cast<char32_t>(static_cast<unsigned char>(s[pos + 3]) & 0x3F);
        return true;
    default:
        cp = lead;
        len = 1;
        return false;
    }
}

static bool oldIsBoundary(char32_t cp) {
    if (cp < 0x80) {
        const unsigned char c = static_cast<unsigned char>(cp);
        return std::isspace(c) || std::ispunct(c);
    }
    switch (cp) {
    case 0x2018:
    case 0x2019:
    case 0x201C:
    case 0x201D:
    case 0x2010:
    case 0x2011:
    case 0x00AB:
    case 0x00BB:
    case 0x02BC:
        return true;
    default:
        return false;
    }
}

static bool oldIsVowel(char32_t cp) {
    switch (cp) {
    case 0x0410:
    case 0x0430:
    case 0x0415:
    case 0x0435:
    case 0x0401:
    case 0x0451:
    case 0x0406:
    case 0x0456:
    case 0x041E:
    case 0x043E:
    case 0x0423:
    case 0x0443:
    case 0x042B:
    case 0x044B:
    case 0x042D:
    case 0x044D:
    case 0x042E:
    case 0x044E:
    case 0x042F:
    case 0x044F:
        return true;
    default:
        return false;
    }
}

} // namespace

bielaOldLatin::bielaOldLatin(std::string input)
    : transforml(input) {
    this->type = transformType::bielaOldLatin;
}

void bielaOldLatin::transform() {
    std::string out;
    out.reserve(this->input.size() * 2);

    bool atWordStart = true;
    bool prevWasVowel = false;

    for (size_t i = 0; i < this->input.size(); ) {
        char32_t cp = 0;
        size_t len = 1;
        oldDecodeUtf8(this->input, i, cp, len);

        if (cp == 0x27 || cp == 0x2019 || cp == 0x02BC) {
            atWordStart = true;
            prevWasVowel = false;
            i += len;
            continue;
        }

        char32_t nextCp = 0;
        size_t nextLen = 0;
        const bool hasNext = i + len < this->input.size() && oldDecodeUtf8(this->input, i + len, nextCp, nextLen);
        if (hasNext && nextCp == 0x044C) {
            switch (cp) {
            case 0x0417: out += "\xC5\xB9"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            case 0x0437: out += "\xC5\xBA"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            case 0x0421: out += "\xC5\x9A"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            case 0x0441: out += "\xC5\x9B"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            case 0x041D: out += "\xC5\x83"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            case 0x043D: out += "\xC5\x84"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            case 0x041B: out += "L"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            case 0x043B: out += "l"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            case 0x0422: out += "\xC4\x86"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            case 0x0442: out += "\xC4\x87"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            case 0x0426: out += "\xC4\x86"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            case 0x0446: out += "\xC4\x87"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            case 0x0420: out += "Rz"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            case 0x0440: out += "rz"; i += len + nextLen; atWordStart = false; prevWasVowel = false; continue;
            default:
                break;
            }
        }

        if (cp == 0x0415 || cp == 0x0435 || cp == 0x0401 || cp == 0x0451 || cp == 0x042E || cp == 0x044E || cp == 0x042F || cp == 0x044F) {
            const bool useJ = atWordStart || prevWasVowel;
            switch (cp) {
            case 0x0415: out += useJ ? "Je" : "Ie"; break;
            case 0x0435: out += useJ ? "je" : "ie"; break;
            case 0x0401: out += useJ ? "Jo" : "Io"; break;
            case 0x0451: out += useJ ? "jo" : "io"; break;
            case 0x042E: out += useJ ? "Ju" : "Iu"; break;
            case 0x044E: out += useJ ? "ju" : "iu"; break;
            case 0x042F: out += useJ ? "Ja" : "Ia"; break;
            case 0x044F: out += useJ ? "ja" : "ia"; break;
            }
            atWordStart = false;
            prevWasVowel = true;
            i += len;
            continue;
        }

        switch (cp) {
        case 0x0410: out += "A"; prevWasVowel = true; atWordStart = false; break;
        case 0x0430: out += "a"; prevWasVowel = true; atWordStart = false; break;
        case 0x0411: out += "B"; prevWasVowel = false; atWordStart = false; break;
        case 0x0431: out += "b"; prevWasVowel = false; atWordStart = false; break;
        case 0x0412: out += "W"; prevWasVowel = false; atWordStart = false; break;
        case 0x0432: out += "w"; prevWasVowel = false; atWordStart = false; break;
        case 0x0413: out += "H"; prevWasVowel = false; atWordStart = false; break;
        case 0x0433: out += "h"; prevWasVowel = false; atWordStart = false; break;
        case 0x0490: out += "G"; prevWasVowel = false; atWordStart = false; break;
        case 0x0491: out += "g"; prevWasVowel = false; atWordStart = false; break;
        case 0x0414: out += "D"; prevWasVowel = false; atWordStart = false; break;
        case 0x0434: out += "d"; prevWasVowel = false; atWordStart = false; break;
        case 0x0416: out += "\xC5\xBD"; prevWasVowel = false; atWordStart = false; break;
        case 0x0436: out += "\xC5\xBE"; prevWasVowel = false; atWordStart = false; break;
        case 0x0417: out += "Z"; prevWasVowel = false; atWordStart = false; break;
        case 0x0437: out += "z"; prevWasVowel = false; atWordStart = false; break;
        case 0x0418: out += "I"; prevWasVowel = true; atWordStart = false; break;
        case 0x0438: out += "i"; prevWasVowel = true; atWordStart = false; break;
        case 0x0419: out += "J"; prevWasVowel = false; atWordStart = false; break;
        case 0x0439: out += "j"; prevWasVowel = false; atWordStart = false; break;
        case 0x041A: out += "K"; prevWasVowel = false; atWordStart = false; break;
        case 0x043A: out += "k"; prevWasVowel = false; atWordStart = false; break;
        case 0x041B: out += "\xC5\x81"; prevWasVowel = false; atWordStart = false; break;
        case 0x043B: out += "\xC5\x82"; prevWasVowel = false; atWordStart = false; break;
        case 0x041C: out += "M"; prevWasVowel = false; atWordStart = false; break;
        case 0x043C: out += "m"; prevWasVowel = false; atWordStart = false; break;
        case 0x041D: out += "N"; prevWasVowel = false; atWordStart = false; break;
        case 0x043D: out += "n"; prevWasVowel = false; atWordStart = false; break;
        case 0x041E: out += "O"; prevWasVowel = true; atWordStart = false; break;
        case 0x043E: out += "o"; prevWasVowel = true; atWordStart = false; break;
        case 0x041F: out += "P"; prevWasVowel = false; atWordStart = false; break;
        case 0x043F: out += "p"; prevWasVowel = false; atWordStart = false; break;
        case 0x0420: out += "R"; prevWasVowel = false; atWordStart = false; break;
        case 0x0440: out += "r"; prevWasVowel = false; atWordStart = false; break;
        case 0x0421: out += "S"; prevWasVowel = false; atWordStart = false; break;
        case 0x0441: out += "s"; prevWasVowel = false; atWordStart = false; break;
        case 0x0422: out += "T"; prevWasVowel = false; atWordStart = false; break;
        case 0x0442: out += "t"; prevWasVowel = false; atWordStart = false; break;
        case 0x0423: out += "U"; prevWasVowel = true; atWordStart = false; break;
        case 0x0443: out += "u"; prevWasVowel = true; atWordStart = false; break;
        case 0x040E: out += "\xC5\xAC"; prevWasVowel = false; atWordStart = false; break;
        case 0x045E: out += "\xC5\xAD"; prevWasVowel = false; atWordStart = false; break;
        case 0x0424: out += "F"; prevWasVowel = false; atWordStart = false; break;
        case 0x0444: out += "f"; prevWasVowel = false; atWordStart = false; break;
        case 0x0425: out += "Ch"; prevWasVowel = false; atWordStart = false; break;
        case 0x0445: out += "ch"; prevWasVowel = false; atWordStart = false; break;
        case 0x0426: out += "C"; prevWasVowel = false; atWordStart = false; break;
        case 0x0446: out += "c"; prevWasVowel = false; atWordStart = false; break;
        case 0x0427: out += "Cz"; prevWasVowel = false; atWordStart = false; break;
        case 0x0447: out += "cz"; prevWasVowel = false; atWordStart = false; break;
        case 0x0428: out += "Sz"; prevWasVowel = false; atWordStart = false; break;
        case 0x0448: out += "sz"; prevWasVowel = false; atWordStart = false; break;
        case 0x042B: out += "Y"; prevWasVowel = true; atWordStart = false; break;
        case 0x044B: out += "y"; prevWasVowel = true; atWordStart = false; break;
        case 0x042D: out += "E"; prevWasVowel = true; atWordStart = false; break;
        case 0x044D: out += "e"; prevWasVowel = true; atWordStart = false; break;
        case 0x044C:
        case 0x042C:
        case 0x042A:
        case 0x044A:
            prevWasVowel = false;
            atWordStart = false;
            break;
        default:
            out.append(this->input, i, len);
            prevWasVowel = false;
            atWordStart = false;
            break;
        }

        i += len;
    }

    this->output = out;
}

void bielaOldLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}
