//Hellas to Latin
#pragma once

#include "source.h"

class hellasLatin: public transforml{
public:
    explicit hellasLatin(std::string input);
    void transform() override;
    void outputl() override;
};