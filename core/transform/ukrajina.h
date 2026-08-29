//Ukrajina Lacinka
#pragma once

#include "source.h"

class ukrajinaLatin : public transforml {
public:
    explicit ukrajinaLatin(std::string input);
    void transform() override;
    void outputl() override;
};
