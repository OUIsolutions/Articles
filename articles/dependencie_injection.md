# Dependency Injection

## Introduction

Dependency injection is a design pattern that helps create libraries that can run across multiple operating systems efficiently. When developing a library, one common challenge is providing support for various operating systems, as each has its own unique way of handling operations. The fundamental concept of dependency injection is straightforward: instead of trying to support every operating system directly, you delegate that responsibility to the library user.

By implementing dependency injection, you allow users to provide the necessary dependencies the library needs to function. This approach ensures your library can run on any operating system that the user wants to support, improving flexibility and maintainability.

## Approaches to Dependency Injection

There are multiple methods to implement dependency injection, each with its own advantages and disadvantages. This guide will explore both compile-time and runtime approaches.

### The Compile-Time Approach: Macros

While not my preferred method, macro-based dependency injection is widely used. This approach requires the user to define dependencies at compile time through macros. The library then uses these definitions to access the required functionality.

#### Project Structure:
```
project/
├── deps.h
├── main.c
├── my_lib.c
└── my_lib.h
```

#### Example Implementation:

**my_lib.h:**
```c
#ifndef MY_LIB_H
#define MY_LIB_H

void my_lib_func();

#endif
```

**my_lib.c:**
```c
#include "my_lib.h"
#include MY_LIB_DEPS

#ifndef my_lib_printf 
#error "my_lib_printf is not defined"
#endif

void my_lib_func(){
    my_lib_printf("Hello World\n");
}
```

**deps.h:**
```c
#include <stdio.h>
#define my_lib_printf printf
```

**main.c:**
```c
#include <stdio.h>
#include "my_lib.h"

int main(){
    my_lib_func();
}
```

**Compilation Command:**
```sh
gcc project/main.c project/my_lib.c -DMY_LIB_DEPS='"deps.h"'
```

#### Advantages:
- Simple implementation
- Compile time errors (way more easy to debu)
- No global variables required
- Minimal runtime overhead

#### Disadvantages:
- Cannot change dependencies during runtime
- Limited granularity for dependency injection
- Requires recompilation when dependencies change
- More complex build process

## Runtime Dependency Injection

Runtime dependency injection offers greater flexibility than compile-time approaches. Here are three common methods:

### 1. The Global Function Pointer Approach

This approach uses global function pointers that the user must set before using the library. This provides flexibility while maintaining simplicity.

#### Project Structure:
```
project/
├── main.c
├── my_lib.c
└── my_lib.h
```

**my_lib.h:**
```c
#ifndef MY_LIB_H
#define MY_LIB_H

extern int (*my_lib_printf)(const char *format, ...);
extern void *(*my_lib_malloc)(unsigned long int size);

const char * check_dependencies();
void my_lib_func();

#endif
```

**my_lib.c:**
```c
#include "my_lib.h"

int (*my_lib_printf)(const char *format, ...);
void *(*my_lib_malloc)(unsigned long int size);

const char * check_dependencies(){
    if(!my_lib_printf){
        return "my_lib_printf";
    }
    if(!my_lib_malloc){
        return "my_lib_malloc";
    }
    return 0;
}

void my_lib_func(){
    my_lib_printf("Hello World\n");
}
```

**main.c:**
```c
#include "my_lib.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    my_lib_printf = printf;
    my_lib_malloc = malloc;

    const char *import_error = check_dependencies();
    if(import_error){
        printf("Error importing %s\n", import_error);
        return 1;
    }

    my_lib_func();
}
```

#### Advantages:
- Runtime flexibility
- Fine-grained dependency control
- No recompilation needed when changing dependencies
- Supports testing with different dependency implementations

#### Disadvantages:
- Requires explicit dependency setup
- Mandatory validation before library use
- Global state can lead to complications in complex applications

### 2. The Global Struct Approach

This approach uses a global struct to store function pointers. It offers better organization than individual global function pointers.

**my_lib.h:**
```c
#ifndef MY_LIB_H
#define MY_LIB_H

typedef struct MyLibDeps{
    int (*my_lib_printf)(const char *format, ...);
    void *(*my_lib_malloc)(unsigned long int size);
} MyLibDeps;

extern MyLibDeps my_lib_deps;

const char * check_dependencies();
void my_lib_func();

#endif
```

**my_lib.c:**
```c
#include "my_lib.h"

MyLibDeps my_lib_deps;

const char * check_dependencies(){
    if(!my_lib_deps.my_lib_printf){
        return "my_lib_printf";
    }
    if(!my_lib_deps.my_lib_malloc){
        return "my_lib_malloc";
    }
    return 0;
}

void my_lib_func(){
    my_lib_deps.my_lib_printf("Hello World\n");
}
```

**main.c:**
```c
#include "my_lib.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    my_lib_deps.my_lib_printf = printf;
    my_lib_deps.my_lib_malloc = malloc;

    const char *import_error = check_dependencies();
    if(import_error){
        printf("Error importing %s\n", import_error);
        return 1;
    }

    my_lib_func();
}
```

