//Hellas to Latin
#include "hellas.h"

hellasLatin::hellasLatin(std::string input)
    : transforml(input) {
    this->type = transformType::hellasLatin;
}

void hellasLatin::transform() {
    this->output = this->input;
}

void hellasLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}
