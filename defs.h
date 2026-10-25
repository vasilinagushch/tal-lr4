#define MAX_LEX 20 

typedef char LEX[MAX_LEX]; 

enum TypeLex {
    TInt = 1, TShort = 2, TLong = 3, TDouble = 4,
    TStruct = 5, TDo = 6, TWhile = 7, TMain = 8,
    TIdent = 10,
    TConstInt10 = 30, TConstInt16 = 31, TConstFloat = 32,
    TComma = 41, TSemicolon = 42, TLParen = 43, TRParen = 44,
    TLBrace = 45, TRBrace = 46, TAssign = 47, TEq = 48, TNeq = 49,
    TLt = 50, TLe = 51, TGt = 52, TGe = 53,
    TPlus = 54, TMinus = 55, TMult = 56, TDiv = 57, TMod = 58, TDot = 59,
    TEnd = 100, TErr = 200
};

