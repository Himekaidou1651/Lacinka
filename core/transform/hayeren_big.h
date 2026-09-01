//Hayastan Latin
#pragma once

#include "source.h"

class hayerenLatin : public transforml {
public:
    explicit hayerenLatin(std::string input);
    void transform() override;
    void outputl() override;
};
