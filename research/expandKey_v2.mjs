import { KEY_SIZE, schedule } from './utils.mjs';

const WARMUP_ROUNDS = 3;

const permutation = [
  0, 13, 26, 7, 20, 1, 14, 27,
  8, 21, 2, 15, 28, 9, 22, 3,
  16, 29, 10, 23, 4, 17, 30, 11,
  24, 5, 18, 31, 12, 19, 6, 25
];

const rotl = ( value, bits ) => ( value << bits ) | ( value >>> ( 32 - bits ) );
const substitute = value => ( value * 197 + 23 ) & 0xff;

const deriveConstant = ( bytes, round ) => {
  let value = ( 0x9e3779b9 ^ round ) >>> 0;

  for ( let i = 0; i < KEY_SIZE; i++ ) {
    value ^= bytes[ i ];
    value = Math.imul( value, 0x85ebca6b ) >>> 0;
    value = rotl( value, 13 );
  }

  const result = new Uint8Array( KEY_SIZE );

  for ( let i = 0; i < KEY_SIZE; i++ ) {
    value ^= i + round;
    value = Math.imul( value, 0xc2b2ae35 ) >>> 0;
    value = rotl( value, 7 );

    result[ i ] = ( value ^ ( value >>> 8 ) ^ ( value >>> 16 ) ^ ( value >>> 24 ) ) & 0xff;
  }

  return result;
};
