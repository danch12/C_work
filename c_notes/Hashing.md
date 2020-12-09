## Hashing

 a hash function is a mapping that maps from key k onto the index of an entry

as an example lets use an array of size 11 to store some airport codes eg PHL, DCA, FRA

in a three letter string $X_2$, $X_1$ , $X_0$ the letter A has the value 0 , B has the value 1 etc

as there are 26 letters we can use base 26 so that each letter in the airport code has a different value

an example hash function-

$$h(K)= (X_2*26^2 +X_1 *26 +X_0) \% 11 $$

we take modulo 11 because our array is 11 long

applying this to DCA 

$$h(DCA)= (3*26^2 +2 *26 +0) \% 11 $$

$$= (2080)\%11$$

=1



the hash function would have to know the string and the size of array

## collisions

an ideal hashing function maps keys into the array in a uniform and random manner.

collisions are common - think about how when there are 23 people in a room 50% chance of someone having the same birthday

same applies for this



## Linear Probing

policy of finding another free location if a collision occurs is called open addressing

if a collision occurs then keep stepping backwards until a free location is encountered

the simplest method of open addressing is linear probing 

the step taken each time doesnt have to be 1

open addressing through use of linear probing is a very simple technique double hasing is generally much more successful

## Double hasing

instead of having a linear probe for dealing with collisions how about when we collide we then use another hash function to decide how big our steps back through the array are going to be.

the function is chosen so that the two keys which collide at the same address will have different probe decrements

![Screenshot 2020-12-01 at 11.34.16](/Users/danielchow/Desktop/Screenshot 2020-12-01 at 11.34.16.png)

importantly we need to choose a size of array that is prime as this for some maths reason means that our probe will check each cell of our array to find space

 or we can use seperate chaining

## seperate chaining

instead of finding a new space of the new value we can instead create a linked list at that point of the array for each of the values that land at that point.

this has O(n) time complexity but will often be faster





## sample hash function

```c
int hash(unsigned int sz, char *s)
{
  unsigned long hash = 5381;
  int c;
  while(c=(*s++))
  {
    hash=33*hash ^c;
  }
  return (int) (hash%sz);
}

```



## Cukoo hashing

use two tables

with two hash functions-

one used only when dealing with table 0 and a different hash function for table 1 going back to table 0

so a word comes in and we use hash function 1 for table 0 then we use it again to put it in table 1- then if needed we can use hash function 2 to put it back in table 0 ??

or potentially we use hash function 1 for table 0 and hash function 2 for table 1 

arrive at first table and then insert value, but if the position in the first table is full we remove the word at that position and use the second hash function to put it into table 1 - so to be clear we remove the original word and put it in the second table.

When table gets too full we resize - we have to be really careful when changing the size of the array as this will change our hashing function (our modulo will be different). So we have to take them all out and rehash all the values

sometimes when we start bouncing words between the tables it will just bounce in some form of cycle so we have to keep track of how many times we have bounced the same words out- and if this gets too large then we will need to change our hash function and rehash everything or will need to resize the array.