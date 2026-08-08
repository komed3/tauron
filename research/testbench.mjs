import { KEY_SIZE, ROUNDS, hammingDistance } from './utils.mjs';

const randomKey = () => {
  const key = new Uint8Array( KEY_SIZE );
  crypto.getRandomValues( key );
  return key;
};

const clone = key => new Uint8Array( key );

const flipBit = ( key, bit ) => {
  const result = clone( key );
  result[ Math.floor( bit / 8 ) ] ^= 1 << ( bit % 8 );
  return result;
};

const flipBits = ( key, bits ) => {
  const result = clone( key );

  for ( const bit of bits ) result[ Math.floor( bit / 8 ) ] ^= 1 << ( bit % 8 );
  return result;
};

const xorKeys = ( a, b ) => {
  const result = new Uint8Array( a.length );

  for ( let i = 0; i < a.length; i++ ) result[ i ] = a[ i ] ^ b[ i ];
  return result;
};

const flatten = schedule => schedule.slice( 1 ).flatMap( round => Array.from( round ) );
const average = values => values.reduce( ( sum, value ) => sum + value, 0 ) / values.length;

const testDeterminism = expandKey => {
  const key = randomKey();

  const a = expandKey( key );
  const b = expandKey( key );

  return a.every( ( round, i ) => round.every( ( value, byte ) => value === b[ i ][ byte ] ) );
};

const testCollisions = ( expandKey, samples ) => {
  const seen = new Set();

  for ( let i = 0; i < samples; i++ ) {
    const schedule = expandKey( randomKey() );
    const value = flatten( schedule ).join( ',' );

    if ( seen.has( value ) ) return false;
    seen.add( value );
  }

  return true;
};

const testAvalanche = ( expandKey, samples ) => {
  const values = [];

  for ( let i = 0; i < samples; i++ ) {
    const key = randomKey();
    const changed = flipBit( key, Math.floor( Math.random() * ( KEY_SIZE * 8 ) ) );

    const a = expandKey( key ), b = expandKey( changed );
    for ( let round = 0; round <= ROUNDS; round++ ) values.push( hammingDistance( a[ round ], b[ round ] ) );
  }

  return { min: Math.min( ...values ), avg: average( values ), max: Math.max( ...values ) };
};

const testMultiBitAvalanche = ( expandKey, samples ) => {
  const changes = [ 1, 2, 4, 8, 16, 32, 128, 256 ];
  const result = {};

  for ( const count of changes ) {
    const values = [];

    for ( let i = 0; i < samples; i++ ) {
      const key = randomKey();
      const bits = [];

      while ( bits.length < count ) {
        const bit = Math.floor( Math.random() * KEY_SIZE * 8 );
        if ( ! bits.includes( bit ) ) bits.push( bit );
      }

      const changed = flipBits( key, bits );

      const a = expandKey( key ), b = expandKey( changed );
      for ( let round = 1; round <= ROUNDS; round++ ) values.push( hammingDistance( a[ round ], b[ round ] ) );
    }

    result[ count ] = average( values );
  }

  return result;
};

const testRelatedKeys = ( expandKey, samples ) => {
  const diffs = [];

  for ( let i = 0; i < samples; i++ ) {
    const key = randomKey(), delta = randomKey();
    const related = xorKeys( key, delta );

    const a = expandKey( key ), b = expandKey( related );
    for ( let round = 1; round <= ROUNDS; round++ ) diffs.push( hammingDistance( a[ round ], b[ round ] ) );
  }

  return average( diffs );
};

const testPositions = expandKey => {
  const key = randomKey(), base = expandKey( key );
  const result = [];

  for ( let bit = 0; bit < KEY_SIZE * 8; bit++ ) {
    const changed = expandKey( flipBit( key, bit ) );
    result.push( changed.map( ( round, index ) => hammingDistance( base[ index ], round ) ) );
  }

  return result;
};

const testStructuredKeys = expandKey => {
  const patterns = [
    new Uint8Array( KEY_SIZE ),
    new Uint8Array( KEY_SIZE ).fill( 0xff ),
    Uint8Array.from( { length: KEY_SIZE }, ( _, i ) => i ),
    Uint8Array.from( { length: KEY_SIZE }, ( _, i ) => 0xff - i ),
    Uint8Array.from( { length: KEY_SIZE }, ( _, i ) => i % 2 ? 0xff : 0 ),
    Uint8Array.from( { length: KEY_SIZE }, ( _, i ) => i % 2 ? 0 : 0xff )
  ];

  return patterns.map( key => ( { input: key, output: expandKey( key ).at( -1 ) } ) );
};

const testRoundBias = ( expandKey, samples ) => {
  const counts = Array.from( { length: ROUNDS }, () => new Uint32Array( KEY_SIZE * 8 ) );

  for ( let i = 0; i < samples; i++ ) {
    const schedule = expandKey( randomKey() );

    for ( let round = 1; round <= ROUNDS; round++ ) {
      const key = schedule[ round ];

      for ( let byte = 0; byte < KEY_SIZE; byte++ )
        for ( let bit = 0; bit < 8; bit++ )
          if ( key[ byte ] & ( 1 << bit ) )
            counts[ round - 1 ][ byte * 8 + bit ]++;
    }
  }

  return counts;
};

export const runTests = ( expandKey, {
  collisionSamples = 10_000,
  avalancheSamples = 100,
  multiBitSamples = 25,
  relatedKeySamples = 100
} = {} ) => {
  console.log( '=== STATISTICAL TESTS ===' );
  console.log( `Determinism        ${ testDeterminism( expandKey ) ? 'PASS' : 'FAIL' }` );
  console.log( `Collisions         ${ testCollisions( expandKey, collisionSamples ) ? 'PASS' : 'FAIL' }` );

  const avalanche = testAvalanche( expandKey, avalancheSamples );
  console.log( `Avalanche          min ${ avalanche.min }  avg ${ avalanche.avg.toFixed( 1 ) }  max ${ avalanche.max }` );


  console.log( 'Multi-bit avalanche' );
  const multi = testMultiBitAvalanche( expandKey, multiBitSamples );
  for ( const [ bits, value ] of Object.entries( multi ) ) console.log(
    `  ${ bits.padStart( 3, ' ' )} bit${ bits === '1' ? ' ' : 's'} → ${ value.toFixed( 1 ) }`
  );

  console.log( `Related keys       avg ${ testRelatedKeys( expandKey, relatedKeySamples ).toFixed( 2 ) }` );

  console.log( 'Position avalanche' );
  const positions = testPositions( expandKey );

  for ( let round = 1; round <= ROUNDS; round++ ) {
    const values = positions.map( result => result[ round ] );

    console.log(
      `  R${ round.toString().padStart( 2, '0' )}  min ${
        Math.min( ...values ).toString().padStart( 3, ' ')
      }  avg ${
        average( values ).toFixed( 1 ).padStart( 5, ' ')
      }  max ${
        Math.max( ...values ).toString().padStart( 3, ' ')
      }`
    );
  }

  console.log( 'Structured keys' );

  for ( const { input, output } of testStructuredKeys( expandKey ) )
    console.log( `  ${ Array.from( input.slice( 0, 8 ), byte => byte.toString( 16 ).padStart( 2, '0' ) ).join( ' ' ) } → ${
      Array.from( output.slice( 0, 8 ), byte => byte.toString( 16 ).padStart( 2, '0' ) ).join( ' ' )
    }` );

  return { positions, roundBias: testRoundBias( expandKey, avalancheSamples ) };
};
