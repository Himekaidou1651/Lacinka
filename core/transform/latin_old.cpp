//Old Latin
#include "latin_old.h"

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
