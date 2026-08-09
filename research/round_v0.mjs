import { expandKey_v2 } from './expandKey_v2.mjs';
import { BLOCK_SIZE } from './utils.mjs';

const permutation = [
  0, 13, 26, 7, 20, 1, 14, 27,
  8, 21, 2, 15, 28, 9, 22, 3,
  16, 29, 10, 23, 4, 17, 30, 11,
  24, 5, 18, 31, 12, 19, 6, 25
];

const inversePermutation = new Uint8Array( BLOCK_SIZE );
for ( let i = 0; i < BLOCK_SIZE; i++ ) inversePermutation[ permutation[ i ] ] = i;

const rotl = ( value, bits ) => ( ( value << bits ) | ( value >>> ( 32 - bits ) ) ) >>> 0;
const rotr = ( value, bits ) => ( ( value >>> bits ) | ( value << ( 32 - bits ) ) ) >>> 0;

const substitute = ( value ) => ( value * 197 + 23 ) & 0xff;
const inverseSubstitute = ( value ) => ( ( value - 23 ) * 13 ) & 0xff;

const mix = ( words ) => {
  for ( let i = 0; i < 8; i += 2 ) {
    words[ i ] = ( words[ i ] + rotl( words[ i + 1 ], 5 ) ) >>> 0;
    words[ i + 1 ] = ( words[ i + 1 ] ^ rotl( words[ i ], 13 ) ) >>> 0;
  }

  for ( let i = 0; i < 8; i++ ) {
    const other = words[ ( i + 3 ) & 7 ];
    words[ i ] = ( words[ i ] ^ rotl( other, ( i * 7 + 3 ) & 31 ) ) >>> 0;
  }
};

const inverseMix = ( words ) => {
  for ( let i = 7; i >= 0; i-- ) {
    const other = words[ ( i + 3 ) & 7 ];
    words[ i ] = ( words[ i ] ^ rotl( other, ( i * 7 + 3 ) & 31 ) ) >>> 0;
  }

  for ( let i = 6; i >= 0; i -= 2 ) {
    words[ i + 1 ] = ( words[ i + 1 ] ^ rotl( words[ i ], 13 ) ) >>> 0;
    words[ i ] = ( words[ i ] - rotl( words[ i + 1 ], 5 ) ) >>> 0;
  }
};

const toWords = ( bytes ) => {
  const words = new Uint32Array( 8 );

  for ( let i = 0; i < 8; i++ ) {
    const offset = i * 4;

    words[ i ] = bytes[ offset ] |
      ( bytes[ offset + 1 ] << 8 ) |
      ( bytes[ offset + 2 ] << 16 ) |
      ( bytes[ offset + 3 ] << 24 );
  }

  return words;
};

const fromWords = ( words ) => {
  const bytes = new Uint8Array( BLOCK_SIZE );

  for ( let i = 0; i < 8; i++ ) {
    const offset = i * 4, word = words[ i ];

    bytes[ offset ] = word & 0xff;
    bytes[ offset + 1 ] = ( word >>> 8 ) & 0xff;
    bytes[ offset + 2 ] = ( word >>> 16 ) & 0xff;
    bytes[ offset + 3 ] = ( word >>> 24 ) & 0xff;
  }

  return bytes;
};

const transform = ( state, roundKey ) => {
  const bytes = new Uint8Array( BLOCK_SIZE );
  for ( let i = 0; i < BLOCK_SIZE; i++ ) bytes[ i ] = substitute( state[ i ] );

  const words = toWords( bytes );
  mix( words );

  const mixed = fromWords( words ), rotated = new Uint8Array( BLOCK_SIZE );
  for ( let i = 0; i < BLOCK_SIZE; i++ ) rotated[ i ] = mixed[ ( i + 7 ) & 31 ];

  const result = new Uint8Array( BLOCK_SIZE );
  for ( let i = 0; i < BLOCK_SIZE; i++ ) result[ i ] = rotated[ permutation[ i ] ] ^ roundKey[ i ];

  return result;
};

const inverseTransform = ( state, roundKey ) => {
  const unkeyed = new Uint8Array( BLOCK_SIZE );
  for ( let i = 0; i < BLOCK_SIZE; i++ ) unkeyed[ i ] = state[ i ] ^ roundKey[ i ];

  const unpermuted = new Uint8Array( BLOCK_SIZE );
  for ( let i = 0; i < BLOCK_SIZE; i++ ) unpermuted[ i ] = unkeyed[ inversePermutation[ i ] ];

  const unrotated = new Uint8Array( BLOCK_SIZE );
  for ( let i = 0; i < BLOCK_SIZE; i++ ) unrotated[ i ] = unpermuted[ ( i + 25 ) & 31 ];

  const words = toWords( unrotated );
  inverseMix( words );

  const bytes = fromWords( words ), result = new Uint8Array( BLOCK_SIZE );
  for ( let i = 0; i < BLOCK_SIZE; i++ ) result[ i ] = inverseSubstitute( bytes[ i ] );

  return result;
};

export const encryptBlock_v0 = ( block, key ) => {
  const keys = expandKey_v2( key );

  let state = new Uint8Array( block );
  for ( let round = 1; round < keys.length; round++ ) state = transform( state, keys[ round ] );

  return state;
};

export const decryptBlock_v0 = ( block, key ) => {
  const keys = expandKey_v2( key );

  let state = new Uint8Array( block );
  for ( let round = keys.length - 1; round >= 1; round-- ) state = inverseTransform( state, keys[ round ] );

  return state;
};
