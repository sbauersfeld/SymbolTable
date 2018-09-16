# SymbolTable
An efficient symbol table lookup that maps variable identifiers to scope levels and line numbers. The goal of this project is to keep track of variables declared within various scope levels in the most efficient manner as possible. My solution uses open hash tables and aims to always insert and remove from the beginning of lists when possible.

The following is a more detailed explanation of my implementation of the symbol table.

My implementation of the Symbol Table class uses a hash table with pointers to a list structure. The list structure is a linked list with next pointers, a string data member, and two integer data members. The string data member stores the identifier for each declared variable and the integers contain the scope level and line number for the identifier. There is also a vector of integers that stores the bucket number that every identifier is inserted into. An integer is used to keep track of the current scope level. 
Every time a new scope is entered, the vector pushes back a -1 to mark the boundary between different scope levels. When a variable is declared, it is assigned a bucket number and is added to the front of the linked list at the hash tables corresponding bucket. Its bucket number is then added to the vector of bin numbers. When a scope is exited, every bin number between the last vector index and the closest -1 is used to remove the first node in that corresponding bucket’s linked list. In order to find a particular id, the corresponding bucket to that id is used to search for the first node in that bucket’s linked list until an identical id is found.
Enter scope is constant time. Exit scope is linear in the number of identifiers going out of scope. Declare is linear in the number of characters in the string. Insert is linear in the load factor of the hash table. Find is linear in the load factor of the hash table. RemoveId is constant time.

Pseudocode for ExitScope:
If this function has been called more than enter scope
	Return false
For every number declared in the last scope
	Remove the node at the front of that bin number’s list
Decrement the current scope

Pseudocode for declare:
If the identifier is invalid
	Return false
Assign a bin number based on the characters in the id
If the id as not been declared in the current scope
	Insert the id at the front of the hash tables corresponding bucket
	Push the bucket number onto the current vector
Otherwise return false

Pseudocode for find:
If the identifier is invalid
	Return false
Find the bin number corresponding to the characters in the id
Search through that bucket until we find the id
If it is not found
	Return false
Otherwise
	Return the line number corresponding to that id
