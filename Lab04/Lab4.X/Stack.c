/*Include libraries*/
#include "BOARD.h"
#include "Stack.h"
#include "stdio.h"

void StackInit(struct Stack *stack)
{
    stack ->currentItemIndex = -1;
    stack ->initialized = TRUE;
}

int StackIsEmpty(const struct Stack *stack)
{
    if (stack ->currentItemIndex == -1 && stack->initialized == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int StackIsFull(const struct Stack *stack)
{
    if (stack ->currentItemIndex == STACK_SIZE - 1 && stack ->initialized == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int StackPush(struct Stack *stack, float value)
{
    stack ->currentItemIndex++;
    stack ->stackItems[stack->currentItemIndex] = value;
    if (stack ->initialized == TRUE && stack -> currentItemIndex != STACK_SIZE - 1) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

int StackPop(struct Stack *stack, float *value)
{
    *value = stack ->stackItems[stack->currentItemIndex];
    stack ->currentItemIndex--;
    if (stack->currentItemIndex == -1 || stack ->initialized != TRUE) {
        return STANDARD_ERROR;
    } else {
        return SUCCESS;
    }
}

int StackGetSize(const struct Stack *stack)
{
    if (stack->initialized != TRUE) {
        return SIZE_ERROR;
    } else if (stack->currentItemIndex == -1 && stack ->initialized == TRUE) {
        return 0;
    } else {
        return stack->currentItemIndex;
    }
}