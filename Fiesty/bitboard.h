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

    ///
    /// @return the bb for a file
    ///
	static CBitBoard fileBits( CFile f ) 
    { 
        return CBitBoard( 0x8080808080808080ULL >> ( 8 * U8( f.get() ) ) );
    }

    ///
    /// @return the bb for a rank
    ///
	static CBitBoard rankBits( CRank r ) 
    { 
        return CBitBoard( 0xFFULL >> ( 8 * U8( r.get() ) ) );
    }

    ///
    /// @return the bb with the pieces advanced a number of ranks
    ///
    CBitBoard advanceRanks( U8 numRanks ) const 
    { 
        return ( mBitBoard << ( 8 * numRanks ) ); 
    };

    ///
    /// @return the bb with the pieces retreated a number of ranks
    ///
    CBitBoard retreatRanks( U8 numRanks ) const 
    { 
        return ( mBitBoard >> ( 8 * numRanks ) ); 
    };

    ///
    /// @return the bb with the pieces shifted left the number of files
    ///
    CBitBoard leftFiles( U8 numFiles ) const
    {
        return ( mBitBoard << numFiles ) & fileBits( EFile::kFileH ).get();
    }

    ///
    /// @return the bb with the pieces shifted right the number of files
    ///
    CBitBoard rightFiles( U8 numFiles ) 
    {
        return ( mBitBoard >> numFiles ) & fileBits( EFile::kFileA ).get();
    }
    
    ///
    /// @return the bb with only the pieces on a specified rank
    ///
    CBitBoard onRank( CRank r ) const 
    { 
        return ( mBitBoard & ( rankBits( r ) ).get() );
    }

    ///
    /// @return the bb with only the pieces on a specified file
    ///
    CBitBoard onFile( CFile f ) const
    { 
        return ( mBitBoard & ( fileBits( f ) ).get() );
    }

    ///
    /// @return the bb with everything but the specified square masked off
    ///
    CBitBoard atSquare( CSqix sqix ) const
    {
        return mBitBoard & sqix.asBitBoard();
    }

private:
    YBitBoard             mBitBoard;

};

#endif  // bitboard.h
