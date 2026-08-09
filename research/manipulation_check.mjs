import { encrypt, decrypt } from './tauron_v2.mjs';


const PASS = 'Tauron manipulation test passphrase';
const SIZE = 1024 * 1024;

const sha256 = async data => {
  const hash = await crypto.subtle.digest( 'SHA-256', data );
  return Array.from( new Uint8Array( hash ) )
    .map( byte => byte.toString( 16 ).padStart( 2, '0' ) )
    .join( '' );
};

const equal = ( a, b ) => a.length === b.length && a.every( ( byte, index ) => byte === b[ index ] );

const flipBit = ( data, byte, bit = 0 ) => {
  const result = new Uint8Array( data );
  result[ byte ] ^= 1 << bit;
  return result;
};

const flipByte = ( data, byte ) => {
  const result = new Uint8Array( data );
  result[ byte ] ^= 0xff;
  return result;
};

const tests = [
  [ 'First bit', data => flipBit( data, 0, 0 ) ],
  [ 'First byte', data => flipByte( data, 0 ) ],
  [ 'Middle bit', data => flipBit( data, Math.floor( data.length / 2 ), 3 ) ],
  [ 'Middle byte', data => flipByte( data, Math.floor( data.length / 2 ) ) ],
  [ 'Last bit', data => flipBit( data, data.length - 1, 7 ) ],
  [ 'Last byte', data => flipByte( data, data.length - 1 ) ],
  [ 'Block boundary', data => flipBit( data, 32, 0 ) ],
  [ 'Block boundary - 1', data => flipBit( data, 31, 0 ) ],
  [ 'Block boundary + 1', data => flipBit( data, 33, 0 ) ]
];

const plaintext = new TextEncoder().encode(
  'Tauron manipulation test. '.repeat( 1000 )
);

const encrypted = encrypt( plaintext, PASS );
const originalHash = await sha256( plaintext );

console.log( '\n## MANIPULATION TEST\n' );
console.log( `Plaintext    ${ plaintext.length } bytes` );
console.log( `Ciphertext   ${ encrypted.length } bytes` );
console.log( `SHA-256      ${ originalHash }\n` );

for ( const [ name, manipulate ] of tests ) {
  const modified = manipulate( encrypted );

  try {
    const decrypted = decrypt( modified, PASS );
    const hash = await sha256( decrypted );
    const intact = equal( plaintext, decrypted );

    console.log(
      `${ name.padEnd( 20 ) } ` +
      `${ intact ? 'NOT DETECTED' : 'DETECTED' } ` +
      `${ intact ? 'FAIL' : 'PASS' }`
    );

    if ( !intact )
      console.log( `  resulting SHA-256 ${ hash }` );
  } catch ( error ) {
    console.log( `${ name.padEnd( 20 ) } DETECTED PASS` );
    console.log( `  ${ error.message }` );
  }
}
