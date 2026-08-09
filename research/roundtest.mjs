import { encryptBlock_v1 } from './round_v1.mjs';
import { BLOCK_SIZE, hammingDistance, hex } from './utils.mjs';

const SAMPLES = 10_000;
const COLLISION_SAMPLES = 10_000;
const POSITION_SAMPLES = 100;

const randomBytes = ( length = BLOCK_SIZE ) => {
const result = new Uint8Array( length );

for ( let i = 0; i < result.length; i++ ) result[ i ] = Math.floor( Math.random() * 256 );

return result;
};

const cloneBytes = ( value ) => new Uint8Array( value );

const flipBit = ( value, bit ) => {
const result = cloneBytes( value );
result[ Math.floor( bit / 8 ) ] ^= 1 << ( bit % 8 );

return result;
};

const flipBits = ( value, count ) => {
const result = cloneBytes( value ), positions = new Set();

while ( positions.size < count ) positions.add( Math.floor( Math.random() * BLOCK_SIZE * 8 ) );
for ( const bit of positions ) result[ Math.floor( bit / 8 ) ] ^= 1 << ( bit % 8 );

return result;
};

const equalBytes = ( a, b ) => {
if ( a.length !== b.length ) return false;
for ( let i = 0; i < a.length; i++ ) if ( a[ i ] !== b[ i ] ) return false;

return true;
};

const stats = ( values ) => {
let min = Infinity, max = -Infinity, sum = 0;

for ( const value of values ) {
if ( value < min ) min = value;
if ( value > max ) max = value;
sum += value;
}

return { min, avg: sum / values.length, max };
};

const format = ( value ) => value.toFixed( 1 ).padStart( 5, ' ' );

const printStats = ( name, values, suffix = '' ) => {
const { min, avg, max } = stats( values );

console.log(
`${ name.padEnd( 28 ) } min ${ format( min ) }  avg ${ format( avg ) }  max ${ format( max ) }${ suffix }`
);
};

const section = ( title ) => {
console.log();
console.log( `=== ${ title } ===` );
};

const createEncryptTest = ( plaintext, key ) => ( input ) => encryptBlock_v1( plaintext ? input : input, key );

const testDeterminism = ( encrypt, name = 'Determinism' ) => {
let pass = true;

for ( let i = 0; i < SAMPLES; i++ ) {
const input = randomBytes();
const a = encrypt( input ), b = encrypt( input );

if ( ! equalBytes( a, b ) ) {
pass = false;
break;
}
}

console.log( `${ name.padEnd( 28 ) } ${ pass ? 'PASS' : 'FAIL' }` );
return pass;
};

const testOutputFormat = ( encrypt ) => {
let pass = true;

for ( let i = 0; i < SAMPLES; i++ ) {
const output = encrypt( randomBytes() );

if ( ! ( output instanceof Uint8Array ) || output.length !== BLOCK_SIZE ) {
pass = false;
break;
}
}

console.log( `Output format             ${ pass ? 'PASS' : 'FAIL' }` );
return pass;
};

const testCollisions = ( encrypt ) => {
const seen = new Set();
let collisions = 0;

for ( let i = 0; i < COLLISION_SAMPLES; i++ ) {
const output = hex( encrypt( randomBytes() ) );

if ( seen.has( output ) ) collisions++;
else seen.add( output );
}

console.log( `Collisions                ${ collisions === 0 ? 'PASS' : `FAIL (${ collisions })` }` );
return collisions === 0;
};

const testAvalanche = ( encrypt ) => {
const values = [];

for ( let i = 0; i < SAMPLES; i++ ) {
const input = randomBytes();
const changed = flipBit( input, Math.floor( Math.random() * BLOCK_SIZE * 8 ) );

const a = encrypt( input ), b = encrypt( changed );
values.push( hammingDistance( a, b ) );
}

printStats( 'Avalanche', values, '/256 bits' );
};

const testMultiBitAvalanche = ( encrypt ) => {
const counts = [ 1, 2, 4, 8, 16, 32, 64, 128, 256 ];

for ( const count of counts ) {
const values = [];

for ( let i = 0; i < SAMPLES; i++ ) {
const input = randomBytes(), changed = flipBits( input, count );
const a = encrypt( input ), b = encrypt( changed );

values.push( hammingDistance( a, b ) );
}

const { min, avg, max } = stats( values );

console.log(
`${ count.toString().padStart( 3 ) } bits changed`.padEnd( 28 ) +
`min ${ format( min ) }  avg ${ format( avg ) }  max ${ format( max ) }`
);
}
};

