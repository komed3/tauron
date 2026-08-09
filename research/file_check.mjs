import { readFile, writeFile } from 'node:fs/promises';
import { createHash } from 'node:crypto';
import { basename, dirname, join } from 'node:path';

import { encrypt, decrypt } from './tauron_v2.mjs';
