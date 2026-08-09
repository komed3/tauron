import { encryptBlock_v1 } from './round_v1.mjs';
import { runTests } from './testbench.mjs';

const plaintext = new Uint8Array( 32 );
plaintext[ 0 ] = 1;

runTests( key => encryptBlock_v1( plaintext, key ) );
