#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define BITS_PER_BYTE 8
#define CRC_LEN 16
#define generator 35767


//PRINTING/FORMATTING FUNCTIONS
void pr(char* t, size_t len){ //print string with binary representation
  size_t bufSize = BITS_PER_BYTE+1;
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

void printCharAsBinary(int c, size_t size){ 
  for (int j = size-1; j >= 0; j--){    
    printf("%c", (c & (1 << j)) ? '1' : '0');
    if(j % 8 == 0)
      printf(" ");
  }
  printf("\n");
}

//--------------------------



int* generateCRC(char* t){
  int msb_mask = 1 << BITS_PER_BYTE-1;
  int shift_reg_mask = 1 << CRC_LEN;
  int* sRegister = malloc(CRC_LEN + 1);
  int chars = strlen(t) + ((CRC_LEN)/8); //length of the message + CRC length
  
  //SHIFTING INTO REGISTER
  for (int i = 0; i < chars; i++){
    for (int j = BITS_PER_BYTE-1; j >= 0; j--){
      *sRegister <<= 1; //shift register
      int MSB = (t[i] & msb_mask ? 1 : 0); //get next bit in stream
      *sRegister |= MSB; //add next bit to register
      t[i] <<= 1; //shift stream to next bit
      int a = (shift_reg_mask & *sRegister) >> CRC_LEN;  // wether or not MSB is 1
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

  printf("generator:");
  printCharAsBinary(generator, CRC_LEN+1);
  
  pr(t, sizeof(t) - 1); //WARNING YOU SHOULD NOT SUBTRACT FROM A SIZE_T UNLESS U WANT SOMETHING TO BREAK LATER
  int* crc = generateCRC(t);
  printCharAsBinary(*crc - (1 << 8), BITS_PER_BYTE);

  int test = 141;

  *crc &= UINT_MAX >> (UINT_MAX - (CRC_LEN-1)); //mask away any unused bits that are still technically part of the int (bc we're only using a small part of the int)
  
  printf("generator: %d\n", generator);
  printf("result: %d\n", *crc);
  printf("result in bin: ");
  printCharAsBinary(*crc, sizeof(int) * 8);
  
  free(crc);
  return 0;
}
