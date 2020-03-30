// User libraries
#include "LinkedList.h"
//CMPE13 Support Library
#include "BOARD.h"
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>
// Microchip libraries
#include <xc.h>
#include <plib.h>

//LinkedListNew function implementation

ListItem *LinkedListNew(char *data)
{
    ListItem *new = malloc(sizeof (ListItem));
    if (new == NULL) {
        return NULL;
    } else {
        new->data = data;
        new->previousItem = NULL;
        new->nextItem = NULL;
        return new;
    }
}

ListItem *LinkedListCreateAfter(ListItem *item, char *data)
{
    ListItem *new = LinkedListNew(data);
    if (new == NULL) {
        return NULL;
    }
    //Item is at the end of the list
    if (item->nextItem == NULL) {
        new->nextItem = NULL;
        new->previousItem = item;
        item->nextItem = new;
        //item is not at the end of the list.
    } else {
        new->previousItem = item;
        new->nextItem = item->nextItem;
        item->nextItem->previousItem = new;
        item->nextItem = new;
    }
    return new;
}

ListItem *LinkedListGetFirst(ListItem *list)
{
    if (list == NULL) {
        return NULL;
    }
    while (list->previousItem != NULL) {
        list = list->previousItem;
    }
    return list;
}

int LinkedListSize(ListItem *list)
{
    ListItem *count = LinkedListGetFirst(list);
    int counter = 0;
    while (count != NULL) {
        counter++;
        count = count ->nextItem;
    }
    return counter;
}

int LinkedListPrint(ListItem *list)
{
    if (list == NULL) {
        return STANDARD_ERROR;
    }
    //return to the first item in the list
    list = LinkedListGetFirst(list);
    printf("[");
    while (list != NULL) {
        //print out the NULL argument in the list.
        if (list->data == NULL) {
            printf(" (null) ");
        } else {
            printf(" %s ", list->data);
        }
        list = list ->nextItem;
    }
    printf("]\n");
    return SUCCESS;
}

char *LinkedListRemove(ListItem *item)
{
    ListItem *removed = item;
    char *data = removed ->data;
    if (removed == NULL) {
        return NULL;
    }//remove the head Item.
    else if (removed->previousItem == NULL) {
        removed->nextItem->previousItem = NULL;
        free(removed);
        //remove the end item.
    } else if (removed->nextItem == NULL) {
        removed->previousItem->nextItem = NULL;
        free(removed);
    }//remove middle item.
    else {
        removed ->previousItem->nextItem = removed->nextItem;
        removed->nextItem->previousItem = removed->previousItem;
        free(removed);
    }
    return data;
}

int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem)
{
    if (firstItem != NULL || secondItem != NULL) {
        //assign a pointer to store the firstItem;
        char *swap = NULL;
        swap = firstItem ->data;
        firstItem->data = secondItem->data;
        secondItem->data = swap;
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

int LinkedListSort(ListItem *list)
{
    ListItem *preWord;
    ListItem *nxtWord;
    if (list == NULL) {
        return STANDARD_ERROR;
    }
    for (preWord = list; preWord ->nextItem != NULL; preWord = preWord ->nextItem) {
        for (nxtWord = preWord->nextItem; nxtWord != NULL; nxtWord = nxtWord->nextItem) {
            if(preWord->data == NULL){ //I'm not exactly sure why this works. I try return STANDARD_ERROR but it doesn't work.
                break;                //but this one does the job.
            }
            if (nxtWord->data == NULL) { //swap Null to the first place.
                LinkedListSwapData(preWord, nxtWord);
            } else if (strlen(preWord->data) > strlen(nxtWord->data)) {
                LinkedListSwapData(preWord, nxtWord);
            } else if (strlen(preWord->data) == strlen(nxtWord->data)) { //Rank by Alphabetical order when they have the same length.
                if (strcmp(nxtWord->data, preWord->data) < 0) {
                    LinkedListSwapData(preWord, nxtWord);
                }
            }
        }
    }
    return SUCCESS;
}