#### Advantages:
- All the benefits of the global function pointer approach
- Centralized dependency declarations
- Improved code organization
- Easier to extend with additional dependencies

#### Disadvantages:
- Still requires explicit dependency setup
- Still uses global state

### 3. The Interface Approach

This approach passes a dependency struct to each function that needs it. This eliminates global state and provides the most flexibility.

**my_lib.h:**
```c
#ifndef MY_LIB_H
#define MY_LIB_H

typedef struct MyLibDeps{
    int (*my_lib_printf)(const char *format, ...);
    void *(*my_lib_malloc)(unsigned long int size);
} MyLibDeps;

const char * check_dependencies(MyLibDeps *deps);
void my_lib_func(MyLibDeps *deps);

#endif
```

**my_lib.c:**
```c
#include "my_lib.h"

const char * check_dependencies(MyLibDeps *deps){
    if(!deps->my_lib_printf){
        return "my_lib_printf";
    }
    if(!deps->my_lib_malloc){
        return "my_lib_malloc";
    }
    return 0;
}

void my_lib_func(MyLibDeps *deps){
    deps->my_lib_printf("Hello World\n");
}
```

**main.c:**
```c
#include "my_lib.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    MyLibDeps my_lib_deps = {
        .my_lib_printf = printf,
        .my_lib_malloc = malloc
    };

    const char *import_error = check_dependencies(&my_lib_deps);
    if(import_error){
        printf("Error importing %s\n", import_error);
        return 1;
    }

    my_lib_func(&my_lib_deps);
}
```

#### Advantages:
- No global state
- Maximum flexibility
- Different dependencies can be used for different parts of the application
- Supports multiple concurrent instances with different dependencies
- Ideal for multithreaded applications

#### Disadvantages:
- Requires passing dependency struct to every function
- Can result in more verbose code
- May increase parameter count in function signatures

### 4. The Hybrid Approach

This approach combines the interface method with a global default struct. It offers both flexibility and convenience.

**my_lib.h:**
```c
#ifndef MY_LIB_H
#define MY_LIB_H

typedef struct MyLibDeps{
    int (*my_lib_printf)(const char *format, ...);
    void *(*my_lib_malloc)(unsigned long int size);
} MyLibDeps;

extern MyLibDeps my_lib_deps_default;

const char * check_dependencies_with_deps(MyLibDeps *deps);
const char * check_dependencies();
void my_lib_func_with_deps(MyLibDeps *deps);
void my_lib_func();

#endif
```

**my_lib.c:**
```c
#include "my_lib.h"

MyLibDeps my_lib_deps_default;

const char * check_dependencies_with_deps(MyLibDeps *deps){
    if(!deps->my_lib_printf){
        return "my_lib_printf";
    }
    if(!deps->my_lib_malloc){
        return "my_lib_malloc";
    }
    return 0;
}

const char * check_dependencies(){
    return check_dependencies_with_deps(&my_lib_deps_default);
}

void my_lib_func_with_deps(MyLibDeps *deps){
    deps->my_lib_printf("Hello World\n");
}

void my_lib_func(){
    return my_lib_func_with_deps(&my_lib_deps_default);
}
```

**main.c:**
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
    // Default configuration
    my_lib_deps_default.my_lib_printf = printf;
    my_lib_deps_default.my_lib_malloc = malloc;

    const char *import_error = check_dependencies();
    if(import_error){
        printf("Error importing %s\n", import_error);
        return 1;
    }

    // Using default dependencies
    my_lib_func();

    // Custom configuration
    MyLibDeps my_lib_deps_custom = {
        .my_lib_printf = print_green,
        .my_lib_malloc = malloc
    };

    import_error = check_dependencies_with_deps(&my_lib_deps_custom);
    if(import_error){
        printf("Error importing %s\n", import_error);
        return 1;
    }

    // Using custom dependencies
    my_lib_func_with_deps(&my_lib_deps_custom);
}
```

#### Advantages:
- Combines convenience of global defaults with flexibility of interface approach
- Provides simple API for common cases
- Allows advanced usage when needed
- Supports both global and localized dependency injection

#### Disadvantages:
- Increased code complexity
- Larger API surface area
- Potential for confusion between global and local dependencies

## Conclusion

Dependency injection is a powerful design pattern that enables the creation of flexible, platform-independent libraries. By allowing users to provide implementation details, you can create libraries that work across multiple operating systems without having to handle every platform-specific detail yourself.

When choosing a dependency injection approach, consider your specific requirements:

- Avoid Macros for dependency injection due to their limitations
- For simple libraries, global function pointers may be sufficient
- For more complex libraries, consider struct-based approaches
- The hybrid approach offers the best flexibility but at the cost of complexity

Remember that the goal of dependency injection is to make your library more versatile while reducing the burden of platform-specific code. By implementing it effectively, you can create libraries that are easier to maintain, test, and use across different environments.
