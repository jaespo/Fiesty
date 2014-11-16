/// file bitboard.h
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// headers having to do with BitBoards
///
///
#ifndef Fiesty_bitboard_h
#define Fiesty_bitboard_h
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

    void operator |=( CBitBoard bb ) { mBitBoard |= bb.get(); }
    void setSquare( YSqix sqix ) { mBitBoard |= 1ULL << sqix; }
    CSqix popLsb()
    {
        unsigned long index;
        assert( mBitBoard != 0 );
        _BitScanForward64( &index, mBitBoard );
        mBitBoard &= mBitBoard - 1;
        return CSqix( YSqix( index ) );
    }
    
    std::string asStr() const;
    std::string asAbbr() const;

    CBitBoard advanceFiles( int numFiles ) { return ( mBitBoard << ( numFiles << 3 ) ); };
    CBitBoard retreatFiles( int numFiles ) { return ( mBitBoard >> ( numFiles << 3 ) ); };
    CBitBoard onRank( CRank r ) { return ( mBitBoard & ( 0xFFULL << ( U8( r.get() ) << 3 ) ) ); }

private:
    YBitBoard             mBitBoard;
};

#endif  // bitboard.h
