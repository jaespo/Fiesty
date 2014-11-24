/// file gen.h
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// header that was generated.  Run fiesty with the "gen" option and then 
/// paste the results in here.
///
///
#ifndef Fiesty_gen_h
#define Fiesty_gen_h
#include "fiesty.h"
#include "square.h"

///
/// CGen conatains various generated move sets and magics
///
class CGen
{
public:
    static void generate();

#if !defined( FIESTYGEN )
    static const YBitBoard bbKnightMoveSet[CSqix::kNumSquares];
#endif

private: 
    static void genKnightMoveSet();
};

#endif
