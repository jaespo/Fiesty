/// file test.h
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// Simple test framework defines 
///
#include "fiesty.h"

///
/// Test class for the fiesty chess engine
///
class CTester
{
public:
    static void testAll();

    static bool isInSuite() { return mgbInSuite; }
    static int incrOkCount() { return mgOkCount++; }

    static void beginSuite( char* pzSuiteName );
    static void endSuite();

private:
    static void testPiece();
    static void testPosition();
    static void testMove();
    static void testBitBoard();
    static void testSquare();

    static int          mgOkCount;
    static char*        mgCurSuiteName;
    static bool         mgbInSuite;
};
    
//
//  We use a define rather than a template function to get around type 
//  ambiguity issues between std::string and char*
//  
#define TESTEQ( testName, expr1, expr2 )                                    \
    if ( expr1 == expr2 )                                                   \
    {                                                                       \
        if ( !CTester::isInSuite() )                                        \
            std::cout << testName << " ok" << std::endl << std::flush;      \
            CTester::incrOkCount();                                         \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        std::cout                                                           \
            << testName << " no, " << __FILE__ << "(" << __LINE__ << ")"    \
            << std::endl;                                                   \
        std::cout                                                           \
            << "    x1 = " << ( expr1 ) << std::endl                        \
            << "    x2 = " << ( expr2 ) << std::endl << std::flush;         \
    }
