#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CRC_LEN 16
#define generator 1417


//PRINTING/FORMATTING FUNCTIONS
void pr(char* t, size_t len){ //print string with binary representation
  size_t bufSize = CHAR_BIT+1;
  char spacing[bufSize];
  memset(spacing, ' ', bufSize);
  spacing[bufSize-1] = '\0'; //padding buffer init. 9 spaces, 8 for the bits + 1 for clarity

  for (int i = 0; i < len; i++){
    printf("%c", (t[i] == 0) ? '0' : t[i]); 
    printf("%s", spacing);
  }
  printf("\n");
  
  for (int i = 0; i < len; i++){ //print number in binary
    for (int j = bufSize - 2; j >= 0; j--){ //step through each bit in a bite (-2 for null index and spacing) and compare to mask
      printf("%c", (t[i] & (1 << j)) ? '1' : '0');
    }
    printf(" ");
  }
}

void printCharAsBinary(unsigned long c, size_t size){ 
  for (int j = size-1; j >= 0; j--){    
    printf("%c", (c & (1UL << j)) ? '1' : '0');
    if(j % 8 == 0)
      printf(" ");
  }
  printf("\n");
}

//--------------------------



unsigned long* generateCRC(char* t){
  unsigned long msb_mask = 1 << CHAR_BIT-1;
  unsigned long shift_reg_mask = 1UL << (CRC_LEN < sizeof(unsigned long) * CHAR_BIT ? CRC_LEN : CRC_LEN-1); //last bit in the byte
  unsigned long* sRegister = malloc((CRC_LEN/CHAR_BIT) + 1);
  unsigned long chars = strlen(t) + ((CRC_LEN)/8); //length of the message + CRC length
  
  //SHIFTING INTO REGISTER
  for (int i = 0; i < chars; i++){
    for (int j = CHAR_BIT-1; j >= 0; j--){
      *sRegister <<= 1; //shift register
      unsigned long MSB = (t[i] & msb_mask ? 1 : 0); //get next bit in stream
      *sRegister |= MSB; //add next bit to register
      t[i] <<= 1; //shift stream to next bit
      unsigned long a = (shift_reg_mask & *sRegister) >> CRC_LEN;  // wether or not MSB is 1
      *sRegister = *sRegister ^ (a ? generator : 0); //xor with the generator if MSB is 1, otherwise xor with 0 (do nothing)
    }
  }
  return sRegister;  
}

int main(int argc, char* argv[]){

  char t[3];
  t[0] = 0x41;
  t[1] = 0x41;
  t[2] = 0;
  t[3] = 0; //MAKE SURE YOU HAVE AS MANY NULL TERMINATORS AS YOU HAVE BYTES IN THE CRC

  printf("generator:");
  printCharAsBinary(generator, CRC_LEN+1);
  
  pr(t, sizeof(t) - 1); //WARNING YOU SHOULD NOT SUBTRACT FROM A SIZE_T UNLESS U WANT SOMETHING TO BREAK LATER
  unsigned long* crc = generateCRC(t);
  printCharAsBinary(*crc - (1 << 8), CHAR_BIT);

  *crc &= ULONG_MAX ^ (ULONG_MAX << CRC_LEN); //mask away any unused bits that are still technically part of the int (bc we're only using a small part of the int)
  
  printf("generator: %lu\n", generator);
  printf("result: %lu\n", *crc);
  printf("result in bin: ");
  printCharAsBinary(*crc, CRC_LEN);
  
  free(crc);
  return 0;
}
