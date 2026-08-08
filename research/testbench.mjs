import { KEY_SIZE, ROUNDS, cloneKey, hammingDistance, hex, randomKey } from './utils.mjs';

const SAMPLES = 10_000;
const COLLISION_SAMPLES = 10_000;

const flipBit = ( key, bit ) => {
  const result = cloneKey( key );
  result[ Math.floor( bit / 8 ) ] ^= 1 << ( bit % 8 );

  return result;
};

const flipBits = ( key, count ) => {
  const result = cloneKey( key ), positions = new Set();

  while ( positions.size < count ) positions.add( Math.floor( Math.random() * KEY_SIZE * 8 ) );
  for ( const bit of positions ) result[ Math.floor( bit / 8 ) ] ^= 1 << ( bit % 8 );

  return result;
};

const byteDistance = ( a, b ) => {
  let bytes = 0;
  for ( let i = 0; i < a.length; i++ ) if ( a[ i ] !== b[ i ] ) bytes++;

  return bytes;
};

const equalBytes = ( a, b ) => {
  if ( a.length !== b.length ) return false;
  for ( let i = 0; i < a.length; i++ ) if ( a[ i ] !== b[ i ] ) return false;

  return true;
};

const section = ( title ) => {
  console.log();
  console.log( `=== ${ title } ===` );
};

const stats = ( values ) => {
  let min = Infinity, max = -Infinity, sum = 0;

  for ( const value of values ) {
    if ( value < min ) min = value;
    if ( value > max ) max = value;
    sum += value;
  }

  return { min, avg: sum / values.length, max };
};

const format = ( value ) => value.toFixed( 1 ).padStart( 5, ' ' );

const printStats = ( name, values, suffix = '' ) => {
  const { min, avg, max } = stats( values );

  console.log(
    `${ name.padEnd( 22 ) }  min ${ format( min ) }  avg ${ format( avg ) }  ` +
    `max ${ format( max ) }${ suffix }`
  );
};

const testScheduleFormat = ( expandKey ) => {
  let pass = true;

  for ( let i = 0; i < SAMPLES; i++ ) {
    const key = randomKey(), schedule = expandKey( key );

    if ( ! Array.isArray( schedule ) || schedule.length !== ROUNDS + 1 ) {
      pass = false;
      break;
    }

    if ( schedule.some( round => ! ( round instanceof Uint8Array ) || round.length !== KEY_SIZE ) ) {
      pass = false;
      break;
    }
  }

  console.log( `Schedule format       ${ pass ? 'PASS' : 'FAIL' }` );
  return pass;
};

const testDeterminism = ( expandKey ) => {
  let pass = true;

  for ( let i = 0; i < SAMPLES; i++ ) {
    const key = randomKey();
    const a = expandKey( key ), b = expandKey( key );

    if ( a.length !== b.length || a.some( ( round, index ) => ! equalBytes( round, b[ index ] ) ) ) {
      pass = false;
      break;
    }
  }

  console.log( `Determinism           ${ pass ? 'PASS' : 'FAIL' }` );
  return pass;
};

const testRoundZero = ( expandKey ) => {
  let pass = true;

  for ( let i = 0; i < SAMPLES; i++ ) {
    const key = randomKey(), schedule = expandKey( key );

    if ( ! equalBytes( key, schedule[ 0 ] ) ) {
      pass = false;
      break;
    }
  }

  console.log( `Round 0 preservation  ${ pass ? 'PASS' : 'FAIL' }` );
  return pass;
};

const testCollisions = ( expandKey ) => {
  const seen = new Set();
  let collisions = 0;

  for ( let i = 0; i < COLLISION_SAMPLES; i++ ) {
    const key = randomKey(), schedule = expandKey( key );
    const final = hex( schedule[ ROUNDS ] );

    if ( seen.has( final ) ) collisions++;
    else seen.add( final );
  }

  console.log( `Collisions            ${ collisions === 0 ? 'PASS' : `FAIL (${ collisions })` }` );
  return collisions === 0;
};

const testAvalanche = ( expandKey ) => {
  const values = [];

  for ( let i = 0; i < SAMPLES; i++ ) {
    const key = randomKey();
    const changed = flipBit( key, Math.floor( Math.random() * KEY_SIZE * 8 ) );

    const a = expandKey( key ), b = expandKey( changed );
    values.push( hammingDistance( a[ ROUNDS ], b[ ROUNDS ] ) );
  }

  printStats( 'Avalanche', values, '/256 bits' );
};

const testMultiBitAvalanche = ( expandKey ) => {
  const counts = [ 1, 2, 4, 8, 16, 32, 64, 128, 256 ];

  for ( const count of counts ) {
    const values = [];

    for ( let i = 0; i < SAMPLES; i++ ) {
      const key = randomKey(), changed = flipBits( key, count );
      const a = expandKey( key ), b = expandKey( changed );

      values.push( hammingDistance( a[ ROUNDS ], b[ ROUNDS ] ) );
    }

    const { avg } = stats( values );

    console.log( `${ count.toString().padStart( 6 ) } bits  →  ${ avg.toFixed( 1 ).padStart( 5 ) }` );
  }
};

