#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define CRC_LEN 8
#define GENERATOR 0x31
#define INIT_VAL 0x00
#define XOR_OUT 0x00
#define REF 1



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
    printf("\n");
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

/*
  reflectInt8 takes an 8-bit int (char) and reflects it
  
 */
unsigned int reflectInt8(unsigned char d){
  unsigned char r = 0;
  unsigned char MSB_MASK = 1U << sizeof(unsigned char) * CHAR_BIT - 1;    
  for (int i = 0; i < sizeof(unsigned char) * CHAR_BIT; i++){
      r = MSB_MASK&d ? (r >> 1) ^ MSB_MASK : r >> 1;
      d <<= 1;
  } 
  return r;
}


unsigned long* generateCRC(char* t){
  if(REF){ //REFLECT THE INPUT BITS
    char l[strlen(t)];
    for (int i = 0; i < strlen(t); i++){
      l[strlen(t)-i-1] = reflectInt8(t[i]);
    }
    memcpy(t, l, strlen(t));
  };
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
      if(CHAR_BIT-j + (i * CHAR_BIT) == CRC_LEN) //init before first XOR operation
	*sRegister ^= INIT_VAL;
      unsigned long a = (shift_reg_mask & *sRegister) >> CRC_LEN;  // wether or not MSB is 1
      *sRegister = *sRegister ^ (a ? GENERATOR : 0); //xor with the GENERATOR if MSB is 1, otherwise xor with 0 (do nothing)

    }
  }
  *sRegister = *sRegister ^ XOR_OUT; //xor out
  return sRegister;  
}

int main(int argc, char* argv[]){

  char t[3];
  memset(t, 0, sizeof(t));
  t[0] = 0x41;
  t[1]  = 0x42;

  //printCharAsBinary(reflectInt8(0x41), sizeof(unsigned char) * 8);
       
  pr(t, sizeof(t)); //WARNING YOU SHOULD NOT SUBTRACT FROM A SIZE_T UNLESS U WANT SOMETHING TO BREAK LATER
  unsigned long* crc = generateCRC(t);

  *crc &= ULONG_MAX ^ (ULONG_MAX << CRC_LEN); //mask away any unused bits that are still technically part of the int (bc we're only using a small part of the int)
  printf("\ngenerator: 0x%lx\n", GENERATOR);
  printf("generator in bin: ");
  printCharAsBinary(GENERATOR, CRC_LEN);  
    
  printf("\nresult: 0x%lx\n", *crc);
  printf("result in bin: ");
  printCharAsBinary(*crc, CRC_LEN);
  
  free(crc);
  return 0;
}
