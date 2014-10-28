/// file piece.cpp
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// code having to do with moves.
///
///
#include "fiesty.h"
#include "move.h"

///
/// convert a move into a string abbr
///
std::string CMove::asAbbr() const
{
    return getFrom().asAbbr() + getTo().asAbbr();
}

///
/// convert a move collection into a string abbr
///
std::string CMoves::asAbbr() const
{
    std::string s = std::to_string( mNumMoves ) + ":";

    for ( int j = 0; j < mNumMoves; j++ )
    {
        s.append( mMoves[j].asStr() );
        if ( j < mNumMoves - 1 )
            s.append( " " );
    }
    return s;
}

///
/// constructor of a line
///
CLine::CLine()
{
    mNumMoves = 0;
    mAllocatedMoves = kDeltaMaxMoves;
    mpMoves = new CMove[kDeltaMaxMoves];
    mpHashKeys = new YHashKey[kDeltaMaxMoves];
}

///
/// convert a move collection into a string abbr
///
std::string CLine::asAbbr() const
{
    std::string s = std::to_string( mNumMoves ) + ":";

    for ( int j = 0; j < mNumMoves; j++ )
    {
        s.append( mpMoves[j].asStr() );
        if ( j < mNumMoves - 1 )
            s.append( " " );
    }
    return s;
}

///
/// convert a line into a string
///
std::string CLine::asStr() const
{
    std::string s = std::to_string( mNumMoves ) 
        + " of " + std::to_string( mAllocatedMoves )
        + ":";

    for ( int j = 0; j < mNumMoves; j++ )
    {
        s.append( mpMoves[j].asStr() );
        if ( j < mNumMoves - 1 )
            s.append( " " );
    }
    return s;
}

///
/// realloate the moves in the line
///
void CLine::reallocate()
{
    CMove*          pNewMoves;
    YHashKey*       pNewHashKeys;
    
    pNewMoves = new CMove[mAllocatedMoves + kDeltaMaxMoves];
    pNewHashKeys = new YHashKey[mAllocatedMoves + kDeltaMaxMoves];
    memcpy( pNewMoves, mpMoves, sizeof( CMove ) * mNumMoves );
    memcpy( pNewHashKeys, mpHashKeys, sizeof( YHashKey ) * mNumMoves );
    delete[] mpMoves;
    delete[] mpHashKeys;
    mpMoves = pNewMoves;
    mpHashKeys = pNewHashKeys;
    mAllocatedMoves += kDeltaMaxMoves;
}
