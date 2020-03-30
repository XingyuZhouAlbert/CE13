/*Xingyu Zhou
 Mingyu Ding
 Artificial Agent*/
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

// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****


// **** Define any module-level, global, or external variables here ****
Field myField, enemyField; //two fields for AgentInit.
ProtocolParserStatus incomeMsg;
NegotiationData myData, enemyData;
GuessData myGuessData, enemyGuessData;
AgentEvent agentEF;
AgentState ASM;
uint8_t validate;
TurnOrder turnState;
FieldOledTurn FOturnState;
uint16_t i;
// **** Declare any function prototypes here ****

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts. This can include things like initialization of the field, placement of the boats,
 * etc. The agent can assume that stdlib's rand() function has been seeded properly in order to
 * use it safely within.
 */
void AgentInit(void)
{
    //initializing both fields.
    FieldInit(&myField, FIELD_POSITION_EMPTY);
    FieldInit(&enemyField, FIELD_POSITION_UNKNOWN);
    //Setting the initial state of state machines.
    ASM = AGENT_STATE_GENERATE_NEG_DATA;
    //set boats randomly.
    while (1) {
        //add a small boat. %4 means to randomly generate a direction out of the four direction avaliable. 
        if (FieldAddBoat(&myField, (rand() % FIELD_ROWS), (rand() % FIELD_COLS), (rand() % 4), FIELD_BOAT_SMALL)) {
            break; //when the boat is successfully created, break.
        }
    }
    while (1) {
        if (FieldAddBoat(&myField, (rand() % FIELD_ROWS), (rand() % FIELD_COLS), (rand() % 4), FIELD_BOAT_MEDIUM)) {
            break; //comment same as above.
        }
    }
    while (1) {
        if (FieldAddBoat(&myField, (rand() % FIELD_ROWS), (rand() % FIELD_COLS), (rand() % 4), FIELD_BOAT_LARGE)) {
            break; //comment same as above.
        }
    }
    while (1) {
        if (FieldAddBoat(&myField, (rand() % FIELD_ROWS), (rand() % FIELD_COLS), (rand() % 4), FIELD_BOAT_HUGE)) {
            break; //comment same as above.
        }
    }
}

/**
 * The Run() function for an Agent takes in a single character. It then waits until enough
 * data is read that it can decode it as a full sentence via the Protocol interface. This data
 * is processed with any output returned via 'outBuffer', which is guaranteed to be 255
 * characters in length to allow for any valid NMEA0183 messages. The return value should be
 * the number of characters stored into 'outBuffer': so a 0 is both a perfectly valid output and
 * means a successful run.
 * @param in The next character in the incoming message stream.
 * @param outBuffer A string that should be transmit to the other agent. NULL if there is no
 *                  data.
 * @return The length of the string pointed to by outBuffer (excludes \0 character).
 */
