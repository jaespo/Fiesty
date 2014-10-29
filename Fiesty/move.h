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
//
/// mFrom and mTo each take 6 bits. Promo takes 3 bits.
///
/// The promotion piece type is stored in the 3 most significan bits
/// of mTo.  A non-promotion is stored as a piece-type of kPawn
///
class CMove
{
public:
    CMove() {}

    CMove( CSqix f, CSqix t, CPieceType p ) 
    {
        mFrom = f.get(); 
        mTo = t.get();
        mPromo = U8( p.get() );
    }

    CMove( CSqix f, CSqix t ) 
    {
        mFrom = f.get(); 
        mTo = t.get(); 
        mPromo = U8( EPieceType::kPawn );
    }

    CSqix getFrom() const { return mFrom; }
    CSqix getTo() const { return mTo; }
    CPieceType getPromo() const { return EPieceType( mPromo ); }
    void setFrom( CSqix s ) { mFrom = s.get(); }
    void setTo( CSqix s ) { mTo = s.get(); }
    void setPromo( CPieceType pt ) { mPromo = U8( pt.get() ); }

    std::string asAbbr() const;
    std::string asStr() const { return asAbbr(); }

private:
    std::uint16_t       mFrom   : 6;
    std::uint16_t       mTo     : 6;
    std::uint16_t       mPromo  : 3;
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