const testPositionAvalanche = ( encrypt ) => {
const values = [];

for ( let bit = 0; bit < BLOCK_SIZE * 8; bit++ ) {
const distances = [];

for ( let i = 0; i < POSITION_SAMPLES; i++ ) {
const input = randomBytes(), changed = flipBit( input, bit );
const a = encrypt( input ), b = encrypt( changed );

distances.push( hammingDistance( a, b ) );
}

values.push( { bit, ...stats( distances ) } );
}

const averages = values.map( value => value.avg );
const minimum = values.reduce( ( a, b ) => a.avg < b.avg ? a : b );
const maximum = values.reduce( ( a, b ) => a.avg > b.avg ? a : b );

console.log( `Average                 ${ stats( averages ).avg.toFixed( 1 ) }/256 bits` );
console.log( `Weakest bit             ${ minimum.bit.toString().padStart( 3 ) }  ${ minimum.avg.toFixed( 1 ) } bits` );
console.log( `Strongest bit           ${ maximum.bit.toString().padStart( 3 ) }  ${ maximum.avg.toFixed( 1 ) } bits` );
};

const testBitBalance = ( encrypt ) => {
const counts = new Uint32Array( BLOCK_SIZE * 8 );

for ( let i = 0; i < SAMPLES; i++ ) {
const output = encrypt( randomBytes() );

for ( let byte = 0; byte < output.length; byte++ ) for ( let bit = 0; bit < 8; bit++ )
if ( output[ byte ] & ( 1 << bit ) ) counts[ byte * 8 + bit ]++;
}

const percentages = Array.from( counts, count => count / SAMPLES * 100 );
printStats( 'Bit balance', percentages, '%' );
};

const testByteDistribution = ( encrypt ) => {
const counts = new Uint32Array( 256 );

for ( let i = 0; i < SAMPLES; i++ ) {
const output = encrypt( randomBytes() );

for ( const byte of output ) counts[ byte ]++;
}

const expected = SAMPLES * BLOCK_SIZE / 256;
const deviations = Array.from( counts, count => ( count - expected ) / expected * 100 );

printStats( 'Byte distribution', deviations, '%' );
};

const testHammingWeight = ( encrypt ) => {
const values = [];

for ( let i = 0; i < SAMPLES; i++ ) {
const output = encrypt( randomBytes() );
let bits = 0;

for ( const byte of output ) {
let value = byte;

while ( value ) {
bits += value & 1;
value >>>= 1;
}
}

values.push( bits );
}

const { avg } = stats( values );
const deviation = Math.sqrt( values.reduce( ( sum, value ) => sum + ( value - avg ) ** 2, 0 ) / values.length );

console.log( `Hamming weight           avg ${ avg.toFixed( 2 ) }  dev ${ deviation.toFixed( 2 ) }` );
};

const testStructuredInputs = ( encrypt ) => {
const inputs = [];

inputs.push( new Uint8Array( BLOCK_SIZE ) );
inputs.push( new Uint8Array( BLOCK_SIZE ).fill( 0xff ) );
inputs.push( Uint8Array.from( { length: BLOCK_SIZE }, ( _, index ) => index ) );
inputs.push( Uint8Array.from( { length: BLOCK_SIZE }, ( _, index ) => 0xff - index ) );
inputs.push( Uint8Array.from( { length: BLOCK_SIZE }, ( _, index ) => index % 2 ? 0xff : 0x00 ) );
inputs.push( Uint8Array.from( { length: BLOCK_SIZE }, ( _, index ) => index % 2 ? 0x00 : 0xff ) );
inputs.push( Uint8Array.from( { length: BLOCK_SIZE }, ( _, index ) => index === 0 ? 0x80 : 0x00 ) );
inputs.push( Uint8Array.from( { length: BLOCK_SIZE }, ( _, index ) => index === BLOCK_SIZE - 1 ? 0x80 : 0x00 ) );

for ( const input of inputs ) console.log(
`${ hex( input.subarray( 0, 8 ) ).padEnd( 19 ) } → ${ hex( encrypt( input ).subarray( 0, 8 ) ) }`
);
};

const testZeroPatterns = ( encrypt ) => {
const inputs = [
new Uint8Array( BLOCK_SIZE ),
new Uint8Array( BLOCK_SIZE ).fill( 0xff )
];

for ( const input of inputs ) {
const output = encrypt( input );
const unique = new Set( output ).size;

console.log(
`${ hex( input.subarray( 0, 4 ) ).padEnd( 12 ) } ` +
`${ unique }/${ BLOCK_SIZE } unique output bytes`
);
}
};

