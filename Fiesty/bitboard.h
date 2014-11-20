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
    void setIfValid( I8 rank, I8 file );
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

	static CBitBoard fileBits( CFile f ) { return CBitBoard( 0x8080808080808080ULL >> U8( f.get() 

    CBitBoard advanceRanks( int numRanks ) const { return ( mBitBoard << ( numRanks << 3 ) ); };
    CBitBoard retreatRanks( int numRanks ) const { return ( mBitBoard >> ( numRanks << 3 ) ); };
    CBitBoard leftFiles( int numFiles ) { ... }
    CBitBoard rightFiles( int numFiles ) { ... }
    CBitBoard onRank( CRank r ) { return ( mBitBoard & ( 0xFFULL << ( U8( r.get() ) << 3 ) ) ); }
    CBitBoard onFile( CFile r ) { return ( mBitBoard & ( 0xFFULL << ( U8( r.get() ) << 3 ) ) ); }

private:
    YBitBoard             mBitBoard;

};

#endif  // bitboard.h
