//Russ Lacinka
#pragma once

#include "source.h"

class rusoldOldLatin : public transforml {
public:
    explicit rusoldOldLatin(std::string input);
    void transform() override;
    void outputl() override;
};
