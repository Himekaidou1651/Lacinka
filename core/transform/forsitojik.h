//Tojik Latin
#pragma once

#include "source.h"

class forsiTojikLatin : public transforml {
public:
    explicit forsiTojikLatin(std::string input);
    void transform() override;
    void outputl() override;
};
