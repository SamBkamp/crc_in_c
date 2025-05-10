# CRC in C
a basic cyclic redundancy check in C

I drew inspiration and **a lot** of guidance from Ben Eaters [video](https://www.youtube.com/watch?v=izG7qT0EpBw) on the topic and it informed my implementation greatly.

## How does it work?
I used a shift register approach to solve this problem. I created a "shift register" with `malloc` of a certain size (depending on how long you want your CRC to be) and shifted in values to be computed bit-by-bit (literally). I used masks to make that work in C.
"W" in binary (8 bits):
```
01010111
```
this would then be and'd with an (in this case) 8 bit mask:
```js
W:         01010111
mask:      10000000
W & mask = 00000000
```
This created a second mask that was used to shift in the next value to the register
```js
//if W & mask = 0 
reg:         xxxxxxxx
mask:        00000000
reg | mask = xxxxxxx0

//if W & mask > 0 
reg:         xxxxxxxx
mask:        00000001
reg | mask = xxxxxxx1

```
after this, the register would be xor'd with either ``generator`` or ``0`` depending if the most significant bit of ``reg`` was 1 or 0 respectively.

## How do I use this program?
edit the 3 definitions to your liking:
```c
#define CRC_LEN 8
#define GENERATOR 0x31
#define INIT_VAL 0x00
#define XOR_OUT 0x00
#define REF 1
```
`CRC_LEN` = the length of your crc, in bits

`GENERATOR` = the generator polynomial (expressed as an integer in c, so decimal or hex)

`INIT_VAL` = the initial value (0 if your version doesn't use one)

`XOR_OUT` = the value of your xor out (also 0 if your version of crc doesn't use one)

`REF` = 1 if your version uses reflected crc, 0 if it doesn't *(this is not fully implemented yet, REF = 1 will give you wrong results)*

finally, change your message in the main function 
```c
char t [2] = "W";
```

## Compilation?
its a 60 line c file just throw it into gcc 


### THIS PROGRAM DOES NOT *YET* FULLY IMPLEMENT *REFLECTED* CRC, normal CRC works fine