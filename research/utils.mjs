export const KEY_SIZE = 32;
export const ROUNDS = 16;

export const hex = ( bytes ) => {
  return Array.from( bytes, byte => byte.toString( 16 ).padStart( 2, '0' ) ).join( ' ' );
}
