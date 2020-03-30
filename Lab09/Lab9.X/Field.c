/*Battle Boats Field
 * Xingyu Zhou
 * Partner: Mingyu Ding
 * Collaborate with :Jimmy Chen
 */
// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Agent.h"
#include "CircularBuffer.h"
#include "Leds.h"
#include "Oled.h"
#include "Buttons.h"
#include "Protocol.h"
#include "Uart1.h"
#include "Field.h"
#include "OledDriver.h"
#include "FieldOled.h"

//variables
#define TRUE 1
#define FALSE 0

/**
 * FieldInit() will fill the passed field array with the data specified in positionData. Also the
 * lives for each boat are filled according to the `BoatLives` enum.
 * @param f The field to initialize.
 * @param p The data to initialize the entire field to, should be a member of enum
 *                     FieldPosition.
 */
void FieldInit(Field *f, FieldPosition p)
{
    uint8_t i, j; //rows and col for FieldInit
    //set lives
    f->hugeBoatLives = FIELD_BOAT_LIVES_HUGE;
    f->largeBoatLives = FIELD_BOAT_LIVES_LARGE;
    f->mediumBoatLives = FIELD_BOAT_LIVES_MEDIUM;
    f->smallBoatLives = FIELD_BOAT_LIVES_SMALL;
    //field every spot with p.
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            f->field[i][j] = p;
        }
    }
}

/**
 * Retrieves the value at the specified field position.
 * @param f The Field being referenced
 * @param row The row-component of the location to retrieve
 * @param col The column-component of the location to retrieve
 * @return
 */
FieldPosition FieldAt(const Field *f, uint8_t row, uint8_t col)
{
    return f->field[row][col];
}

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_POSITION_CURSOR, this is the function to use.
 * 
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
FieldPosition FieldSetLocation(Field *f, uint8_t row, uint8_t col, FieldPosition p)
{
    //save oldPos
    FieldPosition oldPos = f->field[row][col];
    f->field[row][col] = p;
    return oldPos;
}

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField,FIELD_POSITION_EMPTY);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_SMALL);
 * }
 *
 * should result in a field like:
 *  _ _ _ _ _ _ _ _
 * [ 3 3 3       3 ]
 * [ 4 4 4 4     3 ]
 * [             3 ]
 *  . . . . . . . .
 *
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_POSITION_*_BOAT values from the
 * FieldPosition enum.
 * @return TRUE for success, FALSE for failure
 */
