import { readFile, writeFile } from 'node:fs/promises';
import { createHash } from 'node:crypto';
import { basename, dirname, join } from 'node:path';

import { encrypt, decrypt } from './tauron_v2.mjs';


const passphrase = 'Tauron file test';

const input = process.argv[ 2 ];

if ( ! input ) {
  console.error( 'Usage: node file_check.mjs <file>' );
  process.exit( 1 );
}

const hash = ( data ) => createHash( 'sha256' ).update( data ).digest( 'hex' );

const original = new Uint8Array( await readFile( input ) );
const encrypted = encrypt( original, passphrase );
const decrypted = decrypt( encrypted, passphrase );

const directory = dirname( input );
const name = basename( input );

const encryptedPath = join( directory, `${ name }.tau` );
const restoredPath = join( directory, `restored_${ name }` );

await writeFile( encryptedPath, encrypted );
await writeFile( restoredPath, decrypted );

const originalHash = hash( original );
const decryptedHash = hash( decrypted );

console.log( 'FILE ROUNDTRIP' );
console.log( '---------------' );
console.log( 'Input       ', name );
console.log( 'Original    ', original.length, 'bytes' );
console.log( 'Encrypted   ', encrypted.length, 'bytes' );
console.log( 'Decrypted   ', decrypted.length, 'bytes' );
console.log( 'SHA-256     ', originalHash );
console.log( 'SHA-256     ', decryptedHash );
console.log( 'ROUNDTRIP   ', originalHash === decryptedHash ? 'PASS' : 'FAIL' );
console.log( 'ENCRYPTED   ', encryptedPath );
console.log( 'RESTORED    ', restoredPath );
