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
#include "bitboard.h"
#include "piece.h"
#include "move.h"

///
/// Class that represents the casting and en passant rights, an the number of
/// duplicates in a position.
///
class CPosRights
{
public:
    CPosRights() { mRights = kAllCastle; }

    //
    //  Getters
    //
    CFile getEnPassantFile() const { return EFile( mRights & kEnPassantFileMask ); }
    U8 isEnPassantLegal() const 
    { 
        return mRights & kEnPassantLegalMask; 
    }
    U8 canWhiteOO() const       { return mRights & kWhiteOOMask; }
    U8 canWhiteOOO() const      { return mRights & kWhiteOOOMask; }
    U8 canBlackOO() const       { return mRights & kBlackOOMask; }
    U8 canBlackOOO() const      { return mRights & kBlackOOOMask; }

    //
    //  Setters
    //
    void init() { mRights = kAllCastle; }
    void setEnPassantFile( CFile f ) 
    { 
        mRights |= U8( f.get() ) | kEnPassantLegalMask; 
    }
    void setWhiteOO()    { mRights |= kWhiteOOMask; }
    void clearWhiteOO()  { mRights &= ~kWhiteOOMask; }
    void setWhiteOOO()   { mRights |= kWhiteOOOMask; }
    void clearWhiteOOO() { mRights &= ~kWhiteOOOMask; }

    void setBlackOO()    { mRights |= kBlackOOMask; }
    void clearBlackOO()  { mRights &= ~kBlackOOMask; }
    void setBlackOOO()   { mRights |= kBlackOOOMask; }
    void clearBlackOOO() { mRights &= ~kBlackOOOMask; }

    std::string asStr() const;
    std::string asAbbr() const { return asStr(); }
    std::string castlingAsStr() const;

private:
    friend class CTester;

    static const U8 kEnPassantFileMask  = 0x07;
    static const U8 kEnPassantLegalMask = 0x08;
    static const U8 kWhiteOOMask        = 0x10;
    static const U8 kWhiteOOOMask       = 0x20;
    static const U8 kBlackOOMask        = 0x40;
    static const U8 kBlackOOOMask       = 0x80;
    static const U8 kAllCastle          = 0xF0;

    U8          mRights;

    //TODO: code me:  test this object ...    
    void onWqrMove() { clearWhiteOOO(); }
    void onWkrMove() { clearWhiteOO(); }
    void onBqrMove() { clearBlackOOO(); }
    void onBkrMove() { clearBlackOO(); }
    void onKMove( CColor c ) 
    { 
        if ( c.isWhite() )
            mRights &= ~( kWhiteOOMask | kWhiteOOOMask );
        else
            mRights &= ~( kBlackOOMask | kBlackOOOMask );
    };

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
    void parseFen( std::string sFen );              //TODO: code me
    CColor getWhoseMove() { return mWhoseMove; }
    CPiece getPiece( YSqix sqix );                  //TODO: code me

    void getPawnNonCaptures( CMoves& rMoves );      //TODO: code me
    void getPawnCaptures( CMoves& rMoves );         //TODO: code me
    void getKnightNonCaptures( CMoves& rMoves );    //TODO: code me
    void getKnightCaptures( CMoves& rMoves );       //TODO: code me
    void getBishopNonCaptures( CMoves& rMoves );    //TODO: code me
    void getBishopCaptures( CMoves& rMoves );       //TODO: code me
    void getRookNonCaptures( CMoves& rMoves );      //TODO: code me
    void getRookCaptures( CMoves& rMoves );         //TODO: code me
    void getQueenNonCaptures( CMoves& rMoves );     //TODO: code me
    void getQueenCaptures( CMoves& rMoves );        //TODO: code me
    void getKingNonCaptures( CMoves& rMoves );      //TODO: code me    
    void getKingCaptures( CMoves& rMoves );         //TODO: code me

    void getMoves( CMoves& rMoves );                //TODO: code me

    void makeMove( CMove m );                       //TODO: code me
    void unmakeMove( CMove m );                     //TODO: code me

    std::string asStr() const;                     //TODO: code me
    std::string asAbbr() const;                    //TODO: code me
    std::string asFen() const;                      //TODO: code me
    std::string asDiagram();                        //TODO: code me

private:
    CColor          mWhoseMove;
    U8              mHalfMoveClock;                 // for 50 move rule
    U8              mDups;                          // for 3 time repetions
    CPosRights      mPositionRights;
    CPiece          mBoard[U8( ERank::kNum ) * U8( EFile::kNum )];
    CBitBoard       mbbPieceType[EPieceType::kNum];
    CBitBoard       mbbColor[EColor::kNum];
    U8              mMoveNum;
};

#endif      // position.h