const testPositionAvalanche = ( expandKey ) => {
  const values = [];

  for ( let round = 1; round <= ROUNDS; round++ ) {
    const distances = [];

    for ( let i = 0; i < SAMPLES; i++ ) {
      const key = randomKey(), changed = flipBit( key, Math.floor( Math.random() * KEY_SIZE * 8 ) );
      const a = expandKey( key ), b = expandKey( changed );

      distances.push( hammingDistance( a[ round ], b[ round ] ) );
    }

    values.push( { round, ...stats( distances ) } );
  }

  console.log( 'Round'.padEnd( 7 ) + 'min'.padStart( 5 ) + 'avg'.padStart( 7 ) + 'max'.padStart( 7 ) );

  for ( const value of values ) console.log(
    `R${ value.round.toString().padStart( 2, '0' ) }` + value.min.toString().padStart( 9 ) +
    value.avg.toFixed( 1 ).padStart( 7 ) + value.max.toString().padStart( 7 )
  );
};

const testRelatedKeys = ( expandKey ) => {
  const values = [];

  for ( let i = 0; i < SAMPLES; i++ ) {
    const key = randomKey(), related = cloneKey( key );
    const changes = 1 + Math.floor( Math.random() * 8 );

    for ( let j = 0; j < changes; j++ ) {
      const index = Math.floor( Math.random() * KEY_SIZE );
      related[ index ] ^= Math.floor( Math.random() * 255 ) + 1;
    }

    const a = expandKey( key ), b = expandKey( related );
    values.push( hammingDistance( a[ ROUNDS ], b[ ROUNDS ] ) );
  }

  printStats( 'Related keys', values, '/256 bits' );
};

const testBitBalance = ( expandKey ) => {
  const counts = new Uint32Array( KEY_SIZE * 8 );

  for ( let i = 0; i < SAMPLES; i++ ) {
    const key = randomKey(), output = expandKey( key )[ ROUNDS ];

    for ( let byte = 0; byte < output.length; byte++ ) for ( let bit = 0; bit < 8; bit++ )
      if ( output[ byte ] & ( 1 << bit ) ) counts[ byte * 8 + bit ]++;
  }

  const percentages = Array.from( counts, count => count / SAMPLES * 100 );
  printStats( 'Bit balance', percentages, '%' );
};

const testByteDistribution = ( expandKey ) => {
  const counts = new Uint32Array( 256 );

  for ( let i = 0; i < SAMPLES; i++ ) {
    const key = randomKey(), output = expandKey( key )[ ROUNDS ];
    for ( const byte of output ) counts[ byte ]++;
  }

  const expected = SAMPLES * KEY_SIZE / 256;
  const deviations = Array.from( counts, count => ( count - expected ) / expected * 100 );
  printStats( 'Byte distribution', deviations, '%' );
};

const testHammingWeight = ( expandKey ) => {
  const values = [];

  for ( let i = 0; i < SAMPLES; i++ ) {
    const key = randomKey(), output = expandKey( key )[ ROUNDS ];
    let bits = 0;

    for ( const byte of output ) {
      let value = byte;

      while ( value ) {
        bits += value & 1;
        value >>>= 1;
      }
    }

    values.push( bits );
  }

  const { avg } = stats( values );
  const deviation = Math.sqrt( values.reduce( ( sum, value ) => sum + ( value - avg ) ** 2, 0 ) / values.length );
  console.log( `Hamming weight          avg ${ avg.toFixed( 2 ) }  dev ${ deviation.toFixed( 2 ) }` );
};

const testRoundUniqueness = ( expandKey ) => {
  let duplicateSchedules = 0, duplicatePairs = 0;

  for ( let i = 0; i < SAMPLES; i++ ) {
    const schedule = expandKey( randomKey() );
    const seen = new Set();

    for ( const round of schedule ) {
      const value = hex( round );

      if ( seen.has( value ) ) duplicatePairs++;
      else seen.add( value );
    }

    if ( seen.size !== schedule.length ) duplicateSchedules++;
  }

  console.log( `Round uniqueness      ${ duplicateSchedules === 0 ? 'PASS' : `FAIL (${ duplicateSchedules })` }` );
  if ( duplicatePairs ) console.log( `  duplicate rounds   ${ duplicatePairs }` );

  return duplicateSchedules === 0;
};

const testRoundDistances = ( expandKey ) => {
  const values = Array.from( { length: ROUNDS }, () => [] );

  for ( let i = 0; i < SAMPLES; i++ ) {
    const schedule = expandKey( randomKey() );

    for ( let round = 1; round <= ROUNDS; round++ )
      values[ round - 1 ].push( hammingDistance( schedule[ round - 1 ], schedule[ round ] ) );
  }

  console.log( 'Round'.padEnd( 7 ) + 'min'.padStart( 5 ) + 'avg'.padStart( 7 ) + 'max'.padStart( 7 ) );

  values.forEach( ( distances, index ) => {
    const { min, avg, max } = stats( distances );

    console.log(
      `R${ ( index + 1 ).toString().padStart( 2, '0' ) }` + min.toString().padStart( 9 ) +
      avg.toFixed( 1 ).padStart( 7 ) + max.toString().padStart( 7 )
    );
  } );
};

