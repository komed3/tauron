import { expandKey_v2 as algo } from './expandKey_v2.mjs';
import { KEY_SIZE, ROUNDS, hammingDistance, hex } from './utils.mjs';

const TEST_KEYS = 1000;

const randomKey = () => {
  const key = new Uint8Array( KEY_SIZE );
  crypto.getRandomValues( key );
  return key;
};

const incrementKey = ( key ) => {
  const result = new Uint8Array( key );

  for ( let i = result.length - 1; i >= 0; i-- ) {
    result[ i ]++;
    if ( result[ i ] !== 0 ) break;
  }

  return result;
};

const countBits = ( byte ) => {
  let bits = 0;

  while ( byte ) {
    bits += byte & 1;
    byte >>>= 1;
  }

  return bits;
};

const average = ( values ) => values.reduce( ( sum, value ) => sum + value, 0 ) / values.length;

const deviation = ( values ) => {
  const avg = average( values );
  return Math.sqrt( average( values.map( value => ( value - avg ) ** 2 ) ) );
};

const equalBytes = ( a, b ) => a.length === b.length && a.every( ( byte, index ) => byte === b[ index ] );

const testDeterminism = expandKey => {
  const key = randomKey();

  const a = expandKey( key );
  const b = expandKey( key );

  const identical = a.every( ( round, index ) => equalBytes( round, b[ index ] ) );
  console.log( `Determinism        ${ identical ? 'PASS' : 'FAIL' }` );
};

const testRandomKeys = ( expandKey ) => {
  const distances = [];
  let prev = expandKey( randomKey() );

  for ( let i = 1; i < TEST_KEYS; i++ ) {
    const current = expandKey( randomKey() );

    distances.push( hammingDistance( prev[ ROUNDS ], current[ ROUNDS ] ) );
    prev = current;
  }

  console.log( `Random keys        avg ${ average( distances ).toFixed( 2 ) }` );
};

const testRelatedKeys = ( expandKey ) => {
  const distances = [];
  let key = randomKey(), prev = expandKey( key );

  for ( let i = 0; i < TEST_KEYS; i++ ) {
    key = incrementKey( key );
    const current = expandKey( key );

    distances.push( hammingDistance( prev[ ROUNDS ], current[ ROUNDS ] ) );
    prev = current;
  }

  console.log( `Related keys       avg ${ average( distances ).toFixed( 2 ) }` );
};

const testBitBalance = ( expandKey ) => {
  const counts = new Uint32Array( 8 );

  for ( let i = 0; i < TEST_KEYS; i++ ) {
    const schedule = expandKey( randomKey() );
    const key = schedule[ ROUNDS ];

    for ( const byte of key )
      for ( let bit = 0; bit < 8; bit++ )
        counts[ bit ] += ( byte >>> bit ) & 1;
  }

  const total = TEST_KEYS * KEY_SIZE;
  const values = Array.from( counts, count => ( count / total ) * 100 );

  console.log(
    `Bit balance        min ${ Math.min( ...values ).toFixed( 2 ) }%` +
    `  avg ${ average( values ).toFixed( 2 ) }%` +
    `  max ${ Math.max( ...values ).toFixed( 2 ) }%`
  );
};

const testByteDistribution = ( expandKey ) => {
  const counts = new Uint32Array( 256 );

  for ( let i = 0; i < TEST_KEYS; i++ ) {
    const schedule = expandKey( randomKey() );
    for ( const byte of schedule[ ROUNDS ] ) counts[ byte ]++;
  }

  const expected = ( TEST_KEYS * KEY_SIZE ) / 256;
  const deviations = Array.from( counts, count => ( count - expected ) / expected * 100 );

  console.log(
    `Byte distribution  min ${ Math.min( ...deviations ).toFixed( 1 ) }%` +
    `  avg ${ average( deviations.map( Math.abs ) ).toFixed( 1 ) }%` +
    `  max ${ Math.max( ...deviations ).toFixed( 1 ) }%`
  );
};

const testHammingWeight = ( expandKey ) => {
  const weights = [];

  for ( let i = 0; i < TEST_KEYS; i++ ) {
    const schedule = expandKey( randomKey() );
    for ( const byte of schedule[ ROUNDS ] ) weights.push( countBits( byte ) );
  }

  console.log(
    `Hamming weight     avg ${ average( weights ).toFixed( 2 ) }` +
    `  dev ${ deviation( weights ).toFixed( 2 ) }`
  );
};

const testStructuredKeys = ( expandKey ) => {
  const keys = [];

  keys.push( new Uint8Array( KEY_SIZE ) );
  keys.push( new Uint8Array( KEY_SIZE ).fill( 0xff ) );

  keys.push( Uint8Array.from( { length: KEY_SIZE }, ( _, index ) => index ) );
  keys.push( Uint8Array.from( { length: KEY_SIZE }, ( _, index ) => 0xff - index ) );
  keys.push( Uint8Array.from( { length: KEY_SIZE }, ( _, index ) => index & 1 ? 0xff : 0 ) );
  keys.push( Uint8Array.from( { length: KEY_SIZE }, ( _, index ) => index & 1 ? 0 : 0xff ) );

  console.log( 'Structured keys' );

  for ( const key of keys ) {
    const result = expandKey( key )[ ROUNDS ];
    console.log( `  ${ hex( key ).slice( 0, 23 ).padEnd( 23 ) } → ${ hex( result ).slice( 0, 23 ) }` );
  }
};

console.log( '=== STATISTICAL TESTS ===' );
testDeterminism( algo );
testRandomKeys( algo );
testRelatedKeys( algo );
testBitBalance( algo );
testByteDistribution( algo );
testHammingWeight( algo );
testStructuredKeys( algo );
