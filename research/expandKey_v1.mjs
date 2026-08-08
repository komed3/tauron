import { KEY_SIZE, schedule } from './utils.mjs';

const permutation = [
  0, 13, 26, 7, 20, 1, 14, 27,
  8, 21, 2, 15, 28, 9, 22, 3,
  16, 29, 10, 23, 4, 17, 30, 11,
  24, 5, 18, 31, 12, 19, 6, 25
];

const rotl = ( value, bits ) => ( value << bits ) | ( value >>> ( 32 - bits ) );
const substitute = value => ( value * 197 + 23 ) & 0xff;

export const expandKey_v1 = ( key ) => schedule( key, ( key, round ) => {
  const bytes = new Uint8Array( KEY_SIZE );

  // 1. Nonlinear byte substitution
  for ( let i = 0; i < KEY_SIZE; i++ ) bytes[ i ] = substitute( key[ i ] );

  // 2. Convert to 32-bit words
  const words = new Uint32Array( 8 );

  for ( let i = 0; i < 8; i++ ) {
    const offset = i * 4;
    words[ i ] = bytes[ offset ]
      | ( bytes[ offset + 1 ] << 8 )
      | ( bytes[ offset + 2 ] << 16 )
      | ( bytes[ offset + 3 ] << 24 );
  }

  // 3. Local ARX mixing
  for ( let i = 0; i < 8; i += 2 ) {
    words[ i ] = ( words[ i ] + rotl( words[ i + 1 ], 5 ) ) >>> 0;
    words[ i + 1 ] = ( words[ i + 1 ] ^ rotl( words[ i ], 13 ) ) >>> 0;
  }

  // 4. Cross-word mixing
  for ( let i = 0; i < 8; i++ ) {
    const other = words[ ( i + 3 ) & 7 ];
    words[ i ] = ( words[ i ] ^ rotl( other, ( i * 7 + 3 ) % 32 ) ) >>> 0;
  }

  // 5. Convert words back to bytes
  for ( let i = 0; i < 8; i++ ) {
    const offset = i * 4;
    const word = words[ i ];

    bytes[ offset ] = word & 0xff;
    bytes[ offset + 1 ] = ( word >>> 8 ) & 0xff;
    bytes[ offset + 2 ] = ( word >>> 16 ) & 0xff;
    bytes[ offset + 3 ] = ( word >>> 24 ) & 0xff;
  }

  // 6. Byte permutation
  const result = new Uint8Array( KEY_SIZE );
  for ( let i = 0; i < KEY_SIZE; i++ ) result[ i ] = bytes[ permutation[ i ] ];

  // 7. Round constant
  for ( let i = 0; i < KEY_SIZE; i++ ) result[ i ] ^= ( round * 29 + i * 17 ) & 0xff;

  return result;
} );
