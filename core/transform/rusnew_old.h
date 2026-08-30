//Russ Lacinka
#pragma once

#include "source.h"

class rusnewOldLatin : public transforml {
public:
    explicit rusnewOldLatin(std::string input);
    void transform() override;
    void outputl() override;
};
