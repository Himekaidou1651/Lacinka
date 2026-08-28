#ifndef SOURCE_H
#define SOURCE_H

#include "../common/Common.h"

enum class transformType{
    normal,
    hellasLatin,
    jugoslav
};

class transforml{
protected:
    std::string input;
    std::string output;
    size_t getLen();
    transformType type;
public:
    transforml(std::string input);
    virtual ~transforml() = default;
    virtual void transform() = 0;
    virtual void outputl();
};

#endif
