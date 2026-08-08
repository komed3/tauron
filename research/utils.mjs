export const KEY_SIZE = 32;
export const ROUNDS = 16;

export const hex = ( bytes ) => {
  return Array.from( bytes, byte => byte.toString( 16 ).padStart( 2, '0' ) ).join( ' ' );
};

export const hammingDistance = ( a, b ) => {
  let bits = 0;

  for ( let i = 0; i < a.length; i++ ) {
    let value = a[ i ] ^ b[ i ];

    while ( value ) {
      bits += value & 1;
      value >>>= 1;
    }
  }

  return bits;
};

const randomKey = () => {
  const key = new Uint8Array( KEY_SIZE );
  webcrypto.getRandomValues( key );

  return key;
};

const cloneKey = ( key ) => new Uint8Array( key );

export const schedule = ( key, fn ) => {
  const keys = [ new Uint8Array( key ) ];
  let current = new Uint8Array( key );

  for ( let round = 1; round <= ROUNDS; round++ ) {
    current = fn( current, round );
    keys.push( current );
  }

  return keys;
};

export const outKeys = ( keys ) => {
  keys.forEach( ( key, round ) => console.log(
    `Round ${ round.toString().padStart( 2, '0' ) }: ${ hex( key ) }`
  ) );
};

export const outDiffer = ( keysA, keysB ) => {
  for ( let round = 0; round <= ROUNDS; round++ ) {
    console.log( `Round ${ round.toString().padStart( 2, '0' ) }: ${
      hammingDistance( keysA[ round ], keysB[ round ] ).toString().padStart( 2, ' ' )
    }/256 bits differ` );
  }
};
