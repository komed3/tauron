import { expandKey_v2 } from './expandKey_v2.mjs';
import { BLOCK_SIZE } from './utils.mjs';

const rotl = ( value, bits ) => ( value << bits ) | ( value >>> ( 32 - bits ) ) >>> 0;
const rotr = ( value, bits ) => ( value >>> bits ) | ( value << ( 32 - bits ) ) >>> 0;

const substitute = value => ( Math.imul( value, 251 ) + 17 ) & 0xff;
const inverseSubstitute = value => ( Math.imul( value - 17, 179 ) ) & 0xff;
