import { ROUNDS } from './utils.mjs';

export const rotateByte = ( value, bits ) => {
  return ( ( value << bits ) | ( value >>> ( 8 - bits ) ) ) & 0xff;
}

export const transform = ( key, round ) => {
  const result = new Uint8Array( key.length );

  for ( let i = 0; i < key.length; i++ ) {
    const next = key[ ( i + 1 ) % key.length ];
    const previous = key[ ( i + key.length - 1 ) % key.length ];

    result[ i ] = rotateByte( key[ i ], ( i + round ) % 8 || 1 ) ^
      next ^ previous ^ ( ( round * 17 + i * 31 ) & 0xff );
  }

  return result;
}

export const expandKey = ( masterKey ) => {
  const keys = [ new Uint8Array( masterKey ) ];
  let current = new Uint8Array( masterKey );

  for ( let round = 1; round <= ROUNDS; round++ ) {
    current = transform( current, round );
    keys.push( current );
  }

  return keys;
}
