import crypto from 'node:crypto';

import { expandKey_v1 } from './expandKey_v1.mjs';
import { KEY_SIZE, ROUNDS, hammingDistance } from './utils.mjs';

const randomKey = () => new Uint8Array( crypto.randomBytes( KEY_SIZE ) );

const flipBit = ( key, bit ) => {
  const result = new Uint8Array( key );
  const byte = Math.floor( bit / 8 );
  const mask = 1 << ( bit % 8 );

  result[ byte ] ^= mask;
  return result;
};

const compareSchedules = ( a, b ) => Array.from(
  { length: ROUNDS + 1 },
  ( _, round ) => hammingDistance( a[ round ], b[ round ] )
);

const average = values => values.reduce( ( sum, value ) => sum + value, 0 ) / values.length;

const testCase = ( name, keyA, keyB ) => {
  const scheduleA = expandKey_v1( keyA );
  const scheduleB = expandKey_v1( keyB );
  const values = compareSchedules( scheduleA, scheduleB );

  console.log( `${ name.padEnd( 18 ) } ` + values.slice( 1 )
    .map( value => value.toString().padStart( 3 ) ).join( '  ' )
  );
};

const avalanche = () => {
  const key = randomKey();
  const values = Array.from( { length: ROUNDS + 1 }, () => [] );

  for ( let bit = 0; bit < KEY_SIZE * 8; bit++ ) {
    const original = expandKey_v1( key );
    const modified = expandKey_v1( flipBit( key, bit ) );

    for ( let round = 0; round <= ROUNDS; round++ )
      values[ round ].push( hammingDistance( original[ round ], modified[ round ] ) );
  }

  console.log( '\n=== BIT AVALANCHE ===' );

  for ( let round = 0; round <= ROUNDS; round++ ) {
    const current = values[ round ];

    console.log(
      `R${ round.toString().padStart( 2, '0' ) } ` +
      `min ${ Math.min( ...current ).toString().padStart( 3 ) } ` +
      `avg ${ average( current ).toFixed( 1 ).padStart( 5 ) } ` +
      `max ${ Math.max( ...current ).toString().padStart( 3 ) }`
    );
  }
};

const zero = new Uint8Array( KEY_SIZE );

const lastBit = new Uint8Array( KEY_SIZE );
lastBit[ KEY_SIZE - 1 ] = 1;

const firstBit = new Uint8Array( KEY_SIZE );
firstBit[ 0 ] = 1;

const lastByte = new Uint8Array( KEY_SIZE );
lastByte[ KEY_SIZE - 1 ] = 0xff;

const firstByte = new Uint8Array( KEY_SIZE );
firstByte[ 0 ] = 0xff;

const allOnes = new Uint8Array( KEY_SIZE ).fill( 0xff );

console.log( '=== KEY SCHEDULE TEST ===' );
console.log( '                   ' + Array.from(
  { length: ROUNDS },
  ( _, round ) => `R${ ( round + 1 ).toString().padStart( 2, '0' ) }`
).join( '  ' ) );

testCase( 'last bit', zero, lastBit );
testCase( 'first bit', zero, firstBit );
testCase( 'last byte', zero, lastByte );
testCase( 'first byte', zero, firstByte );
testCase( 'all bits', zero, allOnes );

avalanche();
