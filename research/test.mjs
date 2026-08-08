import { expandKey_v1 } from './expandKey_v1.mjs';
import { KEY_SIZE, outDiffer, outKeys } from './utils.mjs';

const key1 = new Uint8Array( KEY_SIZE );
const shed1 = expandKey_v1( key1 );

console.log( '=== SHEDULE 1 ===' );
outKeys( shed1 );

const key2 = new Uint8Array( KEY_SIZE );
key2[ KEY_SIZE - 1 ] = 1;
const shed2 = expandKey_v1( key2 );

console.log( '=== SHEDULE 2 ===' );
outKeys( shed2 );

console.log( '=== DIFFER ===' );
outDiffer( shed1, shed2 );
