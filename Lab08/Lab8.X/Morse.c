#include "Morse.h"
#include "BinaryTree.h"
#include "BOARD.h"
#include "Buttons.h"

static uint8_t butEvent = 0;
static int freeRunningCounter = 0;
static Node *root = NULL;
static Node *mTree;

//state of morse events

static enum {
    WAITING,
    DOT,
    DASH,
    INTER_LETTER
} MorseState;

//create morse tree based on the diagram.
//I use lowercase letter 'x' to complete the trees.
static const char MorseTree[] = {'x', //root
    'E', //left child
    'I', 'S', 'H', '5', '4', 'V', 'x', '3', 'U', 'F', 'x', 'x', 'x', 'x', '2', 'A', 'R', 'L', 'x', 'x', 'x', 'x', 'x', 'W', 'P', 'x', 'x', 'J', 'x', '1',
    'T', //right child
    'N', 'D', 'B', '6', 'x', 'X', 'x', 'x', 'K', 'C', 'x', 'x', 'Y', 'x', 'x', 'M', 'G', 'Z', '7', 'x', 'Q', 'x', 'x', 'O', 'x', '8', 'x', 'x', '9', '0'};

/**
 * This function initializes the Morse code decoder. This is primarily the generation of the
 * Morse tree: a binary tree consisting of all of the ASCII alphanumeric characters arranged
 * according to the DOTs and DASHes that represent each character. Traversal of the tree is done by
 * taking the left-child if it is a dot and the right-child if it is a dash. If the morse tree is
 * successfully generated, SUCCESS is returned, otherwise STANDARD_ERROR is returned. This function
 * also initializes the Buttons library so that MorseCheckEvents() can work properly.
 * @return Either SUCCESS if the decoding tree was successfully created or STANDARD_ERROR if not.
 */
int MorseInit(void)
{
    mTree = TreeCreate(6, MorseTree);
    MorseState = WAITING;
    if (mTree == NULL) {
        return STANDARD_ERROR;
    } else {
        ButtonsInit();
        root = mTree; //store the initial state for reset purposes.
        return SUCCESS;
    }
}

/**
 * This function calls ButtonsCheckEvents() once per call and returns which, if any,
 * of the Morse code events listed in the enum above have been encountered. It checks for BTN4
 * events in its input and should be called at 100Hz so that the timing works. The
 * length that BTN4 needs to be held down for a dot is >= 0.25s and < 0.50s with a dash being a button
 * down event for >= 0.5s. The button uptime various between dots/dashes (>= .5s), letters
 * (>= 1s), and words (>= 2s).
 *
 * @note This function assumes that the buttons are all unpressed at startup, so that the first
 *       event it will see is a BUTTON_EVENT_*DOWN.
 *
 * So pressing the button for 0.3s, releasing it for 0.3s, pressing it for 0.6s, and then waiting
 * will decode the string '.-' (A). It will trigger the following order of events:
 * MORSE_EVENT_NONEs, 1 MORSE_EVENT_DOT, more MORSE_EVENT_NONEs, a MORSE_EVENT_DASH, more
 * MORSE_EVENT_NONEs, a MORSE_EVENT_END_CHAR, and then MORSE_EVENT_INTER_WORDs.
 * 
 * @return The MorseEvent that occurred.
 */
MorseEvent MorseCheckEvents(void)
{
    butEvent = ButtonsCheckEvents();
    freeRunningCounter++;
    switch (MorseState) {
    case(WAITING):
        if (butEvent == BUTTON_EVENT_4DOWN) {
            freeRunningCounter = 0;
            MorseState = DOT;
        } else {
            MorseState = WAITING;
            return MORSE_EVENT_NONE;
        }
        break;
    case(DOT):
        if (freeRunningCounter >= MORSE_EVENT_LENGTH_DOWN_DOT) {
            MorseState = DASH;
        } else if (butEvent == BUTTON_EVENT_4UP) {
            freeRunningCounter = 0;
            MorseState = INTER_LETTER;
            return MORSE_EVENT_DOT;
        } else {
            return MORSE_EVENT_NONE;
        }
        break;
    case(DASH):
        if (butEvent == BUTTON_EVENT_4UP) {
            freeRunningCounter = 0;
            MorseState = INTER_LETTER;
            return MORSE_EVENT_DASH;
        }
        break;
    case(INTER_LETTER):
        if (freeRunningCounter >= MORSE_EVENT_LENGTH_UP_INTER_LETTER_TIMEOUT) {
            MorseState = WAITING;
            return MORSE_EVENT_INTER_WORD;
        } else if (butEvent == BUTTON_EVENT_4DOWN) {
            if (freeRunningCounter >= MORSE_EVENT_LENGTH_UP_INTER_LETTER) {
                freeRunningCounter = 0;
                MorseState = DOT;
                return MORSE_EVENT_INTER_LETTER;

            } else {
                freeRunningCounter = 0;
                MorseState = DOT;
                return MORSE_EVENT_NONE;

            }
        }
        return MORSE_EVENT_NONE;
        break;
    }
    return MORSE_EVENT_NONE; //default return NONE EVENT.
}

/**
 * MorseDecode decodes a Morse string by iteratively being passed MORSE_CHAR_DOT or MORSE_CHAR_DASH.
 * Each call that passes a DOT or DASH returns a SUCCESS if the string could still compose a
 * Morse-encoded character. Passing in the special MORSE_CHAR_END_OF_CHAR constant will terminate
 * decoding and return the decoded character. During that call to MorseDecode() the return value
 * will be the character that was decoded or STANDARD_ERROR if it couldn't be decoded. Another
 * special value exists, MORSE_CHAR_DECODE_RESET, which will clear the stored state. When a
 * MORSE_CHAR_DECODE_RESET is done, SUCCESS will be returned. If the input is not a valid MorseChar
 * then the internal state should be reset and STANDARD_ERROR should be returned.
 * 
 * @param in A value from the MorseChar enum which specifies how to traverse the Morse tree.
 * 
 * @return Either SUCCESS on DECODE_RESET or when the next traversal location is still a valid
 *         character, the decoded character on END_OF_CHAR, or STANDARD_ERROR if the Morse tree
 *         hasn't been initialized, the next traversal location doesn't exist/represent a character,
 *         or `in` isn't a valid member of the MorseChar enum.
 */
char MorseDecode(MorseChar in)
{
    if (mTree != NULL) {
        if (in == MORSE_CHAR_DOT) {
            if (root != NULL) {
                root = GetLeftChild(root);
                return SUCCESS;
            } else {
                return STANDARD_ERROR;
            }
        } else if (in == MORSE_CHAR_DASH) {
            if (root != NULL) {
                root = GetRightChild(root);
                return SUCCESS;
            } else {
                return STANDARD_ERROR;
            }
        } else if (in == MORSE_CHAR_END_OF_CHAR) {
            if (root != NULL) {
                return root->data;
            } else {
                return STANDARD_ERROR;
            }
        } else if (in == MORSE_CHAR_DECODE_RESET) {
            root = mTree;
            return SUCCESS;
        }
        //non valid char and other stuff
        root = mTree;
        return STANDARD_ERROR;
    } else {
        return STANDARD_ERROR; //Tree not initialized
    }
}
