

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
