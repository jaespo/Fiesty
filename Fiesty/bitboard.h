/// file bitboard.h
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// headers having to do with BitBoards
///
///
#ifndef Fiesty_BitBoard_h
#define Fiesty_BitBoard_h
#include "fiesty.h"
#include "square.h"

//
//  A bitboard.  Bit 0 (LSB) is a1, Bit 2 is a2, etc.
//
class CBitBoard
{
public:
    CBitBoard() {}
    CBitBoard( YBitBoard bm ) { mBitBoard = bm; }
    YBitBoard get() const { return mBitBoard; }

    void setSquare( YSqix sqix ) { mBitBoard |= 1ULL << sqix; }
    CSqix popLsb()
    {
        unsigned long index;
        assert( mBitBoard != 0 );
        _BitScanForward64( &index, mBitBoard );
        mBitBoard &= mBitBoard - 1;
    }
    
    std::string asStr() const;
    std::string asAbbr() const;

private:
    YBitBoard             mBitBoard;

// ... test this
};

#endif  // bitboard.h
