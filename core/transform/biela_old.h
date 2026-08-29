//Bielarus Lacinka
#pragma once

#include "source.h"

class bielaOldLatin : public transforml {
public:
    explicit bielaOldLatin(std::string input);
    void transform() override;
    void outputl() override;
};
