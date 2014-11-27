/// file generator.h
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
class CGenerator
{
public:
    static void generate();

private: 
    static void genKnightAttacks();
};

#endif
