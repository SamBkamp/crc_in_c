#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CRC_LEN 16
#define GENERATOR 0x1021
#define INIT_VAL 0xFFFF
#define XOR_OUT 0xFFFF



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
    if(j % CHAR_BIT == 0)
      printf(" ");
  }
  printf("\n");
}

//--------------------------



unsigned long* generateCRC(char* t){
  unsigned long msb_mask = 1 << CHAR_BIT-1;
  unsigned long shift_reg_mask = 1UL << CRC_LEN;
  unsigned long* sRegister = malloc((CRC_LEN/CHAR_BIT) + 1);
  unsigned long chars = strlen(t) + ((CRC_LEN)/CHAR_BIT); //length of the message + CRC length
  
  //SHIFTING INTO REGISTER
  for (int i = 0; i < chars; i++){
    for (int j = CHAR_BIT-1; j >= 0; j--){
      //NTH ITERATION: CHAR_BIT-j + (i * CHAR_BIT)
      *sRegister <<= 1; //shift register
      unsigned long MSB = (t[i] & msb_mask ? 1 : 0); //get next bit in stream
      *sRegister |= MSB; //add next bit to register
      t[i] <<= 1; //shift stream to next bit
      if(CHAR_BIT-j + (i * CHAR_BIT) == CRC_LEN)
	*sRegister ^= INIT_VAL;
      unsigned long a = (shift_reg_mask & *sRegister) >> CRC_LEN;  // wether or not MSB is 1
      *sRegister = *sRegister ^ (a ? GENERATOR : 0); //xor with the GENERATOR if MSB is 1, otherwise xor with 0 (do nothing)
    }
  }
  *sRegister = *sRegister ^ XOR_OUT; //xor out
  return sRegister;  
}

int main(int argc, char* argv[]){

  char t[4];
  t[0] = 0x41;
  t[1] = 0x42;
  t[2] = 0;
  t[3] = 0; //MAKE SURE YOU HAVE AT LEAST AS MANY NULL TERMINATORS AS YOU HAVE BYTES IN THE CRC
    
  pr(t, sizeof(t) - 1); //WARNING YOU SHOULD NOT SUBTRACT FROM A SIZE_T UNLESS U WANT SOMETHING TO BREAK LATER
  unsigned long* crc = generateCRC(t);

  *crc &= ULONG_MAX ^ (ULONG_MAX << CRC_LEN); //mask away any unused bits that are still technically part of the int (bc we're only using a small part of the int)
  printf("\ngenerator: %lx\n", GENERATOR);
  printf("generator in bin: ");
  printCharAsBinary(GENERATOR, CRC_LEN);  
  
  
  printf("\nresult: %lx\n", *crc);
  printf("result in bin: ");
  printCharAsBinary(*crc, CRC_LEN);
  
  free(crc);
  return 0;
}
