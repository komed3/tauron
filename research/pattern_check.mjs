import { encrypt, decrypt } from './tauron_v2.mjs';


const passphrase = 'Tauron test passphrase';

const SIZES = [ 0, 1, 2, 30, 31, 32, 33, 63, 64, 65, 95, 96, 97 ];

const patterns = [
  [ 'Zero', () => 0x00 ],
  [ 'FF', () => 0xff ],
  [ 'Alternating', i => i & 1 ? 0xff : 0x00 ],
  [ 'Inverse alternating', i => i & 1 ? 0x00 : 0xff ],
  [ 'Ascending', i => i & 0xff ],
  [ 'Descending', i => 0xff - ( i & 0xff ) ]
];

const createData = ( size, fn ) => {
  const data = new Uint8Array( size );
  for ( let i = 0; i < size; i++ ) data[ i ] = fn( i );
  return data;
};

const equal = ( a, b ) => {
  if ( a.length !== b.length ) return false;
  for ( let i = 0; i < a.length; i++ ) if ( a[ i ] !== b[ i ] ) return false;
  return true;
};

for ( const [ name, pattern ] of patterns ) {
  console.log( `\n${ name }` );

  for ( const size of SIZES ) {
    const plaintext = createData( size, pattern );

    try {
      const encrypted = encrypt( plaintext, passphrase );
      const decrypted = decrypt( encrypted, passphrase );
      const pass = equal( plaintext, decrypted );

      console.log(
        String( size ).padStart( 3 ),
        '→',
        String( encrypted.length ).padStart( 3 ),
        '→',
        String( decrypted.length ).padStart( 3 ),
        pass ? 'PASS' : 'FAIL'
      );
    } catch ( error ) {
      console.log(
        String( size ).padStart( 3 ),
        'ERROR',
        error.message
      );
    }
  }
}
