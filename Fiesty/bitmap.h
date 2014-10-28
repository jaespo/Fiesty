/// file bitmap.h
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// headers having to do with bitmaps
///
///
#ifndef Fiesty_bitmap_h
#define Fiesty_bitmap_h
#include "fiesty.h"
#include "square.h"

//
//  A bitmap.  Bit 0 (LSB) is a1, Bit 2 is a2, etc.
//
class CBitmap
{
public:
    CBitmap() {}
    CBitmap( YBitmap bm ) { mBitmap = bm; }
    YBitmap get() const { return mBitmap; }

    void setSquare( YSqix sqix ) { 
        mBitmap |= 1ULL << sqix; }
    CSqix iterateLsb()
    {
        unsigned long index;
        assert( mBitmap != 0 );
        _BitScanForward64( &index, mBitmap );
        mBitmap &= mBitmap - 1;
    }
    
    std::string asStr() const;
    std::string asAbbr() const;    // ...

private:
    YBitmap             mBitmap;

// ... test this
};

#endif  // bitmap.h