uint8_t FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type)
{
    //make sure the position is empty so that I can place the boat.
    if (f->field[row][col] == FIELD_POSITION_EMPTY) {
        //place the boat depending on the direction.
        if (dir == FIELD_BOAT_DIRECTION_NORTH) {
            //the reason why it's row - 2 is because the grid starts from 0.
            if (type == FIELD_BOAT_SMALL && row - 2 >= 0) { //make sure the small boat is within the bound.
                if ((f->field[row][col] == 0) && (f->field[row - 1][col] == 0) && (f->field[row - 2][col] == 0)) { //if every spot is empty, place the boat.
                    f->field[row][col] = FIELD_POSITION_SMALL_BOAT;
                    f->field[row - 1][col] = FIELD_POSITION_SMALL_BOAT;
                    f->field[row - 2][col] = FIELD_POSITION_SMALL_BOAT;
                    return TRUE;
                } else { // if the boat can't be placed, return FALSE/
                    return FALSE;
                }
            } else if (type == FIELD_BOAT_MEDIUM && row - 3 >= 0) {//make sure the medium boat is within the bound.
                if ((f->field[row][col] == 0) && (f->field[row - 1][col] == 0) && (f->field[row - 2][col] == 0) && (f->field[row - 3][col] == 0)) {
                    f->field[row][col] = FIELD_POSITION_MEDIUM_BOAT;
                    f->field[row - 1][col] = FIELD_POSITION_MEDIUM_BOAT;
                    f->field[row - 2][col] = FIELD_POSITION_MEDIUM_BOAT;
                    f->field[row - 3][col] = FIELD_POSITION_MEDIUM_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            }//set large boat
            else if (type == FIELD_BOAT_LARGE && row - 4 >= 0) {
                if ((f->field[row][col] == 0) && (f->field[row - 1][col] == 0) && (f->field[row - 2][col] == 0) && (f->field[row - 3][col] == 0) && (f->field[row - 4][col] == 0)) {
                    f->field[row][col] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row - 1][col] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row - 2][col] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row - 3][col] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row - 4][col] = FIELD_POSITION_LARGE_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            }//set huge boat.
            else if (type == FIELD_BOAT_HUGE && (row - 5 >= 0)) {
                if ((f->field[row][col] == 0) && (f->field[row - 1][col] == 0) && (f->field[row - 2][col] == 0) && (f->field[row - 3][col] == 0) && (f->field[row - 4][col] == 0) && (f->field[row - 5][col] == 0)) {
                    f->field[row][col] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row - 1][col] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row - 2][col] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row - 3][col] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row - 4][col] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row - 5][col] = FIELD_POSITION_HUGE_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else {
                return FALSE;
            }
        } else if (dir == FIELD_BOAT_DIRECTION_SOUTH) {
            if (type == FIELD_BOAT_SMALL && row + 2 <= 5) {
                if ((f->field[row][col] == 0) && (f->field[row + 1][col] == 0) && (f->field[row + 2][col] == 0)) {
                    f->field[row][col] = FIELD_POSITION_SMALL_BOAT;
                    f->field[row + 1][col] = FIELD_POSITION_SMALL_BOAT;
                    f->field[row + 2][col] = FIELD_POSITION_SMALL_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else if (type == FIELD_BOAT_MEDIUM && row + 3 <= 5) {
                if ((f->field[row][col] == 0) && (f->field[row + 1][col] == 0) && (f->field[row + 2][col] == 0) && (f->field[row + 3][col] == 0)) {
                    f->field[row][col] = FIELD_POSITION_MEDIUM_BOAT;
                    f->field[row + 1][col] = FIELD_POSITION_MEDIUM_BOAT;
                    f->field[row + 2][col] = FIELD_POSITION_MEDIUM_BOAT;
                    f->field[row + 3][col] = FIELD_POSITION_MEDIUM_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else if (type == FIELD_BOAT_LARGE && row + 4 <= 5) {
                if ((f->field[row][col] == 0) && (f->field[row + 1][col] == 0) && (f->field[row + 2][col] == 0) && (f->field[row + 3][col] == 0) && (f->field[row + 4][col] == 0)) {
                    f->field[row][col] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row + 1][col] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row + 2][col] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row + 3][col] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row + 4][col] = FIELD_POSITION_LARGE_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else if (type == FIELD_BOAT_HUGE && row + 5 <= 5) {
                if ((f->field[row][col] == 0) && (f->field[row + 1][col] == 0) && (f->field[row + 2][col] == 0) && (f->field[row + 3][col] == 0) && (f->field[row + 4][col] == 0) && (f->field[row + 5][col] == 0)) {
                    f->field[row][col] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row + 1][col] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row + 2][col] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row + 3][col] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row + 4][col] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row + 5][col] = FIELD_POSITION_HUGE_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else {
                return FALSE;
            }
        } else if (dir == FIELD_BOAT_DIRECTION_WEST) {
            if (type == FIELD_BOAT_SMALL && col - 2 >= 0) {
                if ((f->field[row][col] == 0) && (f->field[row][col - 1] == 0) && (f->field[row][col - 2] == 0)) {
                    f->field[row][col] = FIELD_POSITION_SMALL_BOAT;
                    f->field[row][col - 1] = FIELD_POSITION_SMALL_BOAT;
                    f->field[row][col - 2] = FIELD_POSITION_SMALL_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else if (type == FIELD_BOAT_MEDIUM && col - 3 >= 0) {
                if ((f->field[row][col] == 0) && (f->field[row][col - 1] == 0) && (f->field[row][col - 2] == 0) && (f->field[row][col - 3] == 0)) {
                    f->field[row][col] = FIELD_POSITION_MEDIUM_BOAT;
                    f->field[row][col - 1] = FIELD_POSITION_MEDIUM_BOAT;
                    f->field[row][col - 2] = FIELD_POSITION_MEDIUM_BOAT;
                    f->field[row][col - 3] = FIELD_POSITION_MEDIUM_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else if (type == FIELD_BOAT_LARGE && col - 4 >= 0) {
                if ((f->field[row][col] == 0) && (f->field[row][col - 1] == 0) && (f->field[row][col - 2] == 0) && (f->field[row][col - 3] == 0) && (f->field[row][col - 4] == 0)) {
                    f->field[row][col] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row][col - 1] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row][col - 2] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row][col - 3] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row][col - 4] = FIELD_POSITION_LARGE_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else if (type == FIELD_BOAT_HUGE && col - 5 >= 0) {
                if ((f->field[row][col] == 0) && (f->field[row][col - 1] == 0) && (f->field[row][col - 2] == 0) && (f->field[row][col - 3] == 0) && (f->field[row][col - 4] == 0) && (f->field[row][col - 5] == 0)) {
                    f->field[row][col] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row][col - 1] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row][col - 2] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row][col - 3] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row][col - 4] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row][col - 5] = FIELD_POSITION_HUGE_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else {
                return FALSE;
            }
        } else if (dir == FIELD_BOAT_DIRECTION_EAST) {
            if (type == FIELD_BOAT_SMALL && col + 2 <= 9) {
                if ((f->field[row][col] == 0) && (f->field[row][col + 1] == 0) && (f->field[row][col + 2] == 0)) {
                    f->field[row][col] = FIELD_POSITION_SMALL_BOAT;
                    f->field[row][col + 1] = FIELD_POSITION_SMALL_BOAT;
                    f->field[row][col + 2] = FIELD_POSITION_SMALL_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else if (type == FIELD_BOAT_MEDIUM && col + 3 <= 9) {
                if ((f->field[row][col] == 0) && (f->field[row][col + 1] == 0) && (f->field[row][col + 2] == 0) && (f->field[row][col + 3] == 0)) {
                    f->field[row][col] = FIELD_POSITION_MEDIUM_BOAT;
                    f->field[row][col + 1] = FIELD_POSITION_MEDIUM_BOAT;
                    f->field[row][col + 2] = FIELD_POSITION_MEDIUM_BOAT;
                    f->field[row][col + 3] = FIELD_POSITION_MEDIUM_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else if (type == FIELD_BOAT_LARGE && col + 4 <= 9) {
                if ((f->field[row][col] == 0) && (f->field[row][col + 1] == 0) && (f->field[row][col + 2] == 0) && (f->field[row][col + 3] == 0) && (f->field[row][col + 4] == 0)) {
                    f->field[row][col] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row][col + 1] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row][col + 2] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row][col + 3] = FIELD_POSITION_LARGE_BOAT;
                    f->field[row][col + 4] = FIELD_POSITION_LARGE_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else if (type == FIELD_BOAT_HUGE && col + 5 <= 9) {
                if ((f->field[row][col] == 0) && (f->field[row][col + 1] == 0) && (f->field[row][col + 2] == 0) && (f->field[row][col + 3] == 0) && (f->field[row][col + 4] == 0) && (f->field[row][col + 5] == 0)) {
                    f->field[row][col] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row][col + 1] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row][col + 2] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row][col + 3] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row][col + 4] = FIELD_POSITION_HUGE_BOAT;
                    f->field[row][col + 5] = FIELD_POSITION_HUGE_BOAT;
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else {
                return FALSE;
            }
        }
    } else {
        return FALSE;
    }
    return FALSE;
}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_POSITION_HIT or FIELD_POSITION_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The HIT result is stored in gData->hit as an
 *               output.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
