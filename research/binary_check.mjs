import { encrypt, decrypt } from './tauron_v2.mjs';


const passphrase = 'Tauron binary test';

const SIZES = [ 1, 15, 29, 30, 31, 32, 33, 47, 63, 64, 65, 127, 128, 129, 255, 256, 257, 1023, 1024, 1025 ];

const createData = size => {
  const data = new Uint8Array( size );

  let value = 0x6d;

  for ( let i = 0; i < size; i++ ) {
    value ^= value << 13;
    value ^= value >>> 17;
    value ^= value << 5;
    value >>>= 0;

    data[ i ] = value & 0xff;
  }

  return data;
};

const equal = ( a, b ) => {
  if ( a.length !== b.length ) return false;
  for ( let i = 0; i < a.length; i++ ) if ( a[ i ] !== b[ i ] ) return false;
  return true;
};

console.log( 'Binary roundtrip test' );
console.log( '---------------------' );

for ( const size of SIZES ) {
  const plaintext = createData( size );

  try {
    const encrypted = encrypt( plaintext, passphrase );
    const decrypted = decrypt( encrypted, passphrase );
    const pass = equal( plaintext, decrypted );

    console.log(
      String( size ).padStart( 5 ),
      '→',
      String( encrypted.length ).padStart( 5 ),
      '→',
      String( decrypted.length ).padStart( 5 ),
      pass ? 'PASS' : 'FAIL'
    );
  } catch ( error ) {
    console.log(
      String( size ).padStart( 5 ),
      'ERROR',
      error.message
    );
  }
}
