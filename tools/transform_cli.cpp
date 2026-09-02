#include <iostream>
#include <iterator>
#include <string>

#include "../core/transform/biela_old.h"
#include "../core/transform/biela_new.h"
#include "../core/transform/choseon.h"
#include "../core/transform/hellas.h"
#include "../core/transform/jugoslav.h"
#include "../core/transform/ukrajina.h"
#include "../core/transform/latin_old.h"
#include "../core/transform/rusnew_old.h"
#include "../core/transform/rusold_old.h"
#include "../core/transform/forsitojik.h"
#include "../core/transform/hayeren_big.h"
#include "../core/transform/kartuli.h"
#include "../core/transform/bul_mak.h"

static int parseMode(const char* value) {
    if (!value) {
        return 0;
    }
    try {
        return std::stoi(value);
    } catch (...) {
        return 0;
    }
}

static std::string transformText(const std::string& input, int mode) {
    if (mode == 0) {
        hellasLatin t(input);
        t.transform();
        return t.getOutput();
    }
    else if (mode == 1) {
        serbiaLatin t(input);
        t.transform();
        return t.getOutput();
    }
    else if (mode == 2) {
        choseonLatin t(input);
        t.transform();
        return t.getOutput();
    }
    else if (mode == 3) {
        bielaOldLatin t(input);
        t.transform();
        return t.getOutput();
    }
    else if (mode == 4) {
        bielaNewLatin t(input);
        t.transform();
        return t.getOutput();
    }
    else if (mode == 5) {
        ukrajinaLatin t(input);
        t.transform();
        return t.getOutput();
    }
    else if (mode == 6) {
        latinOldLatin t(input);
        t.transform();
        return t.getOutput();
    }
    else if (mode == 7) {
        rusnewOldLatin t(input);
        t.transform();
        return t.getOutput();
    }
    else if (mode == 8) {
        rusoldOldLatin t(input);
        t.transform();
        return t.getOutput();
    }
    else if (mode == 9) {
        forsiTojikLatin t(input);
        t.transform();
        return t.getOutput();
    }
    else if (mode == 10) {
        hayerenLatin t(input);
        t.transform();
        return t.getOutput();
    }
    else if (mode == 11) {
        kartuliLatin t(input);
        t.transform();
        return t.getOutput();
    }
    else if (mode == 12) {
        bulmakLatin t(input);
        t.transform();
        return t.getOutput();
    }
    else {
        return input;
    }
}

int main(int argc, char** argv) {
    const int mode = argc > 1 ? parseMode(argv[1]) : 0;
    const std::string input((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());
    std::cout << transformText(input, mode);
    return 0;
}
