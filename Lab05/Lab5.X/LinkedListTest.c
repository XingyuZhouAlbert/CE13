// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "LinkedList.h"

// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****
struct ListItem *newList, *newList2, *newList3,
*newList4, *newList5, *newList6,
*newList7, *newList8, *newList9,
*newList10, *newList11, *newList12,
*newList13, *newList14, *newList15,
*newList16, *newList17, *newList18,
*newList19;
char *newData = "He";
char *newData2 = "Hello World I'm Albert";
char *dog = "dog";
char *cat = "cat";
char *cow = "cow";
char *man = "man";
char *yay = "yay";
char *benz = "Benz";
char *bmw = "BMW";
char *mazda = "Mazda";
char *honda = "Honda";
char *albert = "Albert";
char *newData3 = NULL;
int x;
int passed = 0;
// **** Define any module-level, global, or external variables here ****

// **** Declare any function prototypes here ****

int main()
{
    BOARD_Init();
    printf("\r\nWelcome to the LinkedList testing suite\r\n");
    //LLNew Test Code
    newList = LinkedListNew(newData);
    char *testPointer1 = newList->data; //manually assign a pointer for comparison.
    if (testPointer1 == newData) {
        passed++;
    }
    newList2 = LinkedListNew(newData2);
    char *testPointer2 = newList2->data;
    if (testPointer2 == newData2) {
        passed++;
    }
    if (passed == 2) {
        printf("Passed: LinkedListNew()\n");
    }

    //LLCreateAfter Test Code
    newList3 = LinkedListCreateAfter(newList2, dog);
    if (newList2->nextItem == newList3) {
        passed++;
    }
    newList4 = LinkedListCreateAfter(newList, newData);
    if (newList->nextItem == newList4) {
        passed++;
    }
    if (passed == 4) {
        printf("Passed: LinkedListCreateAfter()\n");
    }



    //LLgetSizeTtest Code
    LinkedListCreateAfter(newList2, cat);
    x = LinkedListSize(newList2);
    if (x == 3) {
        passed++;
    }
    LinkedListCreateAfter(newList2, cow);
    LinkedListCreateAfter(newList2, man);
    LinkedListCreateAfter(newList2, yay);
    x = LinkedListSize(newList2);
    if (x == 6) {
        passed++;
    }
    if (passed == 6) {
        printf("Passed: LinkedListSize()\n");
    }

    //LLPrint Test Code
    printf("-----------------------------------------------------------------------------------\n");
    printf("This is the expected output of LLprint:[dog cat cow man yay (null)]\n");
    printf("This is the actual output of LLprint:");
    newList6 = LinkedListNew(dog);
    newList7 = LinkedListCreateAfter(newList6, cat); //Creating a list
    newList8 = LinkedListCreateAfter(newList7, cow);
    newList9 = LinkedListCreateAfter(newList8, man);
    newList10 = LinkedListCreateAfter(newList9, yay);
    newList11 = LinkedListCreateAfter(newList10, newData3);
    LinkedListPrint(newList6);
    printf("\nThis is the expected output of 2nd test:[Benz BMW Mazda Honda (null) Albert (null)]\n");
    printf("This is the actual output of LLprint:");
    newList13 = LinkedListNew(benz);
    newList14 = LinkedListCreateAfter(newList13, bmw);
    newList15 = LinkedListCreateAfter(newList14, mazda);
    newList16 = LinkedListCreateAfter(newList15, honda);
    newList17 = LinkedListCreateAfter(newList16, newData3);
    newList18 = LinkedListCreateAfter(newList17, albert);
    newList19 = LinkedListCreateAfter(newList18, newData3);
    LinkedListPrint(newList13);

    //LLGetFirst Test Code
    printf("-----------------------------------------------------------------------------------\n");
    newList5 = LinkedListGetFirst(newList7);
    if (newList5->data == newList6->data) {
        passed++;
    }
    newList12 = LinkedListGetFirst(newList10);
    if (newList12->data == newList6->data) {
        passed++;
    }
    if (passed == 8) {
        printf("Passed: LinkedGetFirst()\n");
    }

    //LLSwapData Test Code
    //My newList 15 originally points to mazda and newList16 originally points to honda.
    LinkedListSwapData(newList15, newList16);
    if (newList15->data == honda && newList16->data == mazda) {
        passed++;
    }
    LinkedListSwapData(newList15, newList16);
    if (newList15->data == mazda && newList16->data == honda) { //Here I just simply switch back
        passed++;
    }
    if (passed == 10) {
        printf("Passed: LinkedListSwap()\n");
    }

    //LLsort Test Code
     printf("-----------------------------------------------------------------------------------\n");
    LinkedListSort(newList13);
    printf("Expected output of LLsort 1st test:[(null)  (null)  BMW  Benz  Honda  Mazda  Albert ]\n");
    printf("Actual output of LLsort 1st test:");
    LinkedListPrint(newList13);
    printf("\n");
    printf("Expected output of LLsort 2nd test:[ (null)  cat  cow  dog  man  yay ]\n");
    LinkedListSort(newList6);
    printf("Actual output of LLsort 2nd test:");
    LinkedListPrint(newList6);
    //LLremove Test Code
    printf("-----------------------------------------------------------------------------------\n");
    printf("Expected output of LLremove 1st test:[ (null)  (null)  BMW  Benz  Honda  Albert ]\n");
    LinkedListRemove(newList18);
    printf("Actual output of LLremove 1st test:");
    LinkedListPrint(newList13);
    printf("\n");
    printf("Expected output of LLremove 2nd test:[ (null)  (null)  BMW  Honda  Albert ]\n");
    LinkedListRemove(newList16);
    printf("Actual output of LLremove 2nd test:");
    LinkedListPrint(newList13);
    printf("-----------------------------------------------------------------------------------\n");
    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!
    while (1);
}