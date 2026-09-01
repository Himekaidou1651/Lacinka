//serbia to hrvatska
#include "jugoslav.h"

serbiaLatin::serbiaLatin(std::string input)
    : transforml(input) {
    this->type = transformType::jugoslavLatin;
}

void serbiaLatin::transform() {
    std::string s = this->input;

    // 塞尔维亚西里尔字母 → 拉丁字母
    // 大写
    replaceAll(s, "\xD0\x90", "A");  // А → A
    replaceAll(s, "\xD0\x91", "B");  // Б → B
    replaceAll(s, "\xD0\x92", "V");  // В → V
    replaceAll(s, "\xD0\x93", "G");  // Г → G
    replaceAll(s, "\xD0\x94", "D");  // Д → D
    replaceAll(s, "\xD0\x82", "\xC4\x90");  // Ђ → Đ
    replaceAll(s, "\xD0\x95", "E");  // Е → E
    replaceAll(s, "\xD0\x96", "\xC5\xBD");  // Ж → Ž
    replaceAll(s, "\xD0\x97", "Z");  // З → Z
    replaceAll(s, "\xD0\x98", "I");  // И → I
    replaceAll(s, "\xD0\x88", "J");  // Ј → J
    replaceAll(s, "\xD0\x9A", "K");  // К → K
    replaceAll(s, "\xD0\x9B", "L");  // Л → L
    replaceAll(s, "\xD0\x89", "Lj"); // Љ → Lj
    replaceAll(s, "\xD0\x9C", "M");  // М → M
    replaceAll(s, "\xD0\x9D", "N");  // Н → N
    replaceAll(s, "\xD0\x8A", "Nj"); // Њ → Nj
    replaceAll(s, "\xD0\x9E", "O");  // О → O
    replaceAll(s, "\xD0\x9F", "P");  // П → P
    replaceAll(s, "\xD0\xA0", "R");  // Р → R
    replaceAll(s, "\xD0\xA1", "S");  // С → S
    replaceAll(s, "\xD0\xA2", "T");  // Т → T
    replaceAll(s, "\xD0\x8B", "\xC4\x86");  // Ћ → Ć
    replaceAll(s, "\xD0\xA3", "U");  // У → U
    replaceAll(s, "\xD0\xA4", "F");  // Ф → F
    replaceAll(s, "\xD0\xA5", "H");  // Х → H
    replaceAll(s, "\xD0\xA6", "C");  // Ц → C
    replaceAll(s, "\xD0\xA7", "\xC4\x8C");  // Ч → Č
    replaceAll(s, "\xD0\x8F", "D\u017E");  // Џ → Dž (use UTF-8 for ž)
    replaceAll(s, "\xD0\xA8", "\xC5\xA0");  // Ш → Š

    // 小写
    replaceAll(s, "\xD0\xB0", "a");  // а → a
    replaceAll(s, "\xD0\xB1", "b");  // б → b
    replaceAll(s, "\xD0\xB2", "v");  // в → v
    replaceAll(s, "\xD0\xB3", "g");  // г → g
    replaceAll(s, "\xD0\xB4", "d");  // д → d
    replaceAll(s, "\xD1\x92", "\xC4\x91");  // ђ → đ
    replaceAll(s, "\xD0\xB5", "e");  // е → e
    replaceAll(s, "\xD0\xB6", "\xC5\xBE");  // ж → ž
    replaceAll(s, "\xD0\xB7", "z");  // з → z
    replaceAll(s, "\xD0\xB8", "i");  // и → i
    replaceAll(s, "\xD1\x98", "j");  // ј → j
    replaceAll(s, "\xD0\xBA", "k");  // к → k
    replaceAll(s, "\xD0\xBB", "l");  // л → l
    replaceAll(s, "\xD1\x99", "lj"); // љ → lj
    replaceAll(s, "\xD0\xBC", "m");  // м → m
    replaceAll(s, "\xD0\xBD", "n");  // н → n
    replaceAll(s, "\xD1\x9A", "nj"); // њ → nj
    replaceAll(s, "\xD0\xBE", "o");  // о → o
    replaceAll(s, "\xD0\xBF", "p");  // п → p
    replaceAll(s, "\xD1\x80", "r");  // р → r
    replaceAll(s, "\xD1\x81", "s");  // с → s
    replaceAll(s, "\xD1\x82", "t");  // т → t
    replaceAll(s, "\xD1\x9B", "\xC4\x87");  // ћ → ć
    replaceAll(s, "\xD1\x83", "u");  // у → u
    replaceAll(s, "\xD1\x84", "f");  // ф → f
    replaceAll(s, "\xD1\x85", "h");  // х → h
    replaceAll(s, "\xD1\x86", "c");  // ц → c
    replaceAll(s, "\xD1\x87", "\xC4\x8D");  // ч → č
    replaceAll(s, "\xD1\x9F", "d\u017E");  // џ → dž (use UTF-8 for ž)
    replaceAll(s, "\xD1\x88", "\xC5\xA1");  // ш → š

    this->output = s;
}

void serbiaLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}