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

  for ( let i = 0; i < 8; i++ ) {
    const a = words[ i ];
    const b = words[ ( i + 3 ) & 7 ];

    words[ i ] = ( a ^ rotl( b, ( i * 7 + 3 ) % 32 ) ) >>> 0;
  }

  const permutation = [
    0, 13, 26, 7, 20, 1, 14, 27,
    8, 21, 2, 15, 28, 9, 22, 3,
    16, 29, 10, 23, 4, 17, 30, 11,
    24, 5, 18, 31, 12, 19, 6, 25
  ];

  const result = new Uint8Array( KEY_SIZE );
  for ( let i = 0; i < KEY_SIZE; i++ ) result[ i ] = bytes[ permutation[ i ] ];
};