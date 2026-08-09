import { encrypt, decrypt } from './tauron_v2.mjs';
import { hex, KEY_SIZE } from './utils.mjs';

const plaintext = new TextEncoder().encode(
  'Tauron test message. This is a complete encrypt/decrypt test.'
);

const encrypted = encrypt( plaintext, 'test-passphrase' );
const decrypted = decrypt( encrypted, 'test-passphrase' );

console.log( 'PLAINTEXT ', new TextDecoder().decode( plaintext ) );
console.log( 'ENCRYPTED ', hex( encrypted ) );
console.log( 'DECRYPTED ', new TextDecoder().decode( decrypted ) );

const pass = plaintext.length === decrypted.length && plaintext.every( ( value, index ) => value === decrypted[ index ] );

console.log();
console.log( `ROUNDTRIP  ${ pass ? 'PASS' : 'FAIL' }` );