import { createHash } from 'node:crypto';
import { BLOCK_SIZE, KEY_SIZE } from './utils.mjs';
import { encryptBlock_v2, decryptBlock_v2 } from './round_v2.mjs';

const CHECKSUM_SIZE = 4;
const LENGTH_SIZE = 1;
const PAYLOAD_SIZE = BLOCK_SIZE - CHECKSUM_SIZE - LENGTH_SIZE;

const checksum = ( data ) => createHash( 'sha256' ).update( data ).digest().subarray( 0, CHECKSUM_SIZE );

export const deriveKey = ( passphrase ) => {
  if ( typeof passphrase !== 'string' ) throw new TypeError( 'Passphrase must be a string' );
  return new Uint8Array( createHash( 'sha256' ).update( passphrase, 'utf8' ).digest().subarray( 0, KEY_SIZE ) );
};

const createBlock = ( data ) => {
  if ( data.length > PAYLOAD_SIZE ) throw new RangeError( `Block payload exceeds ${ PAYLOAD_SIZE } bytes` );

  const block = new Uint8Array( BLOCK_SIZE ), sum = checksum( data );

  block[ 0 ] = data.length;
  block.set( sum, LENGTH_SIZE );
  block.set( data, LENGTH_SIZE + CHECKSUM_SIZE );

  return block;
};
