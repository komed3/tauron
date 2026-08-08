export const expandKey_v1 = ( key ) => {
  const substitute = value => ( value * 197 + 23 ) & 0xff;

  const words = new Uint32Array( 8 );
  for ( let i = 0; i < 8; i++ ) {
    words[ i ] = key[ i * 4 ]
      | ( key[ i * 4 + 1 ] << 8 )
      | ( key[ i * 4 + 2 ] << 16 )
      | ( key[ i * 4 + 3 ] << 24 );
  }

  const rotl = ( value, bits ) => ( value << bits ) | ( value >>> ( 32 - bits ) );
  for ( let i = 0; i < 8; i += 2 ) {
    words[ i ] = ( words[ i ] + rotl( words[ i + 1 ], 5 ) ) >>> 0;
    words[ i + 1 ] ^= rotl( words[ i ], 13 );
  }
};