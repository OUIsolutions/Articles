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