FieldPosition FieldRegisterEnemyAttack(Field *f, GuessData * gData)
{
    FieldPosition oldPosition = f->field[gData->row][gData->col]; //store the position BEFORE it gets hit.
    if ((oldPosition != FIELD_POSITION_MISS) && oldPosition != FIELD_POSITION_EMPTY) {
        gData->hit = HIT_HIT;
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT; //BOAT HIT!
        if (oldPosition == FIELD_POSITION_SMALL_BOAT) { //small boat decrement life if it's not dead
            if (f->smallBoatLives != 0) {
                f->smallBoatLives--;
                if (f->smallBoatLives == 0) {
                    gData->hit = HIT_SUNK_SMALL_BOAT;
                    return oldPosition;
                }
            }
        } else if (oldPosition == FIELD_POSITION_MEDIUM_BOAT) { //medium boat decrement life if it's not dead.
            if (f->mediumBoatLives != 0) {
                f->mediumBoatLives--;
                if (f->mediumBoatLives == 0) {
                    gData->hit = HIT_SUNK_MEDIUM_BOAT;
                    return oldPosition;
                }
            }
        } else if (oldPosition == FIELD_POSITION_LARGE_BOAT) { //large boat decrement life if it's not dead.
            if (f->largeBoatLives != 0) {
                f->largeBoatLives--;
                if (f->largeBoatLives == 0) {
                    gData->hit = HIT_SUNK_LARGE_BOAT;
                    return oldPosition;
                }
            }
        } else if (oldPosition == FIELD_POSITION_HUGE_BOAT) { //huge boat decrement life if it's not dead.
            if (f->hugeBoatLives != 0) {
                f->hugeBoatLives--;
                if (f->hugeBoatLives == 0) {
                    gData->hit = HIT_SUNK_HUGE_BOAT;
                    return oldPosition;
                }
            }
        }

    } else { //YOU MISS THE SHOT!
        gData->hit = HIT_MISS;
        f->field[gData->row][gData->col] = FIELD_POSITION_MISS;
    }
    return oldPosition; //return position BEFORE the attack.
}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_POSITION_HIT at that position. If it was a miss, display a FIELD_POSITION_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * HIT_SUNK_*_BOAT.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
FieldPosition FieldUpdateKnowledge(Field *f, const GuessData * gData)
{
    FieldPosition oldPosition = f->field[gData->row][gData->col]; //store the previous position before hit/miss is registered.
    if (gData != HIT_MISS) {
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        //clear lives if the hit was a SUNK.
        if (gData->hit == HIT_SUNK_SMALL_BOAT) {
            f->smallBoatLives = 0;
        } else if (gData->hit == HIT_SUNK_MEDIUM_BOAT) {
            f->mediumBoatLives = 0;
        } else if (gData->hit == HIT_SUNK_LARGE_BOAT) {
            f->largeBoatLives = 0;
        } else if (gData->hit == HIT_SUNK_HUGE_BOAT) {
            f->hugeBoatLives = 0;
        }
    } else if (gData == HIT_MISS) { //miss the shot.
        f->field[gData->row][gData->col] = FIELD_POSITION_EMPTY;
    }
    return oldPosition; //return the previous position before hit/miss was registered.
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field * f)
{
    uint8_t boatStatus = 0; //reset the status every time so I can always get the latest boatStatus.
    if (f->smallBoatLives != 0) {
        boatStatus |= FIELD_BOAT_STATUS_SMALL;
    }
    if (f->mediumBoatLives != 0) {
        boatStatus |= FIELD_BOAT_STATUS_MEDIUM;
    }
    if (f->largeBoatLives != 0) {
        boatStatus |= FIELD_BOAT_STATUS_LARGE;
    }
    if (f->hugeBoatLives != 0) {
        boatStatus |= FIELD_BOAT_STATUS_HUGE;
    }
    return boatStatus;
}