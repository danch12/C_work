## Strings

With putting strings in arrays remember to add end char manually!!!

Also means we can just create a big array and then use the end char to symbolise end of array rather than dynamically create.

Also when doing stuff like copying stuff over and we dont want one of them to change we can just set the argument to a constant.

Also in a function instead of dynamically allocating the array you could also make the local variable static to make it last past the function call.



scanf(“%s”) stops when there is any whitespace - this means it basically just scans in a word. 



Using a pointer to a string makes it a constant and therefore cannot be changed unlike using an array. 

*p++ means access contents of what p is pointing to then more p forward by one