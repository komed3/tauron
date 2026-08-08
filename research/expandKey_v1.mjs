export const expandKey_v1 = ( key ) => {
  const substitute = value => ( value * 197 + 23 ) & 0xff;

  const words = new Uint32Array( 8 );
  for ( let i = 0; i < 8; i++ ) {
    words[ i ] = key[ i * 4 ]
      | ( key[ i * 4 + 1 ] << 8 )
      | ( key[ i * 4 + 2 ] << 16 )
      | ( key[ i * 4 + 3 ] << 24 );
  }
};