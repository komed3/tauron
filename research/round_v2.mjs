import { expandKey_v2 } from './expandKey_v2.mjs';
import { BLOCK_SIZE } from './utils.mjs';


const WORDS = 8;
const NONLINEAR_BASE = 0x9e3779b1;

const rotl = ( value, bits ) => { bits &= 31; return ( value << bits ) | ( value >>> ( 32 - bits ) ); };
const add = ( a, b ) => ( a + b ) >>> 0;
const sub = ( a, b ) => ( a - b ) >>> 0;
const mul = ( a, b ) => Math.imul( a, b ) >>> 0;

const inverse32 = value => {
  let result = value;

  for ( let i = 0; i < 5; i++ ) result = mul( result, 2 - mul( value, result ) );
  return result >>> 0;
};

const toWords = ( bytes ) => {
  const words = new Uint32Array( WORDS );

  for ( let i = 0; i < WORDS; i++ ) {
    const offset = i * 4;

    words[ i ] = bytes[ offset ] | ( bytes[ offset + 1 ] << 8 ) |
      ( bytes[ offset + 2 ] << 16 ) | ( bytes[ offset + 3 ] << 24 );
  }

  return words;
};

const fromWords = ( words ) => {
  const bytes = new Uint8Array( BLOCK_SIZE );

  for ( let i = 0; i < WORDS; i++ ) {
    const offset = i * 4, word = words[ i ];

    bytes[ offset ] = word & 0xff;
    bytes[ offset + 1 ] = ( word >>> 8 ) & 0xff;
    bytes[ offset + 2 ] = ( word >>> 16 ) & 0xff;
    bytes[ offset + 3 ] = ( word >>> 24 ) & 0xff;
  }

  return bytes;
};

const injectionRotation = ( round, index ) => ( round * 7 + index * 3 ) & 31;
const diffusionRotationA = ( round, index ) => ( round * 5 + index * 7 + 3 ) & 31;
const diffusionRotationB = ( round, index ) => ( round * 3 + index * 11 + 5 ) & 31;
const butterflyRotationA = ( round, index ) => ( round * 3 + index * 5 + 7 ) & 31;
const butterflyRotationB = ( round, index ) => ( round * 7 + index * 3 + 11 ) & 31;
const crossRotationA = ( round, index ) => ( round * 11 + index * 3 + 5 ) & 31;
const crossRotationB = ( round, index ) => ( round * 13 + index * 7 + 9 ) & 31;
const multiplier = ( round, index ) => ( NONLINEAR_BASE + round * 2 + index * 2 ) >>> 0;

const inject = ( words, keys, round ) => {
  for ( let i = 0; i < WORDS; i++ ) {
    words[ i ] = add( words[ i ], keys[ i ] );
    words[ i ] ^= rotl( keys[ i ], injectionRotation( round, i ) );
    words[ i ] >>>= 0;
  }
};

const inverseInject = ( words, keys, round ) => {
  for ( let i = 0; i < WORDS; i++ ) {
    words[ i ] ^= rotl( keys[ i ], injectionRotation( round, i ) );
    words[ i ] = sub( words[ i ], keys[ i ] );
  }
};

const nonlinear = ( words, round ) => {
  for ( let i = 0; i < WORDS; i++ ) words[ i ] = mul( words[ i ], multiplier( round, i ) );
};

const inverseNonlinear = ( words, round ) => {
  for ( let i = 0; i < WORDS; i++ ) words[ i ] = mul( words[ i ], inverse32( multiplier( round, i ) ) );
};

const mixPair = ( words, a, b, r1, r2 ) => {
  words[ a ] ^= rotl( words[ b ], r1 );
  words[ a ] >>>= 0;
  words[ b ] = add( words[ b ], rotl( words[ a ], r2 ) );
};

