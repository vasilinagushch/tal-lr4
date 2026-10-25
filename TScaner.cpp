#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include "TScaner.h"

using namespace std;

TScaner::TScaner(const string& filename) {
    ifstream in(filename);
    if (!in) {
        cout << "Отсутствует входной файл: " << filename << endl;
        exit(1);
    }
    string lineStr;
    while (getline(in, lineStr)) {
        text += lineStr + '\n'; // читаем файл построчно
    }
    uk = 0;
    line = 1;
}

void TScaner::printError(const string& err, const string& lex) {
    cout << "Ошибка (строка " << line << "): " << err;
    if (!lex.empty()) cout << " [" << lex << "]";
    cout << endl;
}

int TScaner::scaner(LEX l) {
    int i = 0;
    for (int k = 0; k < MAX_LEX; k++) l[k] = '\0';

    // пропуск незначащих символов и комментариев
    while (uk < text.length()) {
         unsigned char c = (unsigned char)text[uk];

        if (c == '\n') { line++; uk++; }
        else if (c == ' ' || c == '\t' || c == '\r') { uk++; }
        else if (c == '/' && uk + 1 < text.length() && text[uk + 1] == '/') {
            while (uk < text.length() && text[uk] != '\n') uk++;
        }
        else break;
    }

    if (uk >= text.length()) return TEnd;

    unsigned char c = (unsigned char)text[uk];

    // идентификаторы и ключевые слова
    if (isalpha(c) || c == '_') {
        bool isTooLong = false; // флаг для проверки длины
        while (uk < text.length() && (isalnum((unsigned char)text[uk]) || text[uk] == '_')) {
            if (i < MAX_LEX - 1) {
                l[i++] = text[uk];
            }
            else {
                isTooLong = true; 
            }
            uk++;
        }

        if (isTooLong) {
            printError("Слишком длинная лексема ", l);
            return TErr;
        }

        if (strcmp(l, "int") == 0) return TInt;
        if (strcmp(l, "short") == 0) return TShort;
        if (strcmp(l, "long") == 0) return TLong;
        if (strcmp(l, "double") == 0) return TDouble;
        if (strcmp(l, "struct") == 0) return TStruct;
        if (strcmp(l, "do") == 0) return TDo;
        if (strcmp(l, "while") == 0) return TWhile;
        if (strcmp(l, "main") == 0) return TMain;
        return TIdent;
    }

    // числа

    bool isDotFloat = (c == '.' && uk + 1 < text.length() && isdigit((unsigned char)text[uk + 1]));

    if (isdigit(c) || isDotFloat) {
        bool isTooLong = false;
        bool isFloat = false; 

        if (isDotFloat) {
            isFloat = true;
            l[i++] = text[uk++]; 

            // цифры после точки
            while (uk < text.length() && isdigit((unsigned char)text[uk])) {
                if (i < MAX_LEX - 1) l[i++] = text[uk];
                else isTooLong = true;
                uk++;
            }
        }
        else {
            // если число начинается с цифры 
            l[i++] = text[uk++];

            // 16-ричные
            if (l[0] == '0' && uk < text.length() && (text[uk] == 'x' || text[uk] == 'X')) {
                l[i++] = text[uk++];
                int hexStart = i;
                while (uk < text.length() && isxdigit((unsigned char)text[uk])) {
                    if (i < MAX_LEX - 1) l[i++] = text[uk];
                    else isTooLong = true;
                    uk++;
                }

                if (i == hexStart) {
                    printError("Неверная 16-ричная константа ", l);
                    return TErr;
                }

                if (isTooLong) {
                    printError("Слишком длинная 16-ричная константа", l);
                    return TErr;
                }
                return TConstInt16;
            }

            // 10-ричные
            while (uk < text.length() && isdigit((unsigned char)text[uk])) {
                if (i < MAX_LEX - 1) l[i++] = text[uk];
                else isTooLong = true;
                uk++;
            }

            // точка (если число началось с цифры, например "1.5"
            if (uk < text.length() && text[uk] == '.') {
                isFloat = true;
                l[i++] = text[uk++];
                while (uk < text.length() && isdigit((unsigned char)text[uk])) {
                    if (i < MAX_LEX - 1) l[i++] = text[uk];
                    else isTooLong = true;
                    uk++;
                }
            }
        }

        // экспонента 
        if (uk < text.length() && (text[uk] == 'e' || text[uk] == 'E')) {
            isFloat = true;
            l[i++] = text[uk++];
            if (uk < text.length() && (text[uk] == '+' || text[uk] == '-')) {
                l[i++] = text[uk++];
            }
            if (uk < text.length() && isdigit((unsigned char)text[uk])) {
                while (uk < text.length() && isdigit((unsigned char)text[uk])) {
                    if (i < MAX_LEX - 1) l[i++] = text[uk];
                    else isTooLong = true;
                    uk++;
                }
            }
            else {
                printError("Неверная вещественная константа", l);
                return TErr;
            }
        }

        if (isTooLong) {
            printError("Слишком длинная числовая константа", l);
            return TErr;
        }

        return isFloat ? TConstFloat : TConstInt10;
    }

    // операции и разделители
    l[i++] = text[uk++];
    switch (c) {
    case ',': return TComma;
    case ';': return TSemicolon;
    case '(': return TLParen;
    case ')': return TRParen;
    case '{': return TLBrace;
    case '}': return TRBrace;
    case '.': return TDot;
    case '+': return TPlus;
    case '-': return TMinus;
    case '*': return TMult;
    case '/': return TDiv;
    case '%': return TMod;
    case '=':
        if (uk < text.length() && text[uk] == '=') { l[i++] = text[uk++]; return TEq; }
        return TAssign;
    case '!':
        if (uk < text.length() && text[uk] == '=') { l[i++] = text[uk++]; return TNeq; }
        break;
    case '<':
        if (uk < text.length() && text[uk] == '=') { l[i++] = text[uk++]; return TLe; }
        return TLt;
    case '>':
        if (uk < text.length() && text[uk] == '=') { l[i++] = text[uk++]; return TGe; }
        return TGt;
    }

    printError("Неверный символ", l);
    return TErr;
}