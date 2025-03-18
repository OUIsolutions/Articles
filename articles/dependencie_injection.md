
### Dependencie Injection
A guide to make libs that runs in every os that existed and will exist.

## Introduction
When you a doing a library an comon problem, its to give suport to many OS,because 
each OS has its own way to do things, and the way to do things in one OS can be
completly different in another OS. So, how can we make a library that runs in every OS?
The Key its simple: you dont !!,you pass the responsability to the user of the library.
This is the idea of Dependencie Injection, you make the user of the library pass the
dependencie that the library needs to run. This way, the user can pass the dependencie
that he wants, and the library will run in every OS that the user wants.


## The Macro Way 
I personaly dont like this way, but its the most common way to do it. The idea is to
make the user pass the dependencie in the compile time, with macros.
#### Project:
```txt
project/
├── main.c
└── my_lib.h
```

#### project/main.c
```c
#include <stdio.h>
#define my_lib_printf printf
#include "my_lib.h"

int main(){
    my_lib_func();
}
```
#### project/my_lib.h
```c
#ifndef my_lib_printf 
#error "my_lib_printf is not defined"
#endif

void my_lib_func(){
    my_lib_printf("Hello World\n");
}
```

#### Pros:
- Its simple to do
- it dont requires global variables
#### Cons:
- for beeing Compile time, you cant change the dependencie in the runtime
- you cannot make a low granularity dependencie injection
- you need to pass the dependencie in the compile time, so you need to recompile the code every time you change the dependencie


## The Global Lambda way
This way is a little more complex, but its more flexible than the macro way. The idea is to make the user pass the dependencie in the runtime, using a global variable that is a lambda function.


#### Project:
```txt
project/
├── main.c
├── my_lib.c
└── my_lib.h
```

#### project/main.c
```c

#include "my_lib.h"
#include <stdio.h>
#include <stdlib.h>

int main(){


    my_lib_printf = printf;
    my_lib_malloc = malloc;

    const char *import_error = check_lambdas();
    if(import_error){
        printf("Error importing %s\n", import_error);
        return 1;
    }

    my_lib_func();
}
```


#### project/my_lib.h
```c

#ifndef MY_LIB_H
#define MY_LIB_H

extern int (*my_lib_printf)(const char *format, ...);
extern void *(*my_lib_malloc)(unsigned long int size);


const char * check_lambdas();


void my_lib_func();
#endif
```

#### project/my_lib.c
```c


#include "my_lib.h"

int (*my_lib_printf)(const char *format, ...);
void *(*my_lib_malloc)(unsigned long int size);


const char * check_lambdas(){
    
    if(!my_lib_printf){
        return "my_lib_printf";
    }
    if(!my_lib_printf){
        return "my_lib_malloc";
    }
    return 0;
}

void my_lib_func(){
    my_lib_printf("Hello World\n");
}

```
#### Pros:
- You can change the dependencie in the runtime
- you can make a low granularity dependencie injection
- you dont need to recompile the code every time you change the dependencie
- you can test behaviors of the library with different dependecies

#### Cons:
- you need to pass the dependencie in the runtime
- you need to check if the dependencie is setted before using it


## The Global Lambda Struct Way
its very similar to the Global Lambda way, but instead of using global variables, you use a struct to store the lambdas.
it gives the advantage that does its more easy to configure.

