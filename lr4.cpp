#include <iostream>
#include "TScaner.h"

using namespace std;

int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "Rus");      

    TScaner* sc = NULL;

    if (argc <= 1)
        sc = new TScaner("input.txt");
    else
        sc = new TScaner(argv[1]);

    int t;
    do {
        LEX lex;
        t = sc->scaner(lex);
        cout << "Тип = " << t << " Лексема = " << lex << endl;
    } while (t != TEnd); 

    if (sc != NULL) delete sc;
    return 0;
}