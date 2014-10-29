/// file move.h
///
///  Fiesty (C) 2014 by Jeffery A Esposito
///
///  header having to do with moves.
///
///
#ifndef Fiesty_move_h
#define Fiesty_move_h
#include "fiesty.h"
#include "piece.h"
#include "square.h"

///
/// Class that represents a chess move, containg the from and 
/// to squares and the promotion piece if any.  
///
/// The promotion piece type is stored in the 3 most significan bits
/// of mTo.  A non-promotion is stored as a piece-type of kPawn
///
class CMove
{
public:
    CMove() {}

    CMove( CSqix f, CSqix t, CPieceType pt ) 
    {
        mFrom = f.get(); 
        mTo = t.get() | ( U8( pt.get() ) << kPromoShift );
    }

    CMove( CSqix f, CSqix t ) 
    {
        mFrom = f.get(); 
        mTo = t.get(); 
    }

    CSqix getFrom() const { return mFrom; }
    CSqix getTo() const { return mTo.get() & kToMask; }
    CPieceType getPromo() const 
    { 
        return CPieceType( EPieceType( mTo.get() >> kPromoShift ) );
    }
    void setFrom( CSqix s ) { mFrom = s.get(); }
    void setTo( CSqix s ) { mTo = s.get(); }
    void setPromo( CPieceType pt ) { mTo = getTo().get() | ( U8( pt.get() ) << 5 ); }

    std::string asAbbr() const;
    std::string asStr() const { return asAbbr(); }

private:
    static const U8     kToMask = 0x7;
    static const U8     kPromoShift = 5;
    CSqix               mFrom;
    CSqix               mTo;
};

//
//  A collection of moves
//
class CMoves 
{
public:
    static const std::uint8_t    kMaxMoves = 255;

    void addMove( CMove m ) { mMoves[mNumMoves++] = m; }
    std::uint8_t getNumMoves() const { return mNumMoves; }
    std::string asStr() const { return asAbbr(); }
    std::string asAbbr() const;

private:
    std::uint8_t            mNumMoves;
    CMove                   mMoves[kMaxMoves + 1];
};

///
///  the moves in a game or a line
///
class  CLine
{
public:
    CLine();
    static const std::uint16_t      kDeltaMaxMoves = 1024;

    void addMove( CMove m, YHashKey h )
    {
        if ( mNumMoves == mAllocatedMoves )
            reallocate();
        mpMoves[mNumMoves] = m;
        mpHashKeys[mNumMoves++] = h;
    }

    void deleteMove() 
    {
        assert( mNumMoves > 0 );
        mNumMoves--;
    }

    std::uint16_t getNumMoves() const { return mNumMoves; }
    
    CMove getMove( std::uint16_t moveIx ) const 
    {
        assert( moveIx < mNumMoves );
        return mpMoves[moveIx];
    }

    YHashKey getHashKey( std::uint16_t ) const; 
    std::uint16_t findHashKey() const;
    std::string asAbbr() const;
    std::string asStr() const;
    
private:
    std::uint16_t           mNumMoves;
    std::uint16_t           mAllocatedMoves;
    CMove*                  mpMoves;
    YHashKey*               mpHashKeys;

    void reallocate(); 
};

#endif          // move