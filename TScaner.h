#include <string>
#include "defs.h"

class TScaner {
private:
    std::string text; 
    int uk, line;     // текущая позиция и строка

public:
    TScaner(const std::string& filename);
    ~TScaner() {} 

    void setUK(int i) { uk = i; }
    int getUK() const { return uk; }
    void setLine(int i) { line = i; }
    int getLine() const { return line; }

    void printError(const std::string& err, const std::string& lex);
    int scaner(LEX l);
};

