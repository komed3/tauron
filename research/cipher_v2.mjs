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
