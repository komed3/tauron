import { encrypt, decrypt } from './tauron_v2.mjs';


const passphrase = 'Tauron test passphrase';

const createData = size => {
  const data = new Uint8Array( size );
  for ( let i = 0; i < size; i++ ) data[ i ] = i & 0xff;
  return data;
};

const equal = ( a, b ) => {
  if ( a.length !== b.length ) return false;
  for ( let i = 0; i < a.length; i++ ) if ( a[ i ] !== b[ i ] ) return false;
  return true;
};

const tests = [
  [ 'Empty', 0 ],
  [ '1 bit', 1 ],
  [ '1 byte', 1 ],
  [ '2 bytes', 2 ],
  [ '31 bytes', 31 ],
  [ '32 bytes', 32 ],
  [ '33 bytes', 33 ],
  [ '63 bytes', 63 ],
  [ '64 bytes', 64 ],
  [ '65 bytes', 65 ],
  [ '95 bytes', 95 ],
  [ '96 bytes', 96 ],
  [ '97 bytes', 97 ]
];

for ( const [ name, size ] of tests ) {
  const plaintext = createData( size );

  try {
    const encrypted = encrypt( plaintext, passphrase );
    const decrypted = decrypt( encrypted, passphrase );
    const pass = equal( plaintext, decrypted );

    console.log(
      name.padEnd( 12 ),
      String( plaintext.length ).padStart( 4 ),
      '→',
      String( encrypted.length ).padStart( 4 ),
      '→',
      String( decrypted.length ).padStart( 4 ),
      pass ? 'PASS' : 'FAIL'
    );
  } catch ( error ) {
    console.log(
      name.padEnd( 12 ),
      'ERROR',
      error.message
    );
  }
}
