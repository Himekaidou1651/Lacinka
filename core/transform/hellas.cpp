//Hellas to Latin
#include "hellas.h"

hellasLatin::hellasLatin(std::string input)
    : transforml(input) {
    this->type = transformType::hellasLatin;
}

void hellasLatin::transform() {
    std::string s = this->input;

    // 三字母组合
    replaceAll(s, "\xCE\xBC\xCF\x80\xCF\x81", "br");    // μπρ → br
    replaceAll(s, "\xCD\x9C\xCF\x80\xCF\x81", "Br");    // ΜΠΡ → Br
    replaceAll(s, "\xCE\xBD\xCF\x84\xCF\x81", "dr");    // ντρ → dr
    replaceAll(s, "\xCD\x9D\xCF\x84\xCF\x81", "Dr");    // ΝΤΡ → Dr
    replaceAll(s, "\xCE\xB3\xCE\xBA\xCF\x81", "gr");    // γκρ → gr
    replaceAll(s, "\xCD\x93\xCE\xBA\xCF\x81", "Gr");    // ΓΚΡ → Gr
    // 双字母组合
    replaceAll(s, "\xCE\xBF\xCF\x85", "ou");            // ου → ou
    replaceAll(s, "\xCE\x9F\xCE\xA5", "Ou");            // ΟΥ → Ou
    replaceAll(s, "\xCE\xB1\xCF\x85", "au");            // αυ → au
    replaceAll(s, "\xCE\x91\xCE\xA5", "Au");            // ΑΥ → Au
    replaceAll(s, "\xCE\xB5\xCF\x85", "eu");            // ευ → eu
    replaceAll(s, "\xCE\x95\xCE\xA5", "Eu");            // ΕΥ → Eu
    replaceAll(s, "\xCE\xB7\xCF\x85", "\xC4\x93u");     // ηυ → ēu
    replaceAll(s, "\xCE\x97\xCE\xA5", "\xC4\x92u");     // ΗΥ → Ēu
    replaceAll(s, "\xCE\xB3\xCE\xB3", "ng");            // γγ → ng
    replaceAll(s, "\xCE\x93\xCE\x93", "Ng");            // ΓΓ → Ng
    replaceAll(s, "\xCE\xB3\xCF\x87", "nx");            // γχ → nx
    replaceAll(s, "\xCE\x93\xCE\xA7", "Nx");            // ΓΧ → Nx
    replaceAll(s, "\xCF\x83\xCF\x87", "\xC5\xA1");      // σχ → š
    replaceAll(s, "\xCE\xA3\xCE\xA7", "\xC5\xA0");      // ΣΧ → Š
    replaceAll(s, "\xCF\x84\xCF\x83", "c");             // τσ → c
    replaceAll(s, "\xCE\xA4\xCE\xA3", "C");             // ΤΣ → C
    replaceAll(s, "\xCF\x84\xCE\xB6", "dz");            // τζ → dz
    replaceAll(s, "\xCE\xA4\xCE\x96", "Dz");            // ΤΖ → Dz
    replaceAll(s, "\xCE\xBC\xCF\x80", "mb");            // μπ → mb
    replaceAll(s, "\xCE\x9C\xCE\xA0", "Mb");            // ΜΠ → Mb
    replaceAll(s, "\xCE\xBD\xCF\x84", "nd");            // ντ → nd
    replaceAll(s, "\xCE\x9D\xCE\xA4", "Nd");            // ΝΤ → Nd
    replaceAll(s, "\xCE\xB3\xCE\xBA", "ng");            // γκ → ng
    replaceAll(s, "\xCE\x93\xCE\x9A", "Ng");            // ΓΚ → Ng
    // 单字母（小写）
    replaceAll(s, "\xCE\xB1", "a");                   // α
    replaceAll(s, "\xCE\xB2", "b");                   // β
    replaceAll(s, "\xCE\xB3", "g");                   // γ
    replaceAll(s, "\xCE\xB4", "d");                   // δ
    replaceAll(s, "\xCE\xB5", "e");                   // ε
    replaceAll(s, "\xCE\xB6", "z");                   // ζ
    replaceAll(s, "\xCE\xB7", "\xC4\x93");            // η → ē
    replaceAll(s, "\xCE\xB8", "th");                  // θ
    replaceAll(s, "\xCE\xB9", "i");                   // ι
    replaceAll(s, "\xCE\xBA", "k");                   // κ
    replaceAll(s, "\xCE\xBB", "l");                   // λ
    replaceAll(s, "\xCE\xBC", "m");                   // μ
    replaceAll(s, "\xCE\xBD", "n");                   // ν
    replaceAll(s, "\xCE\xBE", "ks");                  // ξ
    replaceAll(s, "\xCE\xBF", "o");                   // ο
    replaceAll(s, "\xCF\x80", "p");                   // π
    replaceAll(s, "\xCF\x81", "r");                   // ρ
    replaceAll(s, "\xCF\x82", "s");                   // ς
    replaceAll(s, "\xCF\x83", "s");                   // σ
    replaceAll(s, "\xCF\x84", "t");                   // τ
    replaceAll(s, "\xCF\x85", "y");                   // υ
    replaceAll(s, "\xCF\x86", "ph");                  // φ
    replaceAll(s, "\xCF\x87", "x");                   // χ
    replaceAll(s, "\xCF\x88", "ps");                  // ψ
    replaceAll(s, "\xCF\x89", "\xC5\x8D");            // ω → ō
    // 单字母（大写）
    replaceAll(s, "\xCE\x91", "A");    // Α → A
    replaceAll(s, "\xCE\x92", "B");    // Β → B
    replaceAll(s, "\xCE\x93", "G");    // Γ → G
    replaceAll(s, "\xCE\x94", "D");    // Δ → D
    replaceAll(s, "\xCE\x95", "E");    // Ε → E
    replaceAll(s, "\xCE\x96", "Z");    // Ζ → Z
    replaceAll(s, "\xCE\x97", "\xC4\x92"); // Η → Ē
    replaceAll(s, "\xCE\x98", "Th");   // Θ → Th
    replaceAll(s, "\xCE\x99", "I");    // Ι → I
    replaceAll(s, "\xCE\x9A", "K");    // Κ → K
    replaceAll(s, "\xCE\x9B", "L");    // Λ → L
    replaceAll(s, "\xCE\x9C", "M");    // Μ → M
    replaceAll(s, "\xCE\x9D", "N");    // Ν → N
    replaceAll(s, "\xCE\x9E", "Ks");   // Ξ → Ks
    replaceAll(s, "\xCE\x9F", "O");    // Ο → O
    replaceAll(s, "\xCE\xA0", "P");    // Π → P
    replaceAll(s, "\xCE\xA1", "R");    // Ρ → R
    replaceAll(s, "\xCE\xA3", "S");    // Σ → S
    replaceAll(s, "\xCE\xA4", "T");    // Τ → T
    replaceAll(s, "\xCE\xA5", "Y");    // Υ → Y
    replaceAll(s, "\xCE\xA6", "Ph");   // Φ → Ph
    replaceAll(s, "\xCE\xA7", "X");    // Χ → X
    replaceAll(s, "\xCE\xA8", "Ps");   // Ψ → Ps
    replaceAll(s, "\xCE\xA9", "\xC5\x8C"); // Ω → Ō
    // 大写带重音
    replaceAll(s, "\xCE\x86", "\xC3\x81");   // Ά → Á
    replaceAll(s, "\xCE\x88", "\xC3\x89");   // Έ → É
    replaceAll(s, "\xCE\x89", "\xC7\xBC"); // Ή → Ǽ
    replaceAll(s, "\xCE\x8A", "\xC3\x8D");   // Ί → Í
    replaceAll(s, "\xCE\x8C", "\xC3\x93");   // Ό → Ó
    replaceAll(s, "\xCE\x8E", "\xC3\x9D");   // Ύ → Ý
    replaceAll(s, "\xCE\x8F", "\xE1\xB9\x92"); // Ώ → Ṓ
    // 小写带重音
    replaceAll(s, "\xCE\xAC", "\xC3\xA1");   // ά → á
    replaceAll(s, "\xCE\xAD", "\xC3\xA9");   // έ → é
    replaceAll(s, "\xCE\xAE", "\xC7\xBD"); // ή → ǽ
    replaceAll(s, "\xCE\xAF", "\xC3\xAD");   // ί → í
    replaceAll(s, "\xCF\x8C", "\xC3\xB3");   // ό → ó
    replaceAll(s, "\xCF\x8D", "\xC3\xBD");   // ύ → ý
    replaceAll(s, "\xCF\x8E", "\xE1\xB9\x93"); // ώ → ṓ

    this->output = s;
}

void hellasLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}