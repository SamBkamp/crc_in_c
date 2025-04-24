#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define INT_SIZE 8
#define SHIFT_REG_LEN 9
#define generator 263

//PRINTING/FORMATTING FUNCTIONS
void pr(char* t, size_t len){ //print string with binary representation
  //size_t bufSize = sizeof(int)*8;
  size_t bufSize = INT_SIZE+1;
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
  int msb_mask = 1 << INT_SIZE-1;
  int shift_reg_mask = 1 << SHIFT_REG_LEN-1;
  int* sRegister = malloc(SHIFT_REG_LEN);
  int chars = strlen(t) + (SHIFT_REG_LEN - 1)/8; //length of the message + CRC length
  
  //SHIFTING INTO REGISTER
  for (int i = 0; i < chars; i++){
    for (int j = INT_SIZE-1; j >= 0; j--){
      *sRegister <<= 1; //shift register
      int MSB = (t[i] & msb_mask ? 1 : 0); //get next bit in stream
      *sRegister |= MSB; //add next bit to register
      t[i] <<= 1; //shift stream to next bit
      int a = (shift_reg_mask & *sRegister) >> SHIFT_REG_LEN - 1;  // wether or not MSB is 1
      *sRegister = *sRegister ^ (a ? generator : 0); //xor with the generator if MSB is 1, otherwise xor with 0 (do nothing)
    }
  }
  return sRegister;  
}

int main(int argc, char* argv[]){

  char t[2] = "W";
  
  pr(t, sizeof(t) - 1); //WARNING YOU SHOULD NOT SUBTRACT FROM A SIZE_T UNLESS U WANT SOMETHING TO BREAK LATER
  int* crc = generateCRC(t);
  printCharAsBinary(*crc, SHIFT_REG_LEN-1);
  free(crc);
  return 0;
}
