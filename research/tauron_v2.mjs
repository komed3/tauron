import { createHash } from 'node:crypto';
import { BLOCK_SIZE, KEY_SIZE } from './utils.mjs';
import { encryptBlock_v2, decryptBlock_v2 } from './round_v2.mjs';

const CHECKSUM_SIZE = 4;
const LENGTH_SIZE = 1;
const PAYLOAD_SIZE = BLOCK_SIZE - CHECKSUM_SIZE - LENGTH_SIZE;
