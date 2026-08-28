//serbia to hrvatska
#pragma once

#include "source.h"

class serbiaLatin: public transforml{
public:
    explicit serbiaLatin(std::string input);
    void transform() override;
    void outputl() override;
};
