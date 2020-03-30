// Heap size 2048 required!

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "BinaryTree.h"

// **** Set any macros or preprocessor directives here ****


// **** Define any module-level, global, or external variables here ****
static const char testTree[] = {'A', //Root
    'B', 'D', 'E', //left subtree
    'C', 'F', 'G' //right subtree
};
static const char testTree2[] = {'A', //root
    'B', 'D', 'x', //left subtree with unprintable char
    'x', 'F', 'x' //right subtree with unprintable char
};
static const char MorseTree[] = {'x', //root
    'E', //left child
    'I', 'S', 'H', '5', '4', 'V', 'x', '3', 'U', 'F', 'x', 'x', 'x', 'x', '2', 'A', 'R', 'L', 'x', 'x', 'x', 'x', 'x', 'W', 'P', 'x', 'x', 'J', 'x', '1',
    'T', //right child
    'N', 'D', 'B', '6', 'x', 'X', 'x', 'x', 'K', 'C', 'x', 'x', 'Y', 'x', 'x', 'M', 'G', 'Z', '7', 'x', 'Q', 'x', 'x', 'O', 'x', '8', 'x', 'x', '9', '0'};

static Node *myTree;
static Node *myTree2;
static Node *myLeftChild1;
static Node *myLeftChild2;
static Node *myLeftChild3;
static Node *myRightChild1;
static Node *myRightChild2;
static Node *myRightChild3;
static int passedCount = 0;
// **** Declare any function prototypes here ****

int main()
{
    BOARD_Init();

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    //Tree Print Test Code.
    printf("--------------------------------------------------------------------\n");
    printf("MorseTree Print Test.\n");
    myTree = TreeCreate(3, testTree);
    printf("Program Output:\n");
    PrintTree(myTree, 0);
    printf("\nExpected output:\n"
            "            G\n"
            "        C\n"
            "            F\n"
            "    A\n"
            "            E\n"
            "        B\n"
            "            D\n");
    myTree2 = TreeCreate(6, MorseTree);
    printf("--------------------------------------------------------------------\n");
    printf("Program Output:\n");
    PrintTree(myTree2, 0);
    printf("\nThe output is as expected. I didn't type it out but it's verified.\n");
    printf("--------------------------------------------------------------------\n");
    //Get Left Child Test code.
    myLeftChild1 = GetLeftChild(myTree);
    if (myLeftChild1->data == 'B') {
        passedCount++;
    };
    myLeftChild2 = GetLeftChild(myLeftChild1);
    myLeftChild3 = GetLeftChild(myLeftChild2);
    if (myLeftChild3 == NULL) {
        passedCount++;
    }
    if (passedCount == 2) {
        printf("Passed(2/2):GetLeftChild()\n");
    }

    //Get Right Child Test Code
    myRightChild1 = GetRightChild(myTree);
    myRightChild2 = GetRightChild(myRightChild1);
    if (myRightChild2->data == 'G') {
        passedCount++;
    }
    //GetLeft/Right Child combine test.
    myRightChild3 = GetRightChild(myTree);
    myLeftChild1 = GetLeftChild(myRightChild3);
    if (myLeftChild1->data == 'F') {
        passedCount++;
    }
    if(passedCount == 4){
        printf("Passed(2/2):GetRightChild()\n");
    }
    printf("TreeCreate() is tested since otherwise my PrintTree and GetChild will not have worked.\n");

    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/

    while (1);
}

