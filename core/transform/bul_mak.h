//bulgarian makedonian
#pragma once

#include "source.h"

class bulmakLatin: public transforml{
public:
    explicit bulmakLatin(std::string input);
    void transform() override;
    void outputl() override;
};
