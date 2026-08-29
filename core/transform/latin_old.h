//Old Latin
#pragma once

#include "source.h"

class latinOldLatin : public transforml {
public:
    explicit latinOldLatin(std::string input);
    void transform() override;
    void outputl() override;
};
