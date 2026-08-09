import { decryptBlock_v0, encryptBlock_v0 } from './cipher_v0.mjs';
import { hex } from './utils.mjs';

const key = new Uint8Array( 32 );
const plaintext = new Uint8Array( 32 );

plaintext[ 0 ] = 1;

const encrypted = encryptBlock_v0( plaintext, key );
const decrypted = decryptBlock_v0( encrypted, key );

console.log( 'PLAIN  ', hex( plaintext ) );
console.log( 'ENCRYPT', hex( encrypted ) );
console.log( 'DECRYPT', hex( decrypted ) );
