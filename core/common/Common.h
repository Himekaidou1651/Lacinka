#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cctype>

enum class transformType{
    normal,
    hellasLatin,
    jugoslavLatin,
    eonmonLatin,
    bielaOldLatin,
    bielaNewLatin,
    ukrajinaLatin,
    latinOldLatin,
    rusnewOldLatin,
    rusoldOldLatin,
    forsiTojikLatin,
    hayerenLatin,
    kartuliLatin,
};

inline void replaceAll(std::string& s, const std::string& from, const std::string& to) {
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.length(), to);
        pos += to.length();
    }
}
