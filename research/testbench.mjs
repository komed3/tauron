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
