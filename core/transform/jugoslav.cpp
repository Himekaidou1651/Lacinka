//serbia to hrvatska
#include "jugoslav.h"

serbiaLatin::serbiaLatin(std::string input)
    : transforml(input) {
    this->type = transformType::jugoslav;
}

void serbiaLatin::transform() {
    this->output = this->input;
}

void serbiaLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}
