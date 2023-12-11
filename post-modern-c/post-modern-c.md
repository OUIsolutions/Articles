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

~~~c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//=======================Declaration==============================================
typedef struct {
    char *name;
    char *color;
    int speed;
}Car;
Car * Car_constructor(const char *name, const char *color, int speed);

void Car_setName(Car *car, const char *name);
void Car_setColor(Car *car, const char *color);
void Car_setSpeed(Car *car, int speed);

char * Car_getName(Car *car);
char * Car_getColor(Car *car);
int Car_getSpeed(Car *car);

void Car_print(Car *car);
void Car_free(Car *car);


//=======================Definition==============================================
Car * Car_constructor(const char *name, const char *color, int speed){

    Car *self = (Car*)malloc(sizeof(Car));
    //use these pattern to avoid garbage values
    *self = (Car){0};
    Car_setName(self, name);
    Car_setColor(self, color);
    Car_setSpeed(self, speed);
    return self;
}

void Car_setName(Car *self, const char *name){
   if(self->name)free(self->name);
   self->name = strdup(name); 
}

void Car_setColor(Car *self, const char *color){
   if(self->color)free(self->color);
   self->color = strdup(color); 
}

void Car_setSpeed(Car *self, int speed){
   self->speed = speed; 
}

char * Car_getName(Car *self){
    return self->name;
}

char * Car_getColor(Car *self){
    return self->color;
}

int Car_getSpeed(Car *self){
    return self->speed;
}

void Car_print(Car *self){
    printf("Name: %s\n", Car_getName(self));
    printf("Color: %s\n", Car_getColor(self));
    printf("Speed: %d\n", Car_getSpeed(self));
}

void Car_free(Car *self){
    
    if(self->name)free(self->name);
    if(self->color)free(self->color);
    free(self);
}

//=======================Main====================================================
int main(){
    Car *car = Car_constructor("BMW", "Black", 200);
    Car_print(car);
    Car_free(car);
    return 0;
}

~~~

## Arrays
Arrays must follow the standard of having the name of the corresponding structure, followed by the sulfix: Array
and implement the append, pop methods
as well as you can also implement the foreach, filter, etc. methods if necessary

~~~c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct {
    char **elements;
    int size;
}StringArray;

StringArray * StringArray_constructor();

void StringArray_append(StringArray *self, const char *element);

void StringArray_append_getting_ownership(StringArray *self,  char *element);

void StringArray_insert(StringArray *self, int index, const char *element);

void StringArray_pop(StringArray *self,int index);

char* StringArray_get(StringArray *self, int index);

void StringArray_print(StringArray *self);

void StringArray_free(StringArray *self);

//functional methods 
StringArray * StringArray_map(StringArray *self, char* (*callback)(char*,void *args));
StringArray *StringArray_filter(StringArray *self, bool (*callback)(char*,void *args));
void StringArray_foreach(StringArray *self, void (*callback)(char*,void *args));

void StringArray_self_map(StringArray *self, char* (*callback)(char*,void *args));
void StringArray_self_filter(StringArray *self, bool (*callback)(char*,void *args));


//=======================Definition==============================================
StringArray * StringArray_constructor(){
    StringArray *self = (StringArray*)malloc(sizeof(StringArray));
    *self = (StringArray){0};
    self->elements = (char**)malloc(0);
    
    return self;
}

void StringArray_append(StringArray *self, const char *element){
    self->elements = (char**)realloc(self->elements, sizeof(char*) * (self->size + 1));
    self->elements[self->size] = strdup(element);
    self->size++;
}

void StringArray_append_getting_ownership(StringArray *self,  char *element){
    self->elements = (char**)realloc(self->elements, sizeof(char*) * (self->size + 1));
    self->elements[self->size] =element;
    self->size++;
}


