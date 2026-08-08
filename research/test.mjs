import { expandKey_v1 as algo } from './expandKey_v1.mjs';
import { KEY_SIZE, outDiffer, outKeys } from './utils.mjs';

const key1 = new Uint8Array( KEY_SIZE );
const shed1 = algo( key1 );

console.log( '=== SHEDULE 1 ===' );
outKeys( shed1 );

const key2 = new Uint8Array( KEY_SIZE );
key2[ KEY_SIZE - 1 ] = 1;
const shed2 = algo( key2 );

console.log( '=== SHEDULE 2 ===' );
outKeys( shed2 );

console.log( '=== DIFFER ===' );
outDiffer( shed1, shed2 );