const testRoundRepetition = ( expandKey ) => {
  const distances = [];

  for ( let i = 0; i < SAMPLES; i++ ) {
    const schedule = expandKey( randomKey() );

    for ( let a = 0; a < schedule.length; a++ ) for ( let b = a + 1; b < schedule.length; b++ )
      distances.push( hammingDistance( schedule[ a ], schedule[ b ] ) );
  }

  printStats( 'Round repetition', distances, '/256 bits' );
};

const testStructuredKeys = ( expandKey ) => {
  const keys = [];

  keys.push( new Uint8Array( KEY_SIZE ) );
  keys.push( new Uint8Array( KEY_SIZE ).fill( 0xff ) );
  keys.push( Uint8Array.from( { length: KEY_SIZE }, ( _, index ) => index ) );
  keys.push( Uint8Array.from( { length: KEY_SIZE }, ( _, index ) => 0xff - index ) );
  keys.push( Uint8Array.from( { length: KEY_SIZE }, ( _, index ) => index % 2 ? 0xff : 0x00 ) );
  keys.push( Uint8Array.from( { length: KEY_SIZE }, ( _, index ) => index % 2 ? 0x00 : 0xff ) );
  keys.push( Uint8Array.from( { length: KEY_SIZE }, ( _, index ) => index === 0 ? 0x80 : 0x00 ) );
  keys.push( Uint8Array.from( { length: KEY_SIZE }, ( _, index ) => index === KEY_SIZE - 1 ? 0x80 : 0x00 ) );

  for ( const key of keys ) console.log(
    `${ hex( key.subarray( 0, 8 ) ).padEnd( 23 ) }  →  ` +
    hex( expandKey( key )[ ROUNDS ].subarray( 0, 8 ) )
  );
};

const testZeroPatterns = ( expandKey ) => {
  const patterns = [ new Uint8Array( KEY_SIZE ), new Uint8Array( KEY_SIZE ).fill( 0xff ) ];

  for ( const key of patterns ) {
    const schedule = expandKey( key );
    const unique = new Set( schedule.map( hex ) ).size;

    console.log(
      `${ hex( key.subarray( 0, 4 ) ).padEnd( 14 ) } ` +
      `${ unique }/${ schedule.length } unique rounds`
    );
  }
};

const testRoundCorrelation = ( expandKey ) => {
  const values = [];

  for ( let i = 0; i < SAMPLES; i++ ) {
    const schedule = expandKey( randomKey() );

    for ( let round = 1; round < ROUNDS; round++ ) {
      const a = schedule[ round ], b = schedule[ round + 1 ];
      values.push( hammingDistance( a, b ) );
    }
  }

  printStats( 'Round correlation', values, '/256 bits' );
};

const testPermutationSymmetry = ( expandKey ) => {
  const key = new Uint8Array( KEY_SIZE );

  for ( let i = 0; i < KEY_SIZE; i++ ) key[ i ] = i;
  const a = expandKey( key );

  key.reverse();
  const b = expandKey( key );

  const distance = hammingDistance( a[ ROUNDS ], b[ ROUNDS ] );
  console.log( `Reverse-key distance  ${ distance }/256 bits` );
};

export const runTests = ( expandKey, { samples = SAMPLES, collisionSamples = COLLISION_SAMPLES } = {} ) => {
  if ( typeof expandKey !== 'function' ) throw new TypeError( 'runTests() expects a key expansion function' );

  console.log( '=== KEY SCHEDULE ANALYSIS ===' );
  console.log( `Samples: ${ samples.toLocaleString() }` );
  console.log( `Collisions: ${ collisionSamples.toLocaleString() }` );

  section( 'STRUCTURAL TESTS' );

  testScheduleFormat( expandKey );
  testDeterminism( expandKey );
  testRoundZero( expandKey );
  testCollisions( expandKey );

  section( 'AVALANCHE TESTS' );

  testAvalanche( expandKey );
  console.log( 'Multi-bit avalanche' );
  testMultiBitAvalanche( expandKey );

  console.log( 'Position avalanche' );
  testPositionAvalanche( expandKey );

  testRelatedKeys( expandKey );

  section( 'STATISTICAL TESTS' );

  testBitBalance( expandKey );
  testByteDistribution( expandKey );
  testHammingWeight( expandKey );

  section( 'ROUND STRUCTURE' );

  testRoundUniqueness( expandKey );
  console.log( 'Round-to-round distance' );
  testRoundDistances( expandKey );

  testRoundRepetition( expandKey );
  testRoundCorrelation( expandKey );

  section( 'STRUCTURED INPUTS' );

  testStructuredKeys( expandKey );
  testZeroPatterns( expandKey );
  testPermutationSymmetry( expandKey );

  section( 'ANALYSIS COMPLETE' );
};
