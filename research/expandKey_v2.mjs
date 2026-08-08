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
