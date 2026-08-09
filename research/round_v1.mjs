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
