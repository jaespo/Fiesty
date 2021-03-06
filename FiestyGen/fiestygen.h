/// file fiestygen.h
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// header file that has to with generating bitboards which are then compiled
/// in to Fiesty as static constants.
///
#ifndef Fiesty_gen_h
#define Fiesty_gen_h
#include "fiesty.h"
#include "square.h"

///
/// CGennerator contains static methods that generate various move sets and 
/// magics.
///
class CFiestyGen
{
public:
    static void generate();

private: 
    static void genKingAttacks();
    static void genKnightAttacks();
	static void genRookRays();
	static void genBishopRays();
    static void printBitBoardDiagram( std::string& rLabel, CBitBoard bb );
};

#endif
