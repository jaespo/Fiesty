///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// code having to do with bitmaps
///
///
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "square.h"
#include "bitmap.h"

///
/// returns a bitmap as a fenlike string
///
std::string CBitmap::asStr() const
{
    std::string s;
    for ( std::int8_t r = U8( ERank::kRank8 ); r >= 0; r-- )
    {
        for ( U8 f = U8( EFile::kFileA ); f <= U8( EFile::kFileH ); f++ )
        {
            CSqix sqix = CSqix( ERank( r ), EFile( f ) );
            s.append( ( sqix.asBitmap() & mBitmap ) ? "1" : "0" );
        }
        if ( r ) 
            s.append( "/" );
    }
    return s;
}

///
/// returns a bitmap as a hex string
///
std::string CBitmap::asAbbr() const
{
    std::stringstream stream;
    stream << "0x" << std::hex << mBitmap;
    std::string result( stream.str() );
    return result;
}

