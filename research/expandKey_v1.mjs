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

  return result;
} );
