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

const testDeterminism = ( algo ) => {
  const key = randomKey();

  const a = algo( key );
  const b = algo( key );

  const identical = a.every( ( value, index ) => value === b[ index ] );
  console.log( `Determinism       ${ identical ? 'PASS' : 'FAIL' }` );
};
