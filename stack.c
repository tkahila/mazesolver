#include "stack.h"
#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

//Function for creating stack
Stack *createStack(int capacity) {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->data = (Point *)malloc(capacity * sizeof(Point));
    return stack;
}

//Function to check if stack is empty
int isEmpty(Stack *stack) {
    return (stack->top == -1);
}

//Function to check if stack is full
int isFull(Stack *stack) {
    return (stack->top == stack->capacity - 1);
}

//Function to add new value to stack. Using memory reallocating if stack is full
void push(Stack *stack, Point point) {
    if (stack->top == stack->capacity - 1) {
        stack->capacity += 1;
        stack->data = (Point *)realloc(stack->data, stack->capacity * sizeof(Point));
    }
    stack->data[++stack->top] = point;
}


//Function to remove point from stack and return it
Point pop(Stack *stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty. Can not remove\n");
        Point empty = {0, 0};
        return empty;
    }
    return stack->data[stack->top--];
}

//Function to return latest point added to stack. Returns empty point if empty
Point peek(Stack *stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty. Can not read\n");
        Point empty = {0, 0};
        return empty;
    }
    return stack->data[stack->top];
}

//Function to freeing stack memory
void destroyStack(Stack *stack) {
    free(stack->data);
    free(stack);
}
