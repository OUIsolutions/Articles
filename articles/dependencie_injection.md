
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
make the user pass the dependencie in the compile time, with macros.In These Case we 
pass a file to be imported in the compile time, and the user needs to define the macros
that the library uses. (But you can find diferent ways to do it)

#### Project:
```txt
project/
├── deps.h
├── main.c
├── my_lib.c
└── my_lib.h
```

#### project/main.c
```c
#include <stdio.h>
#include "my_lib.h"

int main(){
    my_lib_func();
}
```

#### project/my_lib.h
```c


#ifndef MY_LIB_H
#define MY_LIB_H


void my_lib_func();

#endif
```

#### project/my_lib.c
```c


#include "my_lib.h"
#include  MY_LIB_DEPS


#ifndef my_lib_printf 
#error "my_lib_printf is not defined"
#endif

void my_lib_func(){
    my_lib_printf("Hello World\n");
}

```

#### project/deps.h
```c
#include <stdio.h>
#define my_lib_printf printf
```
and you will need to compile the code in these way:
```sh
 gcc project/main.c  project/my_lib.c -DMY_LIB_DEPS='"deps.h"'
```


#### Pros:
- Its simple to do
- it dont requires global variables
#### Cons:
- for beeing Compile time, you cant change the dependencie in the runtime
- you cannot make a low granularity dependencie injection
- you need to pass the dependencie in the compile time, so you need to recompile the code every time you change the dependencie


## Run Time Ways to do Dependencie Injection
There are many ways to do dependencie injection in the runtime, but the most common ways are:

#### Project:
For making all the above tecniques more easy to understand, we will make a simple project that
has this following structure:
```txt
project/
├── main.c
├── my_lib.c
└── my_lib.h
```

## The Global Lambda way
This way is a little more complex, but its more flexible than the macro way. The idea is to make the user pass the dependencie in the runtime, using a global variable that is a lambda function.



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


## The Global Struct Way
its very similar to the Global Lambda way, but instead of using global variables, you use a struct to store the lambdas.
it gives the advantage that does its more easy to configure.

#### project/main.c
```c
#include "my_lib.h"
#include <stdio.h>
#include <stdlib.h>
int main(){


    my_lib_deps.my_lib_printf = printf;
    my_lib_deps.my_lib_malloc = malloc;

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

typedef struct MyLibDeps{
    int (*my_lib_printf)(const char *format, ...);
    void *(*my_lib_malloc)(unsigned long int size);
}MyLibDeps;

extern MyLibDeps my_lib_deps;



const char * check_lambdas();


void my_lib_func();
#endif
```

#### project/my_lib.c
```c

#include "my_lib.h"

MyLibDeps my_lib_deps;

const char * check_lambdas(){
    
    if(!my_lib_deps.my_lib_printf){
        return "my_lib_printf";
    }
    if(!my_lib_deps.my_lib_printf){
        return "my_lib_malloc";
    }
    return 0;
}

void my_lib_func(){
    my_lib_deps.my_lib_printf("Hello World\n");
}

```

#### Pros:
- **all the pros of the Global Lambda way**
- lambda declaration is placed in a unique place (struct declaration)

#### Cons:
- **all the cons of the Global Lambda way**

### The Interface Way
This way is very similar to the Global Struct way, but instead of using a struct, you use a interface to store the lambdas.
it gives the advantage that does its more easy to configure, but every  function that uses the dependencie needs to receive the interface as a parameter.

#### project/main.c
```c
#include "my_lib.h"
#include <stdio.h>
#include <stdlib.h>
int main(){
    
    MyLibDeps my_lib_deps = {
        .my_lib_printf = printf,
        .my_lib_malloc = malloc
    };

    const char *import_error = check_lambdas(&my_lib_deps);
    if(import_error){
        printf("Error importing %s\n", import_error);
        return 1;
    }

    my_lib_func(&my_lib_deps);
}
```

#### project/my_lib.h
```c


#ifndef MY_LIB_H
#define MY_LIB_H

typedef struct MyLibDeps{
    int (*my_lib_printf)(const char *format, ...);
    void *(*my_lib_malloc)(unsigned long int size);
}MyLibDeps;


const char * check_lambdas(MyLibDeps *deps);


void my_lib_func(MyLibDeps *deps);
#endif
```

#### project/my_lib.c
```c

#include "my_lib.h"

const char * check_lambdas(MyLibDeps *deps){
    
    if(!deps->my_lib_printf){
        return "my_lib_printf";
    }
    if(!deps->my_lib_printf){
        return "my_lib_malloc";
    }
    return 0;
}

void my_lib_func(MyLibDeps *deps){
    deps->my_lib_printf("Hello World\n");
}

```

#### Pros:
- **all the pros of the Global Struct way**
- its even more easy to configure the dependencie
- its even more granular than the Global Struct way

#### Cons:
- **all the cons of the Global Struct way**
- every function that uses the dependencie needs to receive the interface as a parameter


### The Interface Way Combined with the Global Struct Way
This way is a combination of the Interface Way and the Global Struct Way, you use a global struct to store the lambdas, but every function that uses the dependencie needs to receive the interface as a parameter,and you create a function wrapper that receives the interface as a parameter and calls the function with the global struct.
These way its granular and easy to configure, but will increase the size of the library.

#### project/main.c
```c
#include "my_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int print_green(const char *format, ...){
    printf("\033[0;32m");
    va_list args;
    va_start(args, format);
    int ret = vprintf(format, args);
    va_end(args);
    printf("\033[0m");
    return ret;
}

int main(){
    //Defaut test
    my_lib_deps_default.my_lib_printf = printf;
    my_lib_deps_default.my_lib_malloc = malloc;

    const char *import_error = check_lambdas();
    if(import_error){
        printf("Error importing %s\n", import_error);
        return 1;
    }

    //it will call the default printf
    my_lib_func();

    MyLibDeps my_lib_deps_custom;
    my_lib_deps_custom.my_lib_printf = print_green;
    my_lib_deps_custom.my_lib_malloc = malloc;

    import_error = check_lambdas_with_deps(&my_lib_deps_custom);
    if(import_error){
        printf("Error importing %s\n", import_error);
        return 1;
    }

    //it will call the custom print_green
    my_lib_func_with_deps(&my_lib_deps_custom);

}
```

#### project/my_lib.h
```c



#ifndef MY_LIB_H
#define MY_LIB_H

typedef struct MyLibDeps{
    int (*my_lib_printf)(const char *format, ...);
    void *(*my_lib_malloc)(unsigned long int size);
}MyLibDeps;

extern MyLibDeps my_lib_deps_default;

const char * check_lambdas_with_deps(MyLibDeps *deps);

const char * check_lambdas();

void my_lib_func_with_deps(MyLibDeps *deps);

void my_lib_func();

#endif
```

#### project/my_lib.c
```c

#include "my_lib.h"

MyLibDeps my_lib_deps_default;

const char * check_lambdas_with_deps(MyLibDeps *deps){
    
    if(!deps->my_lib_printf){
        return "my_lib_printf";
    }
    if(!deps->my_lib_printf){
        return "my_lib_malloc";
    }
    return 0;
}
const char * check_lambdas(){
    return check_lambdas_with_deps(&my_lib_deps_default);
}

void my_lib_func_with_deps(MyLibDeps *deps){
    deps->my_lib_printf("Hello World\n");
}

void my_lib_func(){
    return my_lib_func_with_deps(&my_lib_deps_default);
}

```