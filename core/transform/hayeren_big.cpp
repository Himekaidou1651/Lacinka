//Hayastan Latin
#include "hayeren_big.h"

static void replaceAll(std::string& s, const std::string& from, const std::string& to) {
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.length(), to);
        pos += to.length();
    }
}

hayerenLatin::hayerenLatin(std::string input)
    : transforml(input) {
    this->type = transformType::hayerenLatin;
}

void hayerenLatin::transform() {
    std::string s = this->input;



    this->output = s;
}

void hayerenLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}