//Hayastan Latin
#include "hayeren_big.h"

hayerenLatin::hayerenLatin(std::string input)
    : transforml(input) {
    this->type = transformType::hayerenLatin;
}

void hayerenLatin::transform() {
    std::string s = this->input;

    // 二合字母
    replaceAll(s, "\u0548\u0582", "U");  // Ու → U
    replaceAll(s, "\u0578\u0582", "u");  // ու → u

    // 大写
    replaceAll(s, "\u0531", "A");     // Ա
    replaceAll(s, "\u0532", "B");     // Բ
    replaceAll(s, "\u0533", "G");     // Գ
    replaceAll(s, "\u0534", "D");     // Դ
    replaceAll(s, "\u0535", "E");     // Ե
    replaceAll(s, "\u0536", "Z");     // Զ
    replaceAll(s, "\u0537", "Ê");     // Է
    replaceAll(s, "\u0538", "Ë");     // Ը
    replaceAll(s, "\u0539", "Th");    // Թ
    replaceAll(s, "\u053A", "Ž");     // Ժ
    replaceAll(s, "\u053B", "I");     // Ի
    replaceAll(s, "\u053C", "L");     // Լ
    replaceAll(s, "\u053D", "X");     // Խ
    replaceAll(s, "\u053E", "C");     // Ծ
    replaceAll(s, "\u053F", "K");     // Կ
    replaceAll(s, "\u0540", "H");     // Հ
    replaceAll(s, "\u0541", "Dz");    // Ձ
    replaceAll(s, "\u0542", "Ł");     // Ղ
    replaceAll(s, "\u0543", "Č");     // Ճ
    replaceAll(s, "\u0544", "M");     // Մ
    replaceAll(s, "\u0545", "Y");     // Յ
    replaceAll(s, "\u0546", "N");     // Ն
    replaceAll(s, "\u0547", "Š");     // Շ
    replaceAll(s, "\u0548", "O");     // Ո
    replaceAll(s, "\u0549", "Čč");    // Չ
    replaceAll(s, "\u054A", "P");     // Պ
    replaceAll(s, "\u054B", "J");     // Ջ
    replaceAll(s, "\u054C", "Rr");    // Ռ
    replaceAll(s, "\u054D", "S");     // Ս
    replaceAll(s, "\u054E", "V");     // Վ
    replaceAll(s, "\u054F", "T");     // Տ
    replaceAll(s, "\u0550", "R");     // Ր
    replaceAll(s, "\u0551", "Cc");    // Ց
    replaceAll(s, "\u0552", "W");     // Ւ
    replaceAll(s, "\u0553", "Ph");    // Փ
    replaceAll(s, "\u0554", "Kh");    // Ք
    replaceAll(s, "\u0555", "Ō");     // Օ
    replaceAll(s, "\u0556", "F");     // Ֆ

    // 小写
    replaceAll(s, "\u0561", "a");     // ա
    replaceAll(s, "\u0562", "b");     // բ
    replaceAll(s, "\u0563", "g");     // գ
    replaceAll(s, "\u0564", "d");     // դ
    replaceAll(s, "\u0565", "e");     // ե
    replaceAll(s, "\u0566", "z");     // զ
    replaceAll(s, "\u0567", "ê");     // է
    replaceAll(s, "\u0568", "ë");     // ը
    replaceAll(s, "\u0569", "th");    // թ
    replaceAll(s, "\u056A", "ž");     // ժ
    replaceAll(s, "\u056B", "i");     // ի
    replaceAll(s, "\u056C", "l");     // լ
    replaceAll(s, "\u056D", "x");     // խ
    replaceAll(s, "\u056E", "c");     // ծ
    replaceAll(s, "\u056F", "k");     // կ
    replaceAll(s, "\u0570", "h");     // հ
    replaceAll(s, "\u0571", "dz");    // ձ
    replaceAll(s, "\u0572", "ł");     // ղ
    replaceAll(s, "\u0573", "č");     // ճ
    replaceAll(s, "\u0574", "m");     // մ
    replaceAll(s, "\u0575", "y");     // յ
    replaceAll(s, "\u0576", "n");     // ն
    replaceAll(s, "\u0577", "š");     // շ
    replaceAll(s, "\u0578", "o");     // ո
    replaceAll(s, "\u0579", "čč");    // չ
    replaceAll(s, "\u057A", "p");     // պ
    replaceAll(s, "\u057B", "j");     // ջ
    replaceAll(s, "\u057C", "rr");    // ռ
    replaceAll(s, "\u057D", "s");     // ս
    replaceAll(s, "\u057E", "v");     // վ
    replaceAll(s, "\u057F", "t");     // տ
    replaceAll(s, "\u0580", "r");     // ր
    replaceAll(s, "\u0581", "w");     // ւ
    replaceAll(s, "\u0582", "cc");    // ց
    replaceAll(s, "\u0583", "ph");    // փ
    replaceAll(s, "\u0584", "kh");    // ք
    replaceAll(s, "\u0585", "ō");     // օ
    replaceAll(s, "\u0586", "f");     // ֆ
    replaceAll(s, "\u0587", "ew");    // և → ew

    this->output = s;
}

void hayerenLatin::outputl() {
    this->transform();
    std::cout << this->output << std::endl;
}