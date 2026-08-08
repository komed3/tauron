import { expandKey_v1 } from './expandKey_v1.mjs';
import { outKeys } from './utils.mjs';

const key1 = new Uint8Array( 32 );
outKeys( expandKey_v1( key1 ) );
