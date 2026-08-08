import { hammingDistance } from './hamming.mjs';
import { expandKey } from './key-schedule.mjs';
import { hex, KEY_SIZE, ROUNDS } from './utils.mjs';

const key = new Uint8Array( KEY_SIZE );

for ( let i = 0; i < key.length; i++ ) key[ i ] = i;
const keys = expandKey( key );

keys.forEach( ( key, round ) => console.log(
  `Round ${ round.toString().padStart( 2, '0' ) }: ${ hex( key ) }`
) );

const keyA = new Uint8Array( KEY_SIZE );
const keyB = new Uint8Array( KEY_SIZE );

keyB[ KEY_SIZE - 1 ] = 1;

const scheduleA = expandKey( keyA );
const scheduleB = expandKey( keyB );

for ( let round = 0; round <= ROUNDS; round++ ) {
  const distance = hammingDistance( scheduleA[ round ], scheduleB[ round ] );
  console.log( `Round ${ round }: ${ distance }/256 bits differ` );
}
