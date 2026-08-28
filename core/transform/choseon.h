//Choseon to Latin
#pragma once

#include "source.h"

class choseonLatin : public transforml {
public:
    explicit choseonLatin(std::string input);
    void transform() override;
    void outputl() override;
};