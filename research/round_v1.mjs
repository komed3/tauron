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

const mixWords = ( words ) => {
  for ( let i = 0; i < 4; i++ ) {
    const a = i, b = i + 4;

    words[ a ] = ( words[ a ] + rotl( words[ b ], 7 ) ) >>> 0;
    words[ b ] = ( words[ b ] ^ rotl( words[ a ], 11 ) ) >>> 0;
  }

  for ( let i = 0; i < 4; i++ ) {
    const a = i, b = i + 4;

    words[ a ] = ( words[ a ] ^ rotl( words[ b ], 17 ) ) >>> 0;
    words[ b ] = ( words[ b ] + rotl( words[ a ], 3 ) ) >>> 0;
  }

  return words;
};

const unmixWords = ( words ) => {
  for ( let i = 3; i >= 0; i-- ) {
    const a = i, b = i + 4;

    words[ b ] = ( words[ b ] - rotl( words[ a ], 3 ) ) >>> 0;
    words[ a ] = ( words[ a ] ^ rotl( words[ b ], 17 ) ) >>> 0;
  }

  for ( let i = 3; i >= 0; i-- ) {
    const a = i, b = i + 4;

    words[ b ] = ( words[ b ] ^ rotl( words[ a ], 11 ) ) >>> 0;
    words[ a ] = ( words[ a ] - rotl( words[ b ], 7 ) ) >>> 0;
  }

  return words;
};

const xorKey = ( bytes, key ) => {
  for ( let i = 0; i < KEY_SIZE; i++ ) bytes[ i ] ^= key[ i ];
};

const transform = ( block, roundKey ) => {
  let state = new Uint8Array( block );

  // 1. Round key
  xorKey( state, roundKey );

  // 2. Nonlinear substitution
  for ( let i = 0; i < KEY_SIZE; i++ ) state[ i ] = substitute( state[ i ] );

  // 3. Byte rotation
  state = rotateBytes( state );

  // 4. Word-level ARX mixing
  state = fromWords( mixWords( toWords( state ) ) );

  return state;
};

const inverseTransform = ( block, roundKey ) => {
  let state = new Uint8Array( block );

  // 4. Inverse ARX mixing
  state = fromWords( unmixWords( toWords( state ) ) );

  // 3. Inverse byte rotation
  state = inverseRotateBytes( state );

  // 2. Inverse substitution
  for ( let i = 0; i < KEY_SIZE; i++ ) state[ i ] = inverseSubstitute( state[ i ] );

  // 1. Round key
  xorKey( state, roundKey );

  return state;
};
