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
/// Class that contains rank and file rays by direction, to the edge of the 
/// board from the indexed square (not including the indexed square).
///
class CRookRays
{
    YBitBoard       mbbNorth;
    YBitBoard       mbbEast;
    YBitBoard       mbbSouth;
    YBitBoard       mbbWest;
};

///
/// Class that contains diagnol rays by direction, to the edge of the 
/// board from the indexed square (not including the indexed square).
///
class CBishopRays
{
    YBitBoard       mbbNorthEast;
    YBitBoard       mbbSouthEast;
    YBitBoard       mbbSouthWest;
    YBitBoard       mbbNorthWest;
};

///
/// CGen conatains various generated move sets and magics
///
class CGen
{
public:
    static void generate();

    //
    //  For each square, a bit board of the squares a knight attacks
    //
    static const YBitBoard      bbKnightAttacks[CSqix::kNumSquares];

    //
    //  For each square, a bit board of the ray from that square to the 
    //  edge of the board (not including the indexed square).
    //
    static const CRookRays      bbRookRays[CSqix::kNumSquares];

    //
    //  For each square, a bit board of the ray from that square to the 
    //  edge of the board (not including the indexed square).
    //
    // static const CBishopRays    bbBishopRays[CSqix::kNumSquares];

private: 
    static void genKnightAttacks();
    static void genRookRays();
};

#endif
