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
