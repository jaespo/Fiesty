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
class CSearch
{
public:
    CSearch( CPos& rPos ) { mpPos = &rPos; }
    void determineBestMove( CMove& rBestMoves ); // Todo ...

private:
    CPos*           mpPos;
    CMoves          mBestMoves;

    YVal alphaBeta( YVal lowerBound, YVal upperBound, U16 depthLeft ); // Todo ...
    YVal qsearch( YVal lowerBound, YVal upperBound );   // Todo ...
};

#endif 
