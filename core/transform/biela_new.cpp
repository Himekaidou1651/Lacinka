//Bielarus 2007 to Latin
#include "biela_new.h"

static void replaceAll(std::string& s, const std::string& from, const std::string& to) {
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.length(), to);
        pos += to.length();
    }
}

bielaNewLatin::bielaNewLatin(std::string input)
    : transforml(input) {
    this->type = transformType::bielaNewLatin;
}

void bielaNewLatin::transform() {
    std::string s = this->input;

    // 1) 辅音 + ь 组合（先于所有单字母）
    replaceAll(s, "\xD0\x97\xD1\x8C", "\xC5\xB9");   // Зь → Ź
    replaceAll(s, "\xD0\xB7\xD1\x8C", "\xC5\xBA");   // зь → ź
    replaceAll(s, "\xD0\xA1\xD1\x8C", "\xC5\x9A");   // Сь → Ś
    replaceAll(s, "\xD1\x81\xD1\x8C", "\xC5\x9B");   // сь → ś
    replaceAll(s, "\xD0\x9D\xD1\x8C", "\xC5\x83");   // Нь → Ń
    replaceAll(s, "\xD0\xBD\xD1\x8C", "\xC5\x84");   // нь → ń
    replaceAll(s, "\xD0\x9B\xD1\x8C", "L");          // Ль → L
    replaceAll(s, "\xD0\xBB\xD1\x8C", "l");          // ль → l
    replaceAll(s, "\xD0\xA2\xD1\x8C", "\xC4\x86");   // Ть → Ć
    replaceAll(s, "\xD1\x82\xD1\x8C", "\xC4\x87");   // ть → ć
    replaceAll(s, "\xD0\xA6\xD1\x8C", "\xC4\x86");   // Ць → Ć
    replaceAll(s, "\xD1\x86\xD1\x8C", "\xC4\x87");   // ць → ć
    replaceAll(s, "\xD0\xA0\xD1\x8C", "Rz");         // Рь → Rz
    replaceAll(s, "\xD1\x80\xD1\x8C", "rz");         // рь → rz

    // 2) 丢弃隔音符与剩余软/硬音符号
    replaceAll(s, "'", "");
    replaceAll(s, "\xE2\x80\x99", "");               // ’ (U+2019)
    replaceAll(s, "\xCA\xBC", "");                   // ʼ (U+02BC)
    replaceAll(s, "\xD1\x8C", "");                   // ь
    replaceAll(s, "\xD0\xAC", "");                   // Ь
    replaceAll(s, "\xD1\x8A", "");                   // ъ
    replaceAll(s, "\xD0\xAA", "");                   // Ъ

    // 3) 位置字母先转成 i-形（词首/元音后的 j-形在下面统一修正）
    replaceAll(s, "\xD0\xB5", "ie");   // е
    replaceAll(s, "\xD1\x91", "io");   // ё
    replaceAll(s, "\xD1\x8E", "iu");   // ю
    replaceAll(s, "\xD1\x8F", "ia");   // я
    replaceAll(s, "\xD0\x95", "Ie");   // Е
    replaceAll(s, "\xD0\x81", "Io");   // Ё
    replaceAll(s, "\xD0\xAE", "Iu");   // Ю
    replaceAll(s, "\xD0\xAF", "Ia");   // Я

    // 4) 其余单字母（小写）
    replaceAll(s, "\xD0\xB0", "a");     // а
    replaceAll(s, "\xD0\xB1", "b");     // б
    replaceAll(s, "\xD0\xB2", "v");     // в → v（新式）
    replaceAll(s, "\xD0\xB3", "h");     // г
    replaceAll(s, "\xD2\x91", "g");     // ґ
    replaceAll(s, "\xD0\xB4", "d");     // д
    replaceAll(s, "\xD0\xB6", "\xC5\xBE"); // ж → ž（新式）
    replaceAll(s, "\xD0\xB7", "z");     // з
    replaceAll(s, "\xD1\x96", "i");     // і
    replaceAll(s, "\xD0\xB9", "j");     // й
    replaceAll(s, "\xD0\xBA", "k");     // к
    replaceAll(s, "\xD0\xBB", "\xC5\x82"); // л → ł
    replaceAll(s, "\xD0\xBC", "m");     // м
    replaceAll(s, "\xD0\xBD", "n");     // н
    replaceAll(s, "\xD0\xBE", "o");     // о
    replaceAll(s, "\xD0\xBF", "p");     // п
    replaceAll(s, "\xD1\x80", "r");     // р
    replaceAll(s, "\xD1\x81", "s");     // с
    replaceAll(s, "\xD1\x82", "t");     // т
    replaceAll(s, "\xD1\x83", "u");     // у
    replaceAll(s, "\xD1\x9E", "\xC5\xAD"); // ў → ŭ
    replaceAll(s, "\xD1\x84", "f");     // ф
    replaceAll(s, "\xD1\x85", "ch");    // х
    replaceAll(s, "\xD1\x86", "c");     // ц
    replaceAll(s, "\xD1\x87", "\xC4\x8D"); // ч → č（新式）
    replaceAll(s, "\xD1\x88", "\xC5\xA1"); // ш → š（新式）
    replaceAll(s, "\xD1\x8B", "y");     // ы
    replaceAll(s, "\xD1\x8D", "e");     // э

    // 5) 其余单字母（大写）
    replaceAll(s, "\xD0\x90", "A");     // А
    replaceAll(s, "\xD0\x91", "B");     // Б
    replaceAll(s, "\xD0\x92", "V");     // В → V（新式）
    replaceAll(s, "\xD0\x93", "H");     // Г
    replaceAll(s, "\xD2\x90", "G");     // Ґ
    replaceAll(s, "\xD0\x94", "D");     // Д
    replaceAll(s, "\xD0\x96", "\xC5\xBD"); // Ж → Ž（新式）
    replaceAll(s, "\xD0\x97", "Z");     // З
    replaceAll(s, "\xD0\x86", "I");     // І
    replaceAll(s, "\xD0\x99", "J");     // Й
    replaceAll(s, "\xD0\x9A", "K");     // К
    replaceAll(s, "\xD0\x9B", "\xC5\x81"); // Л → Ł
    replaceAll(s, "\xD0\x9C", "M");     // М
    replaceAll(s, "\xD0\x9D", "N");     // Н
    replaceAll(s, "\xD0\x9E", "O");     // О
    replaceAll(s, "\xD0\x9F", "P");     // П
    replaceAll(s, "\xD0\xA0", "R");     // Р
    replaceAll(s, "\xD0\xA1", "S");     // С
    replaceAll(s, "\xD0\xA2", "T");     // Т
    replaceAll(s, "\xD0\xA3", "U");     // У
    replaceAll(s, "\xD0\x8E", "\xC5\xAC"); // Ў → Ŭ
    replaceAll(s, "\xD0\xA4", "F");     // Ф
    replaceAll(s, "\xD0\xA5", "Ch");    // Х
    replaceAll(s, "\xD0\xA6", "C");     // Ц
    replaceAll(s, "\xD0\xA7", "\xC4\x8C"); // Ч → Č（新式）
    replaceAll(s, "\xD0\xA8", "\xC5\xA0"); // Ш → Š（新式）
    replaceAll(s, "\xD0\xAB", "Y");     // Ы
    replaceAll(s, "\xD0\xAD", "E");     // Э

    // 6) 词首位置插入哨兵 \x01
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

    // 7) 反复修正词首/元音后的 i-形 → j-形（处理连续位置字母）
    std::string prev;
    do {
        prev = s;
        replaceAll(s, "\x01ie", "je");
        replaceAll(s, "\x01io", "jo");
        replaceAll(s, "\x01iu", "ju");
        replaceAll(s, "\x01ia", "ja");
        replaceAll(s, "\x01Ie", "Je");
        replaceAll(s, "\x01Io", "Jo");
        replaceAll(s, "\x01Iu", "Ju");
        replaceAll(s, "\x01Ia", "Ja");
        replaceAll(s, "aie", "aje");
        replaceAll(s, "aio", "ajo");
        replaceAll(s, "aiu", "aju");
        replaceAll(s, "aia", "aja");
        replaceAll(s, "eie", "eje");
        replaceAll(s, "eio", "ejo");
        replaceAll(s, "eiu", "eju");
        replaceAll(s, "eia", "eja");
        replaceAll(s, "iie", "ije");
        replaceAll(s, "iio", "ijo");
        replaceAll(s, "iiu", "iju");
        replaceAll(s, "iia", "ija");
        replaceAll(s, "oie", "oje");
        replaceAll(s, "oio", "ojo");
        replaceAll(s, "oiu", "oju");
        replaceAll(s, "oia", "oja");
        replaceAll(s, "uie", "uje");
        replaceAll(s, "uio", "ujo");
        replaceAll(s, "uiu", "uju");
        replaceAll(s, "uia", "uja");
        replaceAll(s, "yie", "yje");
        replaceAll(s, "yio", "yjo");
        replaceAll(s, "yiu", "yju");
        replaceAll(s, "yia", "yja");
        replaceAll(s, "\xC5\xADie", "\xC5\xADje"); // ŭie → ŭje
        replaceAll(s, "\xC5\xADio", "\xC5\xADjo");
        replaceAll(s, "\xC5\xADiu", "\xC5\xADju");
        replaceAll(s, "\xC5\xADia", "\xC5\xADja");
        replaceAll(s, "Aie", "Aje");
        replaceAll(s, "Aio", "Ajo");
        replaceAll(s, "Aiu", "Aju");
        replaceAll(s, "Aia", "Aja");
        replaceAll(s, "Eie", "Eje");
        replaceAll(s, "Eio", "Ejo");
        replaceAll(s, "Eiu", "Eju");
        replaceAll(s, "Eia", "Eja");
        replaceAll(s, "Iie", "Ije");
        replaceAll(s, "Iio", "Ijo");
        replaceAll(s, "Iiu", "Iju");
        replaceAll(s, "Iia", "Ija");
        replaceAll(s, "Oie", "Oje");
        replaceAll(s, "Oio", "Ojo");
        replaceAll(s, "Oiu", "Oju");
        replaceAll(s, "Oia", "Oja");
        replaceAll(s, "Uie", "Uje");
        replaceAll(s, "Uio", "Ujo");
        replaceAll(s, "Uiu", "Uju");
        replaceAll(s, "Uia", "Uja");
        replaceAll(s, "Yie", "Yje");
        replaceAll(s, "Yio", "Yjo");
        replaceAll(s, "Yiu", "Yju");
        replaceAll(s, "Yia", "Yja");
        replaceAll(s, "\xC5\xACIe", "\xC5\xACJe"); // Ŭie → Ŭje
        replaceAll(s, "\xC5\xACIo", "\xC5\xACJo");
        replaceAll(s, "\xC5\xACIu", "\xC5\xACJu");
        replaceAll(s, "\xC5\xACIa", "\xC5\xACJa");
    } while (s != prev);

    // 8) 去掉哨兵
    replaceAll(s, "\x01", "");

    this->output = s;
}

void bielaNewLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}