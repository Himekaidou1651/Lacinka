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

    const std::string B_SOFT = std::string("\x02", 1) + "B";
    const std::string b_SOFT = std::string("\x02", 1) + "b";
    const std::string W_SOFT = std::string("\x02", 1) + "W";
    const std::string w_SOFT = std::string("\x02", 1) + "w";
    const std::string P_SOFT = std::string("\x02", 1) + "P";
    const std::string p_SOFT = std::string("\x02", 1) + "p";
    const std::string F_SOFT = std::string("\x02", 1) + "F";
    const std::string f_SOFT = std::string("\x02", 1) + "f";
    const std::string SOFT_AFTER = std::string("\x03", 1);
    const std::string HARD_AFTER = std::string("\x04", 1);

    // ============================================================
    // 0) 重音与不转写符号
    // ============================================================

    replaceAll(s, "\xD0\x98\xCC\x81", "\xC3\x8D"); // И́ -> Í
    replaceAll(s, "\xD0\xB8\xCC\x81", "\xC3\xAD"); // и́ -> í
    replaceAll(s, "\xE2\x80\x99", "");             // ’


    // ============================================================
    // 1) 辅音 + ь
    // ============================================================

    // Зь
    replaceAll(s, "\xD0\x97\xD1\x8C", "\xC5\xB9" + SOFT_AFTER); // Ź
    replaceAll(s, "\xD0\xB7\xD1\x8C", "\xC5\xBA" + SOFT_AFTER); // ź

    // Сь
    replaceAll(s, "\xD0\xA1\xD1\x8C", "\xC5\x9A" + SOFT_AFTER); // Ś
    replaceAll(s, "\xD1\x81\xD1\x8C", "\xC5\x9B" + SOFT_AFTER); // ś

    // Нь
    replaceAll(s, "\xD0\x9D\xD1\x8C", "\xC5\x83" + SOFT_AFTER); // Ń
    replaceAll(s, "\xD0\xBD\xD1\x8C", "\xC5\x84" + SOFT_AFTER); // ń

    // Ль
    replaceAll(s, "\xD0\x9B\xD1\x8C", "L" + SOFT_AFTER);
    replaceAll(s, "\xD0\xBB\xD1\x8C", "l" + SOFT_AFTER);

    // Ць
    replaceAll(s, "\xD0\xA6\xD1\x8C", "\xC4\x86" + SOFT_AFTER); // Ć
    replaceAll(s, "\xD1\x86\xD1\x8C", "\xC4\x87" + SOFT_AFTER); // ć

    // Рь
    replaceAll(s, "\xD0\xA0\xD1\x8C", "Rz" + SOFT_AFTER);
    replaceAll(s, "\xD1\x80\xD1\x8C", "rz" + SOFT_AFTER);

    // Дь
    replaceAll(s, "\xD0\x94\xD1\x8C", "D\xC5\xBA" + SOFT_AFTER); // Dź
    replaceAll(s, "\xD0\xB4\xD1\x8C", "d\xC5\xBA" + SOFT_AFTER); // dź

    // Мь
    replaceAll(s, "\xD0\x9C\xD1\x8C", "Mj" + SOFT_AFTER);
    replaceAll(s, "\xD0\xBC\xD1\x8C", "mj" + SOFT_AFTER);

    // Ть
    replaceAll(s, "\xD0\xA2\xD1\x8C", "\xC5\xA4" + SOFT_AFTER); // Ť
    replaceAll(s, "\xD1\x82\xD1\x8C", "\xC5\xA5" + SOFT_AFTER); // ť


    // ============================================================
    // 2) Бь / Вь / Пь / Фь
    // ============================================================

    replaceAll(s, "\xD0\x91\xD1\x8C", B_SOFT + SOFT_AFTER); // Бь
    replaceAll(s, "\xD0\xB1\xD1\x8C", b_SOFT + SOFT_AFTER); // бь

    replaceAll(s, "\xD0\x92\xD1\x8C", W_SOFT + SOFT_AFTER); // Вь
    replaceAll(s, "\xD0\xB2\xD1\x8C", w_SOFT + SOFT_AFTER); // вь

    replaceAll(s, "\xD0\x9F\xD1\x8C", P_SOFT + SOFT_AFTER); // Пь
    replaceAll(s, "\xD0\xBF\xD1\x8C", p_SOFT + SOFT_AFTER); // пь

    replaceAll(s, "\xD0\xA4\xD1\x8C", F_SOFT + SOFT_AFTER); // Фь
    replaceAll(s, "\xD1\x84\xD1\x8C", f_SOFT + SOFT_AFTER); // фь


    // ============================================================
    // 3) 保护剩余的 ь / ъ
    // ============================================================

    // ь
    replaceAll(s, "\xD1\x8C", SOFT_AFTER);
    replaceAll(s, "\xD0\xAC", SOFT_AFTER);

    // ъ
    replaceAll(s, "\xD1\x8A", HARD_AFTER);
    replaceAll(s, "\xD0\xAA", HARD_AFTER);


    // ============================================================
    // 4) 处理 ь / ъ + Е Ё Ю Я
    // ============================================================

    // ------------------------------------------------------------
    // 小写
    // ------------------------------------------------------------

    // ье -> je
    replaceAll(s, SOFT_AFTER + "\xD0\xB5", "je");

    // ьё -> jo
    replaceAll(s, SOFT_AFTER + "\xD1\x91", "jo");

    // ью -> ju
    replaceAll(s, SOFT_AFTER + "\xD1\x8E", "ju");

    // ья -> ja
    replaceAll(s, SOFT_AFTER + "\xD1\x8F", "ja");


    // ъе́ / ъю́ / ъя́
    replaceAll(s, HARD_AFTER + "\xD0\xB5\xCC\x81", "j\xC3\xA9");
    replaceAll(s, HARD_AFTER + "\xD1\x8E\xCC\x81", "j\xC3\xBA");
    replaceAll(s, HARD_AFTER + "\xD1\x8F\xCC\x81", "j\xC3\xA1");

    // ъе -> je
    replaceAll(s, HARD_AFTER + "\xD0\xB5", "je");

    // ъё -> jo
    replaceAll(s, HARD_AFTER + "\xD1\x91", "jo");

    // ъю -> ju
    replaceAll(s, HARD_AFTER + "\xD1\x8E", "ju");

    // ъя -> ja
    replaceAll(s, HARD_AFTER + "\xD1\x8F", "ja");


    // ------------------------------------------------------------
    // 大写 Е Ё Ю Я
    // ------------------------------------------------------------

    // ЬЕ / ЪЕ
    replaceAll(s, SOFT_AFTER + "\xD0\x95", "Je");
    replaceAll(s, HARD_AFTER + "\xD0\x95", "Je");

    // ЬЁ / ЪЁ
    replaceAll(s, SOFT_AFTER + "\xD0\x81", "Jo");
    replaceAll(s, HARD_AFTER + "\xD0\x81", "Jo");

    // ЬЮ / ЪЮ
    replaceAll(s, SOFT_AFTER + "\xD0\xAE", "Ju");
    replaceAll(s, HARD_AFTER + "\xD0\xAE", "Ju");

    // ЬЯ / ЪЯ
    replaceAll(s, SOFT_AFTER + "\xD0\xAF", "Ja");
    replaceAll(s, HARD_AFTER + "\xD0\xAF", "Ja");


    // ============================================================
    // 5) 删除已经处理完的 ь / ъ
    // ============================================================

    replaceAll(s, SOFT_AFTER, "");
    replaceAll(s, HARD_AFTER, "");


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
    replaceAll(s, "{", "{\x01");
    replaceAll(s, "}", "}\x01");
    replaceAll(s, "\"", "\"\x01");
    replaceAll(s, "/", "/\x01");
    replaceAll(s, "\\", "\\\x01");
    replaceAll(s, "-", "-\x01");

    replaceAll(s, "\xC2\xAB", "\xC2\xAB\x01"); // «
    replaceAll(s, "\xC2\xBB", "\xC2\xBB\x01"); // »
    replaceAll(s, "\xE2\x80\x9C", "\xE2\x80\x9C\x01"); // “
    replaceAll(s, "\xE2\x80\x9D", "\xE2\x80\x9D\x01"); // ”
    replaceAll(s, "\xE2\x80\x9E", "\xE2\x80\x9E\x01"); // „
    replaceAll(s, "\xE2\x80\x94", "\xE2\x80\x94\x01"); // —
    replaceAll(s, "\xE2\x80\x93", "\xE2\x80\x93\x01"); // –
    replaceAll(s, "\xE2\x80\xA6", "\xE2\x80\xA6\x01"); // …


    // ============================================================
    // 10) 词首 / 元音后的 ie/io/iu/ia -> je/jo/ju/ja
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

        replaceAll(s, "aIe", "aJe");
        replaceAll(s, "aIo", "aJo");
        replaceAll(s, "aIu", "aJu");
        replaceAll(s, "aIa", "aJa");

        replaceAll(s, "eIe", "eJe");
        replaceAll(s, "eIo", "eJo");
        replaceAll(s, "eIu", "eJu");
        replaceAll(s, "eIa", "eJa");

        replaceAll(s, "iIe", "iJe");
        replaceAll(s, "iIo", "iJo");
        replaceAll(s, "iIu", "iJu");
        replaceAll(s, "iIa", "iJa");

        replaceAll(s, "oIe", "oJe");
        replaceAll(s, "oIo", "oJo");
        replaceAll(s, "oIu", "oJu");
        replaceAll(s, "oIa", "oJa");

        replaceAll(s, "uIe", "uJe");
        replaceAll(s, "uIo", "uJo");
        replaceAll(s, "uIu", "uJu");
        replaceAll(s, "uIa", "uJa");

        replaceAll(s, "yIe", "yJe");
        replaceAll(s, "yIo", "yJo");
        replaceAll(s, "yIu", "yJu");
        replaceAll(s, "yIa", "yJa");


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

        replaceAll(s, "AIe", "AJe");
        replaceAll(s, "AIo", "AJo");
        replaceAll(s, "AIu", "AJu");
        replaceAll(s, "AIa", "AJa");

        replaceAll(s, "EIe", "EJe");
        replaceAll(s, "EIo", "EJo");
        replaceAll(s, "EIu", "EJu");
        replaceAll(s, "EIa", "EJa");

        replaceAll(s, "IIe", "IJe");
        replaceAll(s, "IIo", "IJo");
        replaceAll(s, "IIu", "IJu");
        replaceAll(s, "IIa", "IJa");

        replaceAll(s, "OIe", "OJe");
        replaceAll(s, "OIo", "OJo");
        replaceAll(s, "OIu", "OJu");
        replaceAll(s, "OIa", "OJa");

        replaceAll(s, "UIe", "UJe");
        replaceAll(s, "UIo", "UJo");
        replaceAll(s, "UIu", "UJu");
        replaceAll(s, "UIa", "UJa");

        replaceAll(s, "YIe", "YJe");
        replaceAll(s, "YIo", "YJo");
        replaceAll(s, "YIu", "YJu");
        replaceAll(s, "YIa", "YJa");

    } while (s != prev);


    // ============================================================
    // 11) 处理特殊 Бь / Вь / Пь / Фь
    // ============================================================

    // Бь
    replaceAll(s, "\x01" + B_SOFT, "B");
    replaceAll(s, "\x01" + b_SOFT, "b");

    replaceAll(s, B_SOFT, "Bj");
    replaceAll(s, b_SOFT, "bj");


    // Вь
    replaceAll(s, "\x01" + W_SOFT, "W");
    replaceAll(s, "\x01" + w_SOFT, "w");

    replaceAll(s, W_SOFT, "Wj");
    replaceAll(s, w_SOFT, "wj");


    // Пь
    replaceAll(s, "\x01" + P_SOFT, "P");
    replaceAll(s, "\x01" + p_SOFT, "p");

    replaceAll(s, P_SOFT, "Pj");
    replaceAll(s, p_SOFT, "pj");


    // Фь
    replaceAll(s, "\x01" + F_SOFT, "F");
    replaceAll(s, "\x01" + f_SOFT, "f");

    replaceAll(s, F_SOFT, "Fj");
    replaceAll(s, f_SOFT, "fj");


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
