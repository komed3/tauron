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
}
