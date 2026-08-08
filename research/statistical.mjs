import { KEY_SIZE, ROUNDS, hammingDistance, hex } from './utils.mjs';

const TEST_KEYS = 1000;

const randomKey = () => {
  const key = new Uint8Array( KEY_SIZE );
  crypto.getRandomValues( key );
  return key;
};

const incrementKey = key => {
  const result = new Uint8Array( key );

  for ( let i = result.length - 1; i >= 0; i-- ) {
    result[ i ]++;

    if ( result[ i ] !== 0 ) break;
  }

  return result;
};

const countBits = byte => {
  let bits = 0;

  while ( byte ) {
    bits += byte & 1;
    byte >>>= 1;
  }

  return bits;
};
