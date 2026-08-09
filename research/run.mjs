import { decryptBlock_v1 as decrypt, encryptBlock_v1 as encrypt } from './round_v1.mjs';
import { hex } from './utils.mjs';

const key = new Uint8Array( 32 );
const plaintext = new Uint8Array( 32 );

plaintext[ 0 ] = 1;

const encrypted = encrypt( plaintext, key );
const decrypted = decrypt( encrypted, key );

console.log( 'PLAIN  ', hex( plaintext ) );
console.log( 'ENCRYPT', hex( encrypted ) );
console.log( 'DECRYPT', hex( decrypted ) );
