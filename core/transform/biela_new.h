//Bielarus 2007 Latin
#pragma once

#include "source.h"

class bielaNewLatin : public transforml {
public:
    explicit bielaNewLatin(std::string input);
    void transform() override;
    void outputl() override;
};
