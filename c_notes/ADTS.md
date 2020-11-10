## ADTS 

some coding like linked lists are really error prone so we would like to avoid having to re do it all the time

therefore we generally use library code 

build something once and test it well

One example is an Abstract data type

each ADT exposes its functionality via an inter face

one of the most basic ADTs is a collection

## Collections

ususally exposed to the user via a .h file



this is just a simple place to search for/add/ delete data elements

some collections allow duplicate elements and others dont (like sets)

some are ordered and some arent

```c
/* Create an empty coll */
coll* coll_init(void);
/* Add element onto top */
void coll_add(coll* c, datatype i); /* Take element out */
bool coll_remove(coll* c, datatype d); /* Does this exist ? */
bool coll_isin(coll* c, datatype i); /* Return size of coll */
int coll_size(coll* c);
/* Clears all space used */ bool coll_free(coll* c);

```

the interface gives you no hints to the underlying implementation of the ADT- you dont know if they've used a array or linked list etc.

A user of the ADT doesnt really need to know how its implemented

