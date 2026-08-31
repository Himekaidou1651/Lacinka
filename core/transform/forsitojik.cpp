//tojik Latin
#include "forsitojik.h"

static void replaceAll(std::string& s, const std::string& from, const std::string& to) {
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.length(), to);
        pos += to.length();
    }
}

forsiTojikLatin::forsiTojikLatin(std::string input)
    : transforml(input) {
    this->type = transformType::forsiTojikLatin;
}

void forsiTojikLatin::transform() {
    std::string s = this->input;

    // 1) 词首位置插入哨兵 \x01
    s = "\x01" + s;
    replaceAll(s, " ", " \x01");
    replaceAll(s, "\t", "\t\x01");
    replaceAll(s, "\n", "\n\x01");
    replaceAll(s, "\r", "\r\x01");
    replaceAll(s, ",", ",\x01");
    replaceAll(s, ".", ".\x01");
    replaceAll(s, "!", "!\x01");
    replaceAll(s, "?", "?\x01");
    replaceAll(s, ";", ";\x01");
    replaceAll(s, ":", ":\x01");
    replaceAll(s, "(", "(\x01");
    replaceAll(s, ")", ")\x01");
    replaceAll(s, "[", "[\x01");
    replaceAll(s, "]", "]\x01");
    replaceAll(s, "\xC2\xAB", "\xC2\xAB\x01");   // «
    replaceAll(s, "\xC2\xBB", "\xC2\xBB\x01");   // »
    replaceAll(s, "\xE2\x80\x94", "\xE2\x80\x94\x01"); // —
    replaceAll(s, "\xE2\x80\x93", "\xE2\x80\x93\x01"); // –
    replaceAll(s, "\xE2\x80\xA6", "\xE2\x80\xA6\x01"); // …

    // 2) 词首 ъ/Ъ 通常不转写（删除，保留哨兵）
    replaceAll(s, "\x01\xD1\x8A", "\x01");   // ъ
    replaceAll(s, "\x01\xD0\xAA", "\x01");   // Ъ

    // 3) 词首 е → ye、Е → Ye
    replaceAll(s, "\x01\xD0\xB5", "ye");
    replaceAll(s, "\x01\xD0\x95", "Ye");

    // 4) 其余单字母（小写）
    replaceAll(s, "\xD0\xB0", "a");     // а
    replaceAll(s, "\xD0\xB1", "b");     // б
    replaceAll(s, "\xD0\xB2", "v");     // в
    replaceAll(s, "\xD0\xB3", "g");     // г
    replaceAll(s, "\xD2\x93", "\xC4\xA1"); // ғ → ġ
    replaceAll(s, "\xD0\xB4", "d");     // д
    replaceAll(s, "\xD0\xB5", "e");     // е
    replaceAll(s, "\xD1\x91", "yo");    // ё
    replaceAll(s, "\xD0\xB6", "\xC5\xBE"); // ж → ž
    replaceAll(s, "\xD0\xB7", "z");     // з
    replaceAll(s, "\xD0\xB8", "i");     // и
    replaceAll(s, "\xD3\xA3", "\xC4\xAB"); // ӣ → ī
    replaceAll(s, "\xD0\xB9", "y");     // й
    replaceAll(s, "\xD0\xBA", "k");     // к
    replaceAll(s, "\xD2\x9B", "q");     // қ
    replaceAll(s, "\xD0\xBB", "l");     // л
    replaceAll(s, "\xD0\xBC", "m");     // м
    replaceAll(s, "\xD0\xBD", "n");     // н
    replaceAll(s, "\xD0\xBE", "o");     // о
    replaceAll(s, "\xD0\xBF", "p");     // п
    replaceAll(s, "\xD1\x80", "r");     // р
    replaceAll(s, "\xD1\x81", "s");     // с
    replaceAll(s, "\xD1\x82", "t");     // т
    replaceAll(s, "\xD1\x83", "u");     // у
    replaceAll(s, "\xD3\xAF", "\xC5\xAB"); // ӯ → ū
    replaceAll(s, "\xD1\x84", "f");     // ф
    replaceAll(s, "\xD1\x85", "x");     // х
    replaceAll(s, "\xD2\xB3", "h");     // ҳ
    replaceAll(s, "\xD1\x87", "\xC4\x8D"); // ч → č
    replaceAll(s, "\xD2\xB7", "j");     // ҷ
    replaceAll(s, "\xD1\x88", "\xC5\xA1"); // ш → š
    replaceAll(s, "\xD1\x8D", "e");     // э
    replaceAll(s, "\xD1\x8E", "yu");    // ю
    replaceAll(s, "\xD1\x8F", "ya");    // я

    // 5) 其余单字母（大写）
    replaceAll(s, "\xD0\x90", "A");     // А
    replaceAll(s, "\xD0\x91", "B");     // Б
    replaceAll(s, "\xD0\x92", "V");     // В
    replaceAll(s, "\xD0\x93", "G");     // Г
    replaceAll(s, "\xD2\x92", "\xC4\xA0"); // Ғ → Ġ
    replaceAll(s, "\xD0\x94", "D");     // Д
    replaceAll(s, "\xD0\x95", "E");     // Е
    replaceAll(s, "\xD0\x81", "Yo");    // Ё
    replaceAll(s, "\xD0\x96", "\xC5\xBD"); // Ж → Ž
    replaceAll(s, "\xD0\x97", "Z");     // З
    replaceAll(s, "\xD0\x98", "I");     // И
    replaceAll(s, "\xD3\xA2", "\xC4\xAA"); // Ӣ → Ī
    replaceAll(s, "\xD0\x99", "Y");     // Й
    replaceAll(s, "\xD0\x9A", "K");     // К
    replaceAll(s, "\xD2\x9A", "Q");     // Қ
    replaceAll(s, "\xD0\x9B", "L");     // Л
    replaceAll(s, "\xD0\x9C", "M");     // М
    replaceAll(s, "\xD0\x9D", "N");     // Н
    replaceAll(s, "\xD0\x9E", "O");     // О
    replaceAll(s, "\xD0\x9F", "P");     // П
    replaceAll(s, "\xD0\xA0", "R");     // Р
    replaceAll(s, "\xD0\xA1", "S");     // С
    replaceAll(s, "\xD0\xA2", "T");     // Т
    replaceAll(s, "\xD0\xA3", "U");     // У
    replaceAll(s, "\xD3\xAE", "\xC5\xAA"); // Ӯ → Ū
    replaceAll(s, "\xD0\xA4", "F");     // Ф
    replaceAll(s, "\xD0\xA5", "X");     // Х
    replaceAll(s, "\xD2\xB2", "H");     // Ҳ
    replaceAll(s, "\xD0\xA7", "\xC4\x8C"); // Ч → Č
    replaceAll(s, "\xD2\xB6", "J");     // Ҷ
    replaceAll(s, "\xD0\xA8", "\xC5\xA0"); // Ш → Š
    replaceAll(s, "\xD0\xAD", "E");     // Э
    replaceAll(s, "\xD0\xAE", "Yu");    // Ю
    replaceAll(s, "\xD0\xAF", "Ya");    // Я

    // 6) 词内 ъ/Ъ → '（喉塞音）
    replaceAll(s, "\xD1\x8A", "'");     // ъ
    replaceAll(s, "\xD0\xAA", "'");     // Ъ

    // 7) 去掉哨兵
    replaceAll(s, "\x01", "");

    this->output = s;
}

void forsiTojikLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}