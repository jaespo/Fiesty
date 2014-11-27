/// file fiesty.h
///
///  Fiesty (C) 2014 by Jeffery A Esposito
///
///  Commonly used declarations used by the fiesty chess program
///
///  Naming conventions prefixes
///      Y       a data type
///      C       a class
///      U       a class that is a collection of utilities but has no data
///
#ifndef Fiesty_fiesty_h
#define Fiesty_fiesty_h

#include <assert.h>
#include <intrin.h>
#include <cstdint>
#include <string>
#include <iostream>

typedef std::uint8_t    U8;
typedef std::int8_t     S8;
typedef std::uint16_t   U16;
typedef std::int16_t    S16;
typedef std::uint64_t   U64;
typedef std::int64_t    S64;

//
//  Simple acility for debugging, can use insertion operators with x
//
#define TR( x ) std::cout << x << std::endl << std::flush;

///
/// Enum that represents a side's color, white or black
///
enum class EColor : std::uint8_t { kWhite, kBlack, kNone, kNum = kNone };

///
///  Enum that defines the types of pieces
///
enum class EPieceType : std::uint8_t 
    { kPawn, kKnight, kBishop, kRook, kQueen, kKing, kNone, kNum = kNone };

///
/// Enum that defines the type of piece, taking into account color
///
typedef enum class EPiece : std::uint8_t
{ 
    kWhitePawn,
    kWhiteKnight,
    kWhiteBishop,
    kWhiteRook,
    kWhiteQueen,
    kWhiteKing,
    kBlackPawn,
    kBlackKnight,
    kBlackBishop,
    kBlackRook,
    kBlackQueen,
    kBlackKing,
    kNone,
    kNum = kNone
};

///
/// Enum for a rank on the chess board
///
enum class ERank : std::uint8_t 
{
    kRank1,
    kRank2,
    kRank3,
    kRank4,
    kRank5,
    kRank6,
    kRank7,
    kRank8,
    kNone,
    kNum = kNone
};

///
/// Enum for a rank on the chess board
///
enum class EFile : std::uint8_t 
{
    kFileA,
    kFileB,
    kFileC,
    kFileD,
    kFileE,
    kFileF,
    kFileG,
    kFileH,
    kNone,
    kNum = kNone
};

///
/// A square index, stored as 3 bits of rank and 3 bits of file
///
/// file is the three least significant bits
/// rank is the three most significant bits
///
typedef std::uint8_t    YSqix;

///
/// Flags that indictes the rights to castle and en-passant, and the repeated
/// position count.
///
typedef std::uint8_t    YPositionRights;

///
/// 64 bit hash key
///
typedef std::uint64_t    YHashKey;

///
/// 64 bit board
///
typedef std::uint64_t    YBitBoard;

#endif  // fiesty.h