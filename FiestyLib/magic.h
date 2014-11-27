/// file magic.h
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// headers having to do with magic move generation
///
#ifndef Fiesty_magic_h
#define Fiesty_magic_h
#include "fiesty.h"
#include "square.h"
#include "bitboard.h"

///
/// Contains the information needed to generate the keyes into each square's
/// magic hash table.
///
class CMagicKey
{
    const CBitBoard         mbbRayMask;
    std::uint64_t           mMultipier;
};

///
/// The generated magic data
///
class CMagic
{
    const U8        mkRookHashKeyBits       = 12;
    const U16       mkRookHashEntries       = 2 ^ mkRookHashKeyBits;
    const U8        mkBishopHashKeyBits     = 12;
    const U16       mkBishopHashEntries     = 2 ^ mkBishopHashKeyBits;

    const CMagicKey mRookMagicKeys[CSqix::kNumSquares];
    const CMagicKey mBishopMagicKeys[CSqix::kNumSquares];
    const CBitBoard mbbRookAttacks[CSqix::kNumSquares][mkRookHashEntries];
    const CBitBoard mbbBishopAttacks[CSqix::kNumSquares][mkBishopHashEntries];
};

#endif
