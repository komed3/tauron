import { encrypt, decrypt } from './tauron_v2.mjs';


const PASS = 'Tauron random test passphrase';
const SIZES = [ 1024, 1024 * 1024, 10 * 1024 * 1024 ];

const sha256 = async data => {
  const hash = await crypto.subtle.digest( 'SHA-256', data );

  return Array.from( new Uint8Array( hash ) )
    .map( byte => byte.toString( 16 ).padStart( 2, '0' ) )
    .join( '' );
};

const randomBytes = size => {
  const data = new Uint8Array( size );

  for ( let offset = 0; offset < size; offset += 65536 )
    crypto.getRandomValues( data.subarray( offset, Math.min( offset + 65536, size ) ) );

  return data;
};

const equal = ( a, b ) => a.length === b.length && a.every( ( byte, index ) => byte === b[ index ] );

console.log( '\n## RANDOM BINARY ROUNDTRIP\n' );

for ( const size of SIZES ) {
  const plaintext = randomBytes( size );
  const encrypted = encrypt( plaintext, PASS );
  const decrypted = decrypt( encrypted, PASS );

  const originalHash = await sha256( plaintext );
  const decryptedHash = await sha256( decrypted );
  const pass = equal( plaintext, decrypted );

  console.log(
    `${ String( size ).padStart( 8 ) } → ` +
    `${ String( encrypted.length ).padStart( 9 ) } → ` +
    `${ String( decrypted.length ).padStart( 8 ) } ` +
    `${ pass ? 'PASS' : 'FAIL' }`
  );

  console.log( `  SHA-256  ${ originalHash }` );
  console.log( `  SHA-256  ${ decryptedHash }` );
}