const testInputSensitivity = ( encrypt ) => {
const values = [];

for ( let i = 0; i < SAMPLES; i++ ) {
const input = randomBytes();
const changed = randomBytes();

const a = encrypt( input ), b = encrypt( changed );
values.push( hammingDistance( a, b ) );
}

printStats( 'Random input distance', values, '/256 bits' );
};

const testKeyAvalanche = ( encrypt ) => {
const values = [];

for ( let i = 0; i < SAMPLES; i++ ) {
const key = randomBytes();
const changed = flipBit( key, Math.floor( Math.random() * BLOCK_SIZE * 8 ) );

const a = encrypt( key ), b = encrypt( changed );
values.push( hammingDistance( a, b ) );
}

printStats( 'Key avalanche', values, '/256 bits' );
};

const testPlaintextAvalanche = ( encrypt ) => {
const values = [];

for ( let i = 0; i < SAMPLES; i++ ) {
const plaintext = randomBytes();
const changed = flipBit( plaintext, Math.floor( Math.random() * BLOCK_SIZE * 8 ) );

const a = encrypt( plaintext ), b = encrypt( changed );
values.push( hammingDistance( a, b ) );
}

printStats( 'Plaintext avalanche', values, '/256 bits' );
};

const testKeyPositionAvalanche = ( encrypt ) => {
const values = [];

for ( let bit = 0; bit < BLOCK_SIZE * 8; bit++ ) {
const distances = [];

for ( let i = 0; i < POSITION_SAMPLES; i++ ) {
const key = randomBytes(), changed = flipBit( key, bit );
const a = encrypt( key ), b = encrypt( changed );

distances.push( hammingDistance( a, b ) );
}

values.push( { bit, ...stats( distances ) } );
}

const averages = values.map( value => value.avg );
const minimum = values.reduce( ( a, b ) => a.avg < b.avg ? a : b );
const maximum = values.reduce( ( a, b ) => a.avg > b.avg ? a : b );

console.log( `Average                 ${ stats( averages ).avg.toFixed( 1 ) }/256 bits` );
console.log( `Weakest key bit         ${ minimum.bit.toString().padStart( 3 ) }  ${ minimum.avg.toFixed( 1 ) } bits` );
console.log( `Strongest key bit       ${ maximum.bit.toString().padStart( 3 ) }  ${ maximum.avg.toFixed( 1 ) } bits` );
};

const runInputTests = ( encrypt, title ) => {
section( title );

testOutputFormat( encrypt );
testDeterminism( encrypt );
testCollisions( encrypt );

console.log();
console.log( 'Avalanche' );
testAvalanche( encrypt );

console.log( 'Multi-bit avalanche' );
testMultiBitAvalanche( encrypt );

console.log( 'Position avalanche' );
testPositionAvalanche( encrypt );

console.log();
testInputSensitivity( encrypt );

console.log();
testBitBalance( encrypt );
testByteDistribution( encrypt );
testHammingWeight( encrypt );

console.log();
console.log( 'Structured inputs' );
testStructuredInputs( encrypt );
testZeroPatterns( encrypt );
};

const runTests = ( encrypt ) => {
console.log( '=== ENCRYPTION ANALYSIS ===' );
console.log( `Samples: ${ SAMPLES.toLocaleString() }` );
console.log( `Collision samples: ${ COLLISION_SAMPLES.toLocaleString() }` );
console.log( `Block size: ${ BLOCK_SIZE * 8 } bits` );

runInputTests( encrypt, 'PLAINTEXT ANALYSIS' );

section( 'KEY ANALYSIS' );

console.log( 'Key avalanche' );
testKeyAvalanche( encrypt );

console.log( 'Key position avalanche' );
testKeyPositionAvalanche( encrypt );

section( 'ANALYSIS COMPLETE' );
};

const plaintext = new Uint8Array( BLOCK_SIZE );
const key = new Uint8Array( BLOCK_SIZE );

plaintext[ 0 ] = 1;
key[ 0 ] = 1;

runInputTests( input => encryptBlock_v1( input, key ), 'PLAINTEXT ANALYSIS' );

section( 'KEY ANALYSIS' );

testKeyAvalanche( input => encryptBlock_v1( plaintext, input ) );
testKeyPositionAvalanche( input => encryptBlock_v1( plaintext, input ) );

section( 'ANALYSIS COMPLETE' );
