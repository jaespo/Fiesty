/// file search.h
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// headers having to do with search
///
///
#ifndef Fiesty_search_h
#define Fiesty_search_h

#include "position.h"

class CVal
{
public:
    YVal get() { return mVal; }

private:
    YVal        mVal;

};

///
/// Class that does the searching
///
class CSearcher
{
public:
    CSearcher( CPos& rPos ) { mpPos = &rPos; }
    void determineBestMove( CMove& rBestMoves ); // Todo ...
    U64 perft( U16 depthLeft ); // Todo...

private:
    CPos*           mpPos;
    CMoves          mBestMoves;

    U64 perftWhite( U16 depthLeft );
    U64 perftBlack( U16 depthLeft );
    YVal alphaBeta( YVal lowerBound, YVal upperBound, U16 depthLeft ); // Todo ...
    YVal qsearch( YVal lowerBound, YVal upperBound );   // Todo ...
};
#endif 
