/// file gen.h
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// header that was generated.  Run fiesty with the "gen" option and then 
/// paste the results in here.
///
///
#ifndef Fiesty_position_h
#define Fiesty_position_h
#include "fiesty.h"
#include "square.h"

///
/// CGen conatains various generated move sets and magics
///
class CGen
{
public:
    static void generate();

    static const YBitBoard bbKnightMoveSet[CSqix::kNumSquares];

private: 
    static void genKnightMoveSet();
};

#endif
