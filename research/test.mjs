import { expandKey_v2 } from './expandKey_v2.mjs';
import { runTests } from './testbench.mjs';

runTests( key => expandKey_v2( key ) );
