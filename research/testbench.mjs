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

const format = ( value ) => value.toFixed( 1 ).padStart( 5, ' ' );

const stats = ( values ) => {
  let min = Infinity, max = -Infinity, sum = 0;

  for ( const value of values ) {
    if ( value < min ) min = value;
    if ( value > max ) max = value;
    sum += value;
  }

  return { min, avg: sum / values.length, max };
};

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

  console.log( `Schedule format      ${ pass ? 'PASS' : 'FAIL' }` );
  return pass;
};

const testDeterminism = ( expandKey ) => {
  let pass = true;

  for ( let i = 0; i < SAMPLES; i++ ) {
    const key = randomKey();
    const a = expandKey( key ), b = expandKey( key );

    if ( a.length !== b.length || a.some( ( round, index ) => !equalBytes( round, b[ index ] ) ) ) {
      pass = false;
      break;
    }
  }

  console.log( `Determinism           ${ pass ? 'PASS' : 'FAIL' }` );
  return pass;
};
