//Old Latin
#include "latin_old.h"

static void replaceAll(std::string& s, const std::string& from, const std::string& to) {
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.length(), to);
        pos += to.length();
    }
}

latinOldLatin::latinOldLatin(std::string input)
    : transforml(input) {
    this->type = transformType::latinOldLatin;
}

void latinOldLatin::transform() {
    std::string s = this->input;

    for(auto& c : s) {
        c = std::toupper(static_cast<unsigned char>(c));
    }

    replaceAll(s, "AE", "Æ");    // Æ
    replaceAll(s, "OE", "Œ");    // Œ
    replaceAll(s, "J", "I");     // I
    replaceAll(s, "U", "V");     // V
    replaceAll(s, "W", "V");     // V

    this->output = s;
}

void latinOldLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}
