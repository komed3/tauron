import { createHash } from 'node:crypto';

import { expandKey_v2 } from './expandKey_v2.mjs';
import { decrypt_v2, encrypt_v2 } from './round_v2.mjs';
import { BLOCK_SIZE, KEY_SIZE, ROUNDS } from './utils.mjs';


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

const parseBlock = ( block ) => {
  if ( block.length !== BLOCK_SIZE ) throw new RangeError( `Block must contain exactly ${ BLOCK_SIZE } bytes` );

  const length = block[ 0 ];
  if ( length > PAYLOAD_SIZE ) throw new Error( `Invalid payload length: ${ length }` );

  const stored = block.subarray( LENGTH_SIZE, LENGTH_SIZE + CHECKSUM_SIZE );
  const data = block.subarray( LENGTH_SIZE + CHECKSUM_SIZE, LENGTH_SIZE + CHECKSUM_SIZE + length );
  const expected = checksum( data );

  for ( let i = 0; i < CHECKSUM_SIZE; i++ ) if ( stored[ i ] !== expected[ i ] )
    throw new Error( 'Checksum verification failed' );

  return new Uint8Array( data );
};

const splitBlocks = ( data ) => {
  if ( data.length === 0 ) return [ createBlock( data ) ];
  const blocks = [];

  for ( let offset = 0; offset < data.length; offset += PAYLOAD_SIZE )
    blocks.push( createBlock( data.subarray( offset, Math.min( offset + PAYLOAD_SIZE, data.length ) ) ) );

  return blocks;
};

export const encrypt = ( data, passphrase, { rounds = ROUNDS } = {} ) => {
  const blocks = splitBlocks( data ), output = new Uint8Array( blocks.length * BLOCK_SIZE );
  const keys = expandKey_v2( deriveKey( passphrase ), rounds );

  for ( let i = 0; i < blocks.length; i++ ) output.set( encrypt_v2( blocks[ i ], keys ), i * BLOCK_SIZE );
  return output;
};

export const decrypt = ( data, passphrase, { rounds = ROUNDS } = {} ) => {
  if ( data.length === 0 || data.length % BLOCK_SIZE !== 0 )
    throw new RangeError( 'Ciphertext length must be a non-zero multiple of block size' );

  const keys = expandKey_v2( deriveKey( passphrase ), rounds ), parts = [];

  for ( let offset = 0; offset < data.length; offset += BLOCK_SIZE )
    parts.push( parseBlock( decrypt_v2( data.subarray( offset, offset + BLOCK_SIZE ), keys ) ) );

  const output = new Uint8Array( parts.reduce( ( total, part ) => total + part.length, 0 ) );

  let offset = 0;
  for ( const part of parts ) {
    output.set( part, offset );
    offset += part.length;
  }

  return output;
};