void StringArray_insert(StringArray *self, int index, const char *element){
    int converted_index = index;
    if(index < 0)converted_index = self->size + index;
    if(converted_index < 0 || converted_index >= self->size)return;

    self->elements = (char**)realloc(self->elements, sizeof(char*) * (self->size + 1));
    for(int i = self->size; i > converted_index; i--){
        self->elements[i] = self->elements[i - 1];
    }
    self->elements[converted_index] = strdup(element);
    self->size++;
}


void StringArray_pop(StringArray *self,int index){
    int converted_index = index;
    if(index < 0)converted_index = self->size + index;
    if(converted_index < 0 || converted_index >= self->size)return;

    free(self->elements[converted_index]);
    for(int i = converted_index; i < self->size - 1; i++){
        self->elements[i] = self->elements[i + 1];
    }
    self->size--;
}

char* StringArray_get(StringArray *self, int index){
    int converted_index = index;
    if(index < 0)converted_index = self->size + index;
    if(converted_index < 0 || converted_index >= self->size){
        return  NULL;
    };

    return self->elements[converted_index];
}


void StringArray_print(StringArray *self){
    for(int i = 0; i < self->size; i++){
        printf("%s\n", StringArray_get(self, i));
    }
}

void StringArray_free(StringArray *self){
    for(int i = 0; i < self->size; i++){
        free(self->elements[i]);
    }
    free(self->elements);
    free(self);
}

StringArray * StringArray_map(StringArray *self, char* (*callback)(char*,void *args)){
    StringArray *new_array = StringArray_constructor();
    for(int i = 0; i < self->size; i++){
        char *new_element = callback(StringArray_get(self, i), NULL);
        StringArray_append_getting_ownership(new_array, new_element);
    }
    return new_array;

}

StringArray *StringArray_filter(StringArray *self, bool (*callback)(char*,void *args)){
    StringArray *new_array = StringArray_constructor();
    for(int i = 0; i < self->size; i++){
        if(callback(StringArray_get(self, i), NULL)){
            StringArray_append(new_array, StringArray_get(self, i));
        }
    }
    return new_array;

}

void StringArray_foreach(StringArray *self, void (*callback)(char*,void *args)){
    for(int i = 0; i < self->size; i++){
        callback(StringArray_get(self, i), NULL);
    }
}


void StringArray_self_map(StringArray *self, char* (*callback)(char*,void *args)){
    for(int i = 0; i < self->size; i++){
        char *new_element = callback(StringArray_get(self, i), NULL);
        free(self->elements[i]);
        self->elements[i] = new_element;
    }

}
void StringArray_self_filter(StringArray *self, bool (*callback)(char*,void *args)){
    for(int i = 0; i < self->size; i++){
        if(!callback(StringArray_get(self, i), NULL)){
            StringArray_pop(self, i);
            i--;
        }
    }
}


//=======================Usage==============================================
bool not_John(char *name, void *args){
    return strcmp(name, "John") != 0;
}

char *add_Mr(char *name, void *args){
    const char *prefix = "Mr. ";
    int new_name_size = strlen(name) + strlen(prefix);
    char *new_name = (char*)malloc(new_name_size+2);
    sprintf(new_name, "%s%s",prefix, name);
    new_name[new_name_size] = '\0';
    
    return new_name;
}

void print_name(char *name, void *args){
    printf("%s\n", name);
}

int main(){
    StringArray *names = StringArray_constructor();
    StringArray_append(names, "John");
    StringArray_append(names, "Doe");
    StringArray_append(names, "Jane");
    StringArray_append(names, "Doe");
    StringArray_insert(names, 0, "Mr.");
    StringArray_insert(names, -1, "Jr.");


    puts("filtered names");
    StringArray *filtered_names = StringArray_filter(names, not_John);
    StringArray_print(filtered_names);
    StringArray_free(filtered_names);
    
    puts("mapped names");
    StringArray *new_names = StringArray_map(names, add_Mr);
    StringArray_print(new_names);
    StringArray_free(new_names);

    StringArray_free(names);
    return 0;
}
~~~

## Namespaces