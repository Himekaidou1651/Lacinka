//Russ Lacinka
#include "rusnew_old.h"

static void replaceAll(std::string& s, const std::string& from, const std::string& to) {
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.length(), to);
        pos += to.length();
    }
}

rusnewOldLatin::rusnewOldLatin(std::string input)
    : transforml(input) {
    this->type = transformType::rusnewOldLatin;
}

void rusnewOldLatin::transform() {
    std::string s = this->input;

    // ============================================================
    // 内部标记
    //
    // \x01 : 词首/边界哨兵
    // \x02 : 软硬音符号及特殊软辅音的内部状态
    //
    // 正常俄语文本不会使用这些控制字符。
    // ============================================================


    // ============================================================
    // 1) 辅音 + ь
    // ============================================================

    // Зь
    replaceAll(s, "\xD0\x97\xD1\x8C", "\xC5\xB9"); // Ź
    replaceAll(s, "\xD0\xB7\xD1\x8C", "\xC5\xBA"); // ź

    // Сь
    replaceAll(s, "\xD0\xA1\xD1\x8C", "\xC5\x9A"); // Ś
    replaceAll(s, "\xD1\x81\xD1\x8C", "\xC5\x9B"); // ś

    // Нь
    replaceAll(s, "\xD0\x9D\xD1\x8C", "\xC5\x83"); // Ń
    replaceAll(s, "\xD0\xBD\xD1\x8C", "\xC5\x84"); // ń

    // Ль
    replaceAll(s, "\xD0\x9B\xD1\x8C", "L");
    replaceAll(s, "\xD0\xBB\xD1\x8C", "l");

    // Ць
    replaceAll(s, "\xD0\xA6\xD1\x8C", "\xC4\x86"); // Ć
    replaceAll(s, "\xD1\x86\xD1\x8C", "\xC4\x87"); // ć

    // Рь
    replaceAll(s, "\xD0\xA0\xD1\x8C", "Rz");
    replaceAll(s, "\xD1\x80\xD1\x8C", "rz");

    // Дь
    replaceAll(s, "\xD0\x94\xD1\x8C", "D\xC5\xBA"); // Dź
    replaceAll(s, "\xD0\xB4\xD1\x8C", "d\xC5\xBA"); // dź

    // Мь
    replaceAll(s, "\xD0\x9C\xD1\x8C", "Mj");
    replaceAll(s, "\xD0\xBC\xD1\x8C", "mj");

    // Ть
    replaceAll(s, "\xD0\xA2\xD1\x8C", "\xC5\xA4"); // Ť
    replaceAll(s, "\xD1\x82\xD1\x8C", "\xC5\xA5"); // ť


    // ============================================================
    // 2) Бь / Вь / Пь / Фь
    // ============================================================

    replaceAll(s, "\xD0\x91\xD1\x8C", "\x02B"); // Бь
    replaceAll(s, "\xD0\xB1\xD1\x8C", "\x02b"); // бь

    replaceAll(s, "\xD0\x92\xD1\x8C", "\x02W"); // Вь
    replaceAll(s, "\xD0\xB2\xD1\x8C", "\x02w"); // вь

    replaceAll(s, "\xD0\x9F\xD1\x8C", "\x02P"); // Пь
    replaceAll(s, "\xD0\xBF\xD1\x8C", "\x02p"); // пь

    replaceAll(s, "\xD0\xA4\xD1\x8C", "\x02F"); // Фь
    replaceAll(s, "\xD1\x84\xD1\x8C", "\x02f"); // фь


    // ============================================================
    // 3) 保护剩余的 ь / ъ
    // ============================================================

    // ь
    replaceAll(s, "\xD1\x8C", "\x02s");
    replaceAll(s, "\xD0\xAC", "\x02S");

    // ъ
    replaceAll(s, "\xD1\x8A", "\x02h");
    replaceAll(s, "\xD0\xAA", "\x02H");


    // ============================================================
    // 4) 处理 ь / ъ + Е Ё Ю Я
    // ============================================================

    // ------------------------------------------------------------
    // 小写
    // ------------------------------------------------------------

    // ье -> je
    replaceAll(s, "\x02s\xD0\xB5", "je");

    // ьё -> jo
    replaceAll(s, "\x02s\xD1\x91", "jo");

    // ью -> ju
    replaceAll(s, "\x02s\xD1\x8E", "ju");

    // ья -> ja
    replaceAll(s, "\x02s\xD1\x8F", "ja");


    // ъе -> je
    replaceAll(s, "\x02h\xD0\xB5", "je");

    // ъё -> jo
    replaceAll(s, "\x02h\xD1\x91", "jo");

    // ъю -> ju
    replaceAll(s, "\x02h\xD1\x8E", "ju");

    // ъя -> ja
    replaceAll(s, "\x02h\xD1\x8F", "ja");


    // ------------------------------------------------------------
    // 大写 Е Ё Ю Я
    // ------------------------------------------------------------

    // ЬЕ / ЪЕ
    replaceAll(s, "\x02S\xD0\x95", "Je");
    replaceAll(s, "\x02H\xD0\x95", "Je");

    // ЬЁ / ЪЁ
    replaceAll(s, "\x02S\xD0\x81", "Jo");
    replaceAll(s, "\x02H\xD0\x81", "Jo");

    // ЬЮ / ЪЮ
    replaceAll(s, "\x02S\xD0\xAE", "Ju");
    replaceAll(s, "\x02H\xD0\xAE", "Ju");

    // ЬЯ / ЪЯ
    replaceAll(s, "\x02S\xD0\xAF", "Ja");
    replaceAll(s, "\x02H\xD0\xAF", "Ja");


    // ============================================================
    // 5) 删除已经处理完的 ь / ъ
    // ============================================================

    replaceAll(s, "\x02s", "");
    replaceAll(s, "\x02S", "");
    replaceAll(s, "\x02h", "");
    replaceAll(s, "\x02H", "");


    // ============================================================
    // 6) Е Ё Ю Я
    // ============================================================

    // 小写
    replaceAll(s, "\xD0\xB5", "ie"); // е
    replaceAll(s, "\xD1\x91", "io"); // ё
    replaceAll(s, "\xD1\x8E", "iu"); // ю
    replaceAll(s, "\xD1\x8F", "ia"); // я

    // 大写
    replaceAll(s, "\xD0\x95", "Ie"); // Е
    replaceAll(s, "\xD0\x81", "Io"); // Ё
    replaceAll(s, "\xD0\xAE", "Iu"); // Ю
    replaceAll(s, "\xD0\xAF", "Ia"); // Я


    // ============================================================
    // 7) 其余单字母（小写）
    // ============================================================

    replaceAll(s, "\xD0\xB0", "a");      // а
    replaceAll(s, "\xD0\xB1", "b");      // б
    replaceAll(s, "\xD0\xB2", "w");      // в
    replaceAll(s, "\xD0\xB3", "g");      // г
    replaceAll(s, "\xD0\xB4", "d");      // д
    replaceAll(s, "\xD0\xB6", "\xC5\xBC");// ж -> ż
    replaceAll(s, "\xD0\xB7", "z");      // з
    replaceAll(s, "\xD0\xB8", "i");      // и
    replaceAll(s, "\xD0\xB9", "j");      // й
    replaceAll(s, "\xD0\xBA", "k");      // к
    replaceAll(s, "\xD0\xBB", "\xC5\x82");// л -> ł
    replaceAll(s, "\xD0\xBC", "m");      // м
    replaceAll(s, "\xD0\xBD", "n");      // н
    replaceAll(s, "\xD0\xBE", "o");      // о
    replaceAll(s, "\xD0\xBF", "p");      // п
    replaceAll(s, "\xD1\x80", "r");      // р
    replaceAll(s, "\xD1\x81", "s");      // с
    replaceAll(s, "\xD1\x82", "t");      // т
    replaceAll(s, "\xD1\x83", "u");      // у
    replaceAll(s, "\xD1\x84", "f");      // ф
    replaceAll(s, "\xD1\x85", "ch");     // х
    replaceAll(s, "\xD1\x86", "c");      // ц
    replaceAll(s, "\xD1\x87", "cz");     // ч
    replaceAll(s, "\xD1\x88", "sz");     // ш
    replaceAll(s, "\xD1\x89", "szcz");   // щ
    replaceAll(s, "\xD1\x8B", "y");      // ы
    replaceAll(s, "\xD1\x8D", "e");      // э


    // ============================================================
    // 8) 其余单字母（大写）
    // ============================================================

    replaceAll(s, "\xD0\x90", "A");      // А
    replaceAll(s, "\xD0\x91", "B");      // Б
    replaceAll(s, "\xD0\x92", "W");      // В
    replaceAll(s, "\xD0\x93", "G");      // Г
    replaceAll(s, "\xD0\x94", "D");      // Д
    replaceAll(s, "\xD0\x96", "\xC5\xBB");// Ж -> Ż
    replaceAll(s, "\xD0\x97", "Z");      // З
    replaceAll(s, "\xD0\x98", "I");      // И
    replaceAll(s, "\xD0\x99", "J");      // Й
    replaceAll(s, "\xD0\x9A", "K");      // К
    replaceAll(s, "\xD0\x9B", "\xC5\x81");// Л -> Ł
    replaceAll(s, "\xD0\x9C", "M");      // М
    replaceAll(s, "\xD0\x9D", "N");      // Н
    replaceAll(s, "\xD0\x9E", "O");      // О
    replaceAll(s, "\xD0\x9F", "P");      // П
    replaceAll(s, "\xD0\xA0", "R");      // Р
    replaceAll(s, "\xD0\xA1", "S");      // С
    replaceAll(s, "\xD0\xA2", "T");      // Т
    replaceAll(s, "\xD0\xA3", "U");      // У
    replaceAll(s, "\xD0\xA4", "F");      // Ф
    replaceAll(s, "\xD0\xA5", "Ch");     // Х
    replaceAll(s, "\xD0\xA6", "C");      // Ц
    replaceAll(s, "\xD0\xA7", "Cz");     // Ч
    replaceAll(s, "\xD0\xA8", "Sz");     // Ш
    replaceAll(s, "\xD0\xA9", "Szcz");   // Щ
    replaceAll(s, "\xD0\xAB", "Y");      // Ы
    replaceAll(s, "\xD0\xAD", "E");      // Э


    // ============================================================
    // 9) 建立词首哨兵
    // ============================================================

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

    replaceAll(s, "\xC2\xAB", "\xC2\xAB\x01"); // «
    replaceAll(s, "\xC2\xBB", "\xC2\xBB\x01"); // »
    replaceAll(s, "\xE2\x80\x94", "\xE2\x80\x94\x01"); // —
    replaceAll(s, "\xE2\x80\x93", "\xE2\x80\x93\x01"); // –
    replaceAll(s, "\xE2\x80\xA6", "\xE2\x80\xA6\x01"); // …


    // ============================================================
    // 10) 词首 / 元音后的 ie/io/iu/ia → je/jo/ju/ja
    // ============================================================

    std::string prev;

    do {
        prev = s;

        // --------------------------------------------------------
        // 词首
        // --------------------------------------------------------

        replaceAll(s, "\x01ie", "je");
        replaceAll(s, "\x01io", "jo");
        replaceAll(s, "\x01iu", "ju");
        replaceAll(s, "\x01ia", "ja");

        replaceAll(s, "\x01Ie", "Je");
        replaceAll(s, "\x01Io", "Jo");
        replaceAll(s, "\x01Iu", "Ju");
        replaceAll(s, "\x01Ia", "Ja");


        // --------------------------------------------------------
        // 小写元音后
        // --------------------------------------------------------

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


        // --------------------------------------------------------
        // 大写元音后
        // --------------------------------------------------------

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

    } while (s != prev);


    // ============================================================
    // 11) 处理特殊 Бь / Вь / Пь / Фь
    // ============================================================

    // Бь
    replaceAll(s, "\x01\x02B", "B");
    replaceAll(s, "\x01\x02b", "b");

    replaceAll(s, "\x02B", "Bj");
    replaceAll(s, "\x02b", "bj");


    // Вь
    replaceAll(s, "\x01\x02W", "W");
    replaceAll(s, "\x01\x02w", "w");

    replaceAll(s, "\x02W", "Wj");
    replaceAll(s, "\x02w", "wj");


    // Пь
    replaceAll(s, "\x01\x02P", "P");
    replaceAll(s, "\x01\x02p", "p");

    replaceAll(s, "\x02P", "Pj");
    replaceAll(s, "\x02p", "pj");


    // Фь
    replaceAll(s, "\x01\x02F", "F");
    replaceAll(s, "\x01\x02f", "f");

    replaceAll(s, "\x02F", "Fj");
    replaceAll(s, "\x02f", "fj");


    // ============================================================
    // 12) 删除词首哨兵
    // ============================================================

    replaceAll(s, "\x01", "");


    // ============================================================
    // 13) 输出
    // ============================================================

    this->output = s;
}

void rusnewOldLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}