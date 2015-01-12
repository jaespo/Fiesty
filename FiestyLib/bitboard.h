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
    void operator &=( CBitBoard bb ) { mBitBoard &= bb.get(); }
    void operator ^=( CBitBoard bb ) { mBitBoard ^= bb.get(); }
    CBitBoard operator ~() const { return ~mBitBoard; } 

    void setSquare( YSqix sqix ) { mBitBoard |= 1ULL << sqix; }
    void resetSquare( YSqix sqix ) { mBitBoard ^= 1ULL << sqix; }
    CBitBoard getSquareBits( YSqix sqix ) const { return mBitBoard & ( 1ULL << sqix ); }
    void setIfValid( S8 rank, S8 file );
    
    std::string asStr() const;
    std::string asAbbr() const;
    std::string asDiagram() const;
    std::string asStrSquares() const;

    ///
    /// @return the bb for a file
    ///
	static CBitBoard fileBits( CFile f ) 
    { 
        return CBitBoard( 0x0101010101010101ULL << U8( f.get() ) );
    }

    ///
    /// @return the bb for a rank
    ///
	static CBitBoard rankBits( CRank r ) 
    { 
        return CBitBoard( 0xFFULL << ( 8 * U8( r.get() ) ) );
    }

    ///
    /// @returns 
    ///     the square index of the least significan bit, and clears that bit
    /// 
    CSqix popLsb()
    {
        unsigned long index;
        assert( mBitBoard != 0 );
        _BitScanForward64( &index, mBitBoard );
        mBitBoard ^= 1ULL << index;
        return YSqix( index );
    }
    
    ///
    /// @returns 
    ///     the square index of the most significan bit, and clears that bit
    /// 
    CSqix popMsb()
    {
        unsigned long index;
        assert( mBitBoard != 0 );
        _BitScanReverse64( &index, mBitBoard );
        mBitBoard ^= 1ULL << index;
        return YSqix( index );
    }

    ///
    /// @returns the number of bits set in the bitboard
    ///
    U64 popcnt() { return __popcnt64( mBitBoard ); }

    ///
    /// @returns the square index of the least significan bit
    /// 
    CSqix lsb() const
    {
        unsigned long index;
        assert( mBitBoard != 0 );
        _BitScanForward64( &index, mBitBoard );
        return YSqix( index );
    }
    
    ///
    /// @returns the square index of the most significan bit
    /// 
    CSqix msb() const
    {
        unsigned long index;
        assert( mBitBoard != 0 );
        _BitScanReverse64( &index, mBitBoard );
        return YSqix( index );
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
        //
        //  Shifting the position to the left is the same as decreasing the 
        //  arithmetic value of the bitboard  -- so that means use a 
        //  right shift!
        //
        return ( mBitBoard >> numFiles ) & ~fileBits( EFile::kFileH ).get();
    }

    ///
    /// @return the bb with the pieces shifted right the number of files
    ///
    CBitBoard rightFiles( U8 numFiles ) 
    {
        //
        //  Shifting the position to the right is the same as decreasing the 
        //  arithmetic value of the bitboard  -- so that means use a 
        //  left shift!
        //
        return ( mBitBoard << numFiles ) & ~fileBits( EFile::kFileA ).get();
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
