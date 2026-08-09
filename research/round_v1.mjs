import { expandKey_v2 } from './expandKey_v2.mjs';
import { BLOCK_SIZE } from './utils.mjs';

const rotl = ( value, bits ) => ( value << bits ) | ( value >>> ( 32 - bits ) ) >>> 0;
const rotr = ( value, bits ) => ( value >>> bits ) | ( value << ( 32 - bits ) ) >>> 0;

const substitute = ( value ) => ( Math.imul( value, 251 ) + 17 ) & 0xff;
const inverseSubstitute = ( value ) => ( Math.imul( value - 17, 179 ) ) & 0xff;

const rotateBytes = ( bytes ) => {
  const result = new Uint8Array( KEY_SIZE );

  for ( let i = 0; i < KEY_SIZE; i++ ) result[ ( i * 7 ) % KEY_SIZE ] = bytes[ i ];
  return result;
};

const inverseRotateBytes = ( bytes ) => {
  const result = new Uint8Array( KEY_SIZE );

  for ( let i = 0; i < KEY_SIZE; i++ ) result[ i ] = bytes[ ( i * 7 ) % KEY_SIZE ];
  return result;
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
  const bytes = new Uint8Array( KEY_SIZE );

  for ( let i = 0; i < 8; i++ ) {
    const offset = i * 4, word = words[ i ];

    bytes[ offset ] = word & 0xff;
    bytes[ offset + 1 ] = ( word >>> 8 ) & 0xff;
    bytes[ offset + 2 ] = ( word >>> 16 ) & 0xff;
    bytes[ offset + 3 ] = ( word >>> 24 ) & 0xff;
  }

  return bytes;
};
