//Sakartvelo Latin
#pragma once

#include "source.h"

class kartuliLatin : public transforml {
public:
    explicit kartuliLatin(std::string input);
    void transform() override;
    void outputl() override;
};
