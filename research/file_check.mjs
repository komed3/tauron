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