const inverseMixPair = ( words, a, b, r1, r2 ) => {
  words[ b ] = sub( words[ b ], rotl( words[ a ], r2 ) );
  words[ a ] ^= rotl( words[ b ], r1 );
  words[ a ] >>>= 0;
};

const diffuse = ( words, round ) => {
  for ( let i = 0; i < WORDS; i++ ) {
    const next = ( i + 1 ) & 7;

    words[ i ] ^= rotl( words[ next ], diffusionRotationA( round, i ) );
    words[ i ] >>>= 0;
    words[ next ] = add( words[ next ], rotl( words[ i ], diffusionRotationB( round, i ) ) );
  }
};

const inverseDiffuse = ( words, round ) => {
  for ( let i = WORDS - 1; i >= 0; i-- ) {
    const next = ( i + 1 ) & 7;

    words[ next ] = sub( words[ next ], rotl( words[ i ], diffusionRotationB( round, i ) ) );
    words[ i ] ^= rotl( words[ next ], diffusionRotationA( round, i ) );
    words[ i ] >>>= 0;
  }
};

const butterfly = ( words, round ) => {
  const pairs = [ [ 0, 1 ], [ 2, 3 ], [ 4, 5 ], [ 6, 7 ] ];
  for ( const [ a, b ] of pairs ) mixPair( words, a, b, butterflyRotationA( round, a ), butterflyRotationB( round, b ) );

  const cross = [ [ 0, 2 ], [ 1, 3 ], [ 4, 6 ], [ 5, 7 ] ];
  for ( const [ a, b ] of cross ) mixPair( words, a, b, crossRotationA( round, a ), crossRotationB( round, b ) );
};

const inverseButterfly = ( words, round ) => {
  const cross = [ [ 0, 2 ], [ 1, 3 ], [ 4, 6 ], [ 5, 7 ] ];

  for ( let i = cross.length - 1; i >= 0; i-- ) {
    const [ a, b ] = cross[ i ];
    inverseMixPair( words, a, b, crossRotationA( round, a ), crossRotationB( round, b ) );
  }

  const pairs = [ [ 0, 1 ], [ 2, 3 ], [ 4, 5 ], [ 6, 7 ] ];

  for ( let i = pairs.length - 1; i >= 0; i-- ) {
    const [ a, b ] = pairs[ i ];
    inverseMixPair( words, a, b, butterflyRotationA( round, a ), butterflyRotationB( round, b ) );
  }
};

export const transform = ( block, key, round ) => {
  const words = toWords( block );
  const keys = toWords( key );

  inject( words, keys, round );
  diffuse( words, round );
  nonlinear( words, round );
  butterfly( words, round );
  nonlinear( words, round );

  return fromWords( words );
};

export const inverseTransform = ( block, key, round ) => {
  const words = toWords( block );
  const keys = toWords( key );

  inverseNonlinear( words, round );
  inverseButterfly( words, round );
  inverseNonlinear( words, round );
  inverseDiffuse( words, round );
  inverseInject( words, keys, round );

  return fromWords( words );
};

export const encryptBlock_v2 = ( block, key ) => {
  const keys = expandKey_v2( key );

  let state = new Uint8Array( block );
  for ( let round = 1; round < keys.length; round++ ) state = transform( state, keys[ round ], round );

  return state;
};

export const decryptBlock_v2 = ( block, key ) => {
  const keys = expandKey_v2( key );

  let state = new Uint8Array( block );
  for ( let round = keys.length - 1; round >= 1; round-- ) state = inverseTransform( state, keys[ round ], round );

  return state;
};

export const encrypt_v2 = ( block, keys ) => {
  let state = new Uint8Array( block );
  for ( let round = 1; round < keys.length; round++ ) state = transform( state, keys[ round ], round );

  return state;
};

export const decrypt_v2 = ( block, keys ) => {
  let state = new Uint8Array( block );
  for ( let round = keys.length - 1; round >= 1; round-- ) state = inverseTransform( state, keys[ round ], round );

  return state;
};
