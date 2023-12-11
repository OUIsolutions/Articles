# The Post Modern C Style

After creating some libraries, including web servers and IO libraries, I received numerous criticisms from both the C community, for having too much of a "C++" style of programming, and from the C++ community for being too C.

And I realized that there is already a group of programmers who follow this exotic style of programming, similar to orthodox C++
https://gist.github.com/bkaradzic/2e39896bc7d8c34e042b

I decided to create: Post Modern C Style.
A style of programming in C, which takes the language's functionality to the maximum, while not falling into the absolute confusion of C++.

## Structs:

Structures must simulate classes, implementing constructors, getters, setters, and destructors.
Following a pattern of all functions having their name, followed by their function.
And always add the structure name as self (similar to python)