int AgentRun(char in, char *outBuffer)
{
    while (in != NULL) { //setting AgentEventFlag using ParsingStatus. As well as recording all incoming data.
        incomeMsg = ProtocolDecode(in, &enemyData, &enemyGuessData);
        if (incomeMsg == PROTOCOL_PARSED_CHA_MESSAGE) {
            agentEF = AGENT_EVENT_RECEIVED_CHA_MESSAGE;
        } else if (incomeMsg == PROTOCOL_PARSED_HIT_MESSAGE) {
            agentEF = AGENT_EVENT_RECEIVED_HIT_MESSAGE;
        } else if (incomeMsg == PROTOCOL_PARSED_DET_MESSAGE) {
            agentEF = AGENT_EVENT_RECEIVED_DET_MESSAGE;
        } else if (incomeMsg == PROTOCOL_PARSED_COO_MESSAGE) {
            agentEF = AGENT_EVENT_RECEIVED_COO_MESSAGE;
        } else if (incomeMsg == PROTOCOL_PARSING_FAILURE) {
            agentEF = AGENT_EVENT_MESSAGE_PARSING_FAILED;
        } else if (incomeMsg == PROTOCOL_PARSING_GOOD || PROTOCOL_WAITING) {
            agentEF = AGENT_EVENT_NONE;
        }
    }
    if (agentEF == AGENT_EVENT_MESSAGE_PARSING_FAILED) { //goes to invalid if parsing failed from any states.
        ASM = AGENT_STATE_INVALID;
    }
    switch (ASM) {
    case(AGENT_STATE_GENERATE_NEG_DATA):
        ProtocolGenerateNegotiationData(&myData); //generate the negotiation Data
        ASM = AGENT_STATE_SEND_CHALLENGE_DATA;
        return ProtocolEncodeChaMessage(outBuffer, &myData);
        break;
    case(AGENT_STATE_SEND_CHALLENGE_DATA):
        if (agentEF == AGENT_EVENT_RECEIVED_CHA_MESSAGE) {
            ASM = AGENT_STATE_DETERMINE_TURN_ORDER;
            return ProtocolEncodeDetMessage(outBuffer, &myData);
            break;
        }
    case(AGENT_STATE_DETERMINE_TURN_ORDER):
        if (agentEF == AGENT_EVENT_RECEIVED_DET_MESSAGE) {// validate opponent neg data.
            validate = ProtocolValidateNegotiationData(&enemyData); //the VALIDATE is the point in the state machine.
            if (validate == TRUE) {
                turnState = ProtocolGetTurnOrder(&myData, &enemyData); //Get the turn Order.
                if (turnState == TURN_ORDER_START) { // Won turn ordering.
                    FOturnState = FIELD_OLED_TURN_MINE; //set turn to MINE
                    FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_MINE);
                    ASM = AGENT_STATE_SEND_GUESS;
                } else if (turnState == TURN_ORDER_DEFER) { //lost turn ordering 
                    FOturnState = FIELD_OLED_TURN_THEIRS;
                    FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_THEIRS);
                    ASM = AGENT_STATE_WAIT_FOR_GUESS;
                } else if (turnState == TURN_ORDER_TIE) {// turn order tie, goes to invalid.
                    ASM = AGENT_STATE_INVALID;
                    OledClear(0); //clear the screen and update message.
                    OledDrawString(AGENT_ERROR_STRING_PARSING);
                    OledUpdate();
                }
            } else if (validate == FALSE) { //Negotiation Data is invalid
                ASM = AGENT_STATE_INVALID;
                OledClear(0);
                OledDrawString(AGENT_ERROR_STRING_NEG_DATA);
                OledUpdate();
            }
        } else if (agentEF == AGENT_EVENT_MESSAGE_PARSING_FAILED) { //from any states to INVALID.
            OledClear(0);
            OledDrawString(AGENT_ERROR_STRING_PARSING);
            OledUpdate();
        }
        break;
    case(AGENT_STATE_WAIT_FOR_GUESS):
        if (agentEF == AGENT_EVENT_RECEIVED_COO_MESSAGE && AgentGetStatus() != 0) {//if Received COO msg and friendly boats left
            FOturnState = FIELD_OLED_TURN_MINE; //set the trun to MINE.
            FieldRegisterEnemyAttack(&myField, &enemyGuessData); //register hit.
            FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_MINE); //update the oled
            ASM = AGENT_STATE_SEND_GUESS;
            return ProtocolEncodeHitMessage(outBuffer, &enemyGuessData); // send HIT message.
        } else if (AgentGetStatus() == 0 && agentEF == AGENT_EVENT_RECEIVED_COO_MESSAGE) { // if no boats left, then YOU LOST.
            FOturnState = FIELD_OLED_TURN_NONE;
            FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_NONE); //update the oled
            ASM = AGENT_STATE_LOST;
            return ProtocolEncodeHitMessage(outBuffer, &enemyGuessData); // send HIT message.
        }
        break;
    case (AGENT_STATE_WAIT_FOR_HIT):
        if (agentEF == AGENT_EVENT_RECEIVED_HIT_MESSAGE && AgentGetEnemyStatus() != 0) { //if hit msg received and enemy boats left
            ASM = AGENT_STATE_WAIT_FOR_GUESS;
            FieldUpdateKnowledge(&enemyField, &enemyGuessData);
            FOturnState = FIELD_OLED_TURN_THEIRS;
            FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_THEIRS);
        } else if (agentEF == AGENT_EVENT_RECEIVED_HIT_MESSAGE && AgentGetEnemyStatus() == 0) { // if no enemy boats left, then you win.
            ASM = AGENT_STATE_WON;
            FOturnState = FIELD_OLED_TURN_NONE;
            FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_NONE);
        }
        break;
    case(AGENT_STATE_SEND_GUESS):
        for (i = 0; i < (BOARD_GetPBClock() / 8); i++); //delay the clock, make it look like thinking.
        myGuessData.row = rand() % FIELD_ROWS;//randomly generate coordinates for enemy guessing.
        myGuessData.col = rand() % FIELD_ROWS;
        return ProtocolEncodeCooMessage(outBuffer, &myGuessData);
        break;
    case(AGENT_STATE_LOST):
        OledDrawString("YOU LOST!\n");
        OledUpdate();
        return FALSE;
    case(AGENT_STATE_WON):
        OledDrawString("YOU WON!!\n");
        OledUpdate();
        return FALSE;
    case(AGENT_STATE_INVALID):
        return FALSE; //INVALID doesnt do anything so I'll just return FALSE;
    }
    return strlen(outBuffer);
}

/**
 * StateCheck() returns a 4-bit number indicating the status of that agent's ships. The smallest
 * ship, the 3-length one, is indicated by the 0th bit, the medium-length ship (4 tiles) is the
 * 1st bit, etc. until the 3rd bit is the biggest (6-tile) ship. This function is used within
 * main() to update the LEDs displaying each agents' ship status. This function is similar to
 * Field::FieldGetBoatStates().
 * @return A bitfield indicating the sunk/unsunk status of each ship under this agent's control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetStatus(void)
{

    return FieldGetBoatStates(&myField);
}

/**
 * This function returns the same data as `AgentCheckState()`, but for the enemy agent.
 * @return A bitfield indicating the sunk/unsunk status of each ship under the enemy agent's
 *         control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetEnemyStatus(void)
{
    return FieldGetBoatStates(&enemyField);
}