import { encryptBlock_v1 } from './round_v1.mjs';
import { runTests } from './testbench.mjs';
import { KEY_SIZE } from './utils.mjs';

const plaintext = new Uint8Array( KEY_SIZE );
plaintext[ 0 ] = 1;

runTests( key => encryptBlock_v1( plaintext, key ) );
