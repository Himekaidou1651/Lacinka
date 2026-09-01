//Sakartvelo Latin
#include "kartuli.h"

static void replaceAll(std::string& s, const std::string& from, const std::string& to) {
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.length(), to);
        pos += to.length();
    }
}

kartuliLatin::kartuliLatin(std::string input)
    : transforml(input) {
    this->type = transformType::kartuliLatin;
}

void kartuliLatin::transform() {
    std::string s = this->input;

    replaceAll(s, "\u10D0", "a");
    replaceAll(s, "\u10D1", "b");
    replaceAll(s, "\u10D2", "g");
    replaceAll(s, "\u10D3", "d");
    replaceAll(s, "\u10D4", "e");
    replaceAll(s, "\u10D5", "v");
    replaceAll(s, "\u10D6", "z");
    replaceAll(s, "\u10F1", "ey");
    replaceAll(s, "\u10D7", "th");
    replaceAll(s, "\u10D8", "i");
    replaceAll(s, "\u10D9", "k");
    replaceAll(s, "\u10DA", "l");
    replaceAll(s, "\u10DB", "m");
    replaceAll(s, "\u10DC", "n");
    replaceAll(s, "\u10F2", "ye");
    replaceAll(s, "\u10DD", "o");
    replaceAll(s, "\u10DE", "p");
    replaceAll(s, "\u10DF", "ž");
    replaceAll(s, "\u10E0", "r");
    replaceAll(s, "\u10E1", "s");
    replaceAll(s, "\u10E2", "t");
    replaceAll(s, "\u10F3", "w");
    replaceAll(s, "\u10E3", "u");
    replaceAll(s, "\u10F7", "ë");
    replaceAll(s, "\u10E4", "ph");
    replaceAll(s, "\u10E5", "kh");
    replaceAll(s, "\u10E6", "ḡ");
    replaceAll(s, "\u10E7", "q");
    replaceAll(s, "\u10F8", "'");
    replaceAll(s, "\u10E8", "š");
    replaceAll(s, "\u10E9", "čč");
    replaceAll(s, "\u10EA", "cc");
    replaceAll(s, "\u10EB", "dz");
    replaceAll(s, "\u10EC", "c");
    replaceAll(s, "\u10ED", "č");
    replaceAll(s, "\u10EE", "x");
    replaceAll(s, "\u10F4", "qh");
    replaceAll(s, "\u10EF", "j");
    replaceAll(s, "\u10F0", "h");
    replaceAll(s, "\u10F5", "ō");
    
    this->output = s;
}

void kartuliLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}