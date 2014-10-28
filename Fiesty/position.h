/// file position.h
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// headers having to do with positions
///
///
#ifndef Fiesty_position_h
#define Fiesty_position_h

#include "fiesty.h"
#include "bitmap.h"
#include "piece.h"
#include "move.h"

///
/// Class that represents the casting and en passant rights, an the number of
/// duplicates in a position.
///
class CPosRights
{
public:
    CPosRights() { mRights = kAll; }

    //
    //  Getters
    //
    EFile getEnPassantFile() const { return EFile( mRights & kEnPassantMask ); }
    U8 getDupCount() const      { return mRights >> 6; }
    U8 canWhiteOO() const       { return mRights & kWhiteOOMask; }
    U8 canWhiteOOO() const      { return mRights & kWhiteOOOMask; }
    U8 canBlackOO() const       { return mRights & kBlackOOMask; }
    U8 canBlackOOO() const      { return mRights & kBlackOOOMask; }

    //
    //  Setters
    //
    void init() { mRights = kAll; }
    void setEnPassantFile( EColor c, EFile f ) 
    { 
        mRights = ( mRights & ~kEnPassantMask ) 
            | ( c == EColor::kWhite ) ? 4 : 5; 
    }
    void incrDupCount()  { if ( mRights < kDupPosMask ) mRights += 0x040; }
    void clearDupCount() { mRights &= ~kDupPosMask; }

    void setWhiteOO()    { mRights |= kWhiteOOMask; }
    void clearWhiteOO()  { mRights &= ~kWhiteOOMask; }
    void setWhiteOOO()   { mRights |= kWhiteOOOMask; }
    void clearWhiteOOO() { mRights &= ~kWhiteOOOMask; }

    void setBlackOO()    { mRights |= kBlackOOMask; }
    void clearBlackOO()  { mRights &= ~kBlackOOMask; }
    void setBlackOOO()   { mRights |= kBlackOOOMask; }
    void clearBlackOOO() { mRights &= ~kBlackOOOMask; }

    std::string asStr() const;                  // ...
    std::string asAbbr() const { return ""; }   // ...
    std::string castlingAsStr() const;  // ...

private:
    static const std::uint8_t   kEnPassantMask   = 0x03;
    static const std::uint8_t   kWhiteOOMask     = 0x04;
    static const std::uint8_t   kWhiteOOOMask    = 0x08;
    static const std::uint8_t   kBlackOOMask     = 0x10;
    static const std::uint8_t   kBlackOOOMask    = 0x20;
    static const std::uint8_t   kDupPosMask      = 0xC0;
    static const std::uint8_t   kAll             = 0x3F;
    static const std::uint8_t   kDupPosShift     = 6;

    //  test this object ...    
    void onWqrMove() { clearWhiteOOO(); }
    void onWkrMove() { clearWhiteOO(); }
    void onBqrMove() { clearBlackOOO(); }
    void onBkrMove() { clearBlackOOO(); }
    void onKMove( CColor c ) 
    { 
        if ( c.isWhite() )
            mRights &= ~( kWhiteOOMask | kWhiteOOOMask );
        else
            mRights &= ~( kBlackOOMask | kBlackOOOMask );
    };

private:
    U8                          mRights;
};

///
/// Class that represents the information needed to undo a chess after it's 
/// been made
///
class CUndoContext
{
public:
    CPiece getPieceMoved() const { return mPieceMoved; }
    CPiece getPieceCaptured() const { return mPieceCaptured; }
    CPosRights getPosRights() const { return mPosRights; }

    void setPieceMoved( CPiece p ) { mPieceMoved = p; }
    void setPieceCaptured( CPiece p ) { mPieceCaptured = p; }
    void setPosRights( CPosRights pr ) { mPosRights = pr; }
    
    std::string asStr() const { return asAbbr(); };
    std::string asAbbr() const;

private:
    CPiece      mPieceMoved;
    CPiece      mPieceCaptured;
    CPosRights  mPosRights;
};

//
//  Class the represents a chess position
//
class CPos
{
public:
    void parseFen( std::string sFen );              // ...
    EColor getWhoseMove() { return mWhoseMove; }
    EPiece getPiece( YSqix sqix );                  // ...

    void getPawnNonCaptures( CMoves& rMoves );      // ...
    void getPawnCaptures( CMoves& rMoves );         // ...
    void getKnightNonCaptures( CMoves& rMoves );    // ...
    void getKnightCaptures( CMoves& rMoves );       // ...
    void getBishopNonCaptures( CMoves& rMoves );    // ...
    void getBishopCaptures( CMoves& rMoves );       // ...
    void getRookNonCaptures( CMoves& rMoves );      // ...
    void getRookCaptures( CMoves& rMoves );         // ...
    void getQueenNonCaptures( CMoves& rMoves );     // ...
    void getQueenCaptures( CMoves& rMoves );        // ...
    void getKingNonCaptures( CMoves& rMoves );      // ...    
    void getKingCaptures( CMoves& rMoves );         // ...

    void getMoves( CMoves& rMoves );                // ...

    void makeMove( CMove m );                       // ...
    void unmakeMove( CMove m );                     // ...

    static std::string asStr();                     // ...
    static std::string asAbbr();                    // ...
    static std::string asFen( EFile f );            // ...

private:
    EColor          mWhoseMove;
    CPosRights      mPositionRights;
    CPiece          mBoard[U8( ERank::kNum ) * U8( EFile::kNum )];
    CBitmap         mBitmapPieces[EPiece::kNum];
};

#endif      // position.h