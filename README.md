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
#define INT_SIZE 8
#define SHIFT_REG_LEN 9
#define generator 263
```
`INT_SIZE` = the size of your integers in bits

`SHIFT_REG_LEN` = the size of your shift register (which is the size of your CRC + 1)

`generator` = the generator polynomial (expressed as an integer in c, so decimal or hex)

finally, change your message in the main function 
```c
char t [2] = "W";
```

## Compilation?
its a 60 line c file just throw it into gcc 


# THIS PROGRAM DOES NOT **YET** FULLY IMPLEMENT REFLECTED CRC