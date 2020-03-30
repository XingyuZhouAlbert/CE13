/*Battle Boats Protocol
 * Xingyu Zhou
 * Mingyu Ding
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

// **** Set any macros or preprocessor directives here ****
#define EIGHT_BITS_MASK 0xFF
#define LEAST_SIGNIFICANT_BIT_MASK 0x01
#define ONE_BYTE 8

// **** Declare any data types here ****

//enum includes states

typedef enum {
    WAITING,
    RECORDING,
    FIRST_CHECKSUM_HALF,
    SECOND_CHECKSUM_HALF,
    NEWLINE
} ParsingStates;

typedef struct {
    char protocalMessage[PROTOCOL_MAX_MESSAGE_LEN];
    int currentIndex;
    ParsingStates currentState;
    ProtocolParserStatus currentParsingState;
    uint8_t checksumData;
} ProtocolDataStruct;

typedef struct {
    int CheckdumCounter;
    uint8_t checksumResult;
} ChecksumdataStruct;

typedef struct {
    uint8_t guessUpper;
    uint8_t guessLower;
    uint8_t keyUpper;
    uint8_t keyLower;
} nDataGenerate;

typedef struct {
    char *messageID;
    char *firstData;
    char *secondData;
    char *hitData;
    char *checksum;
} extractString;


// **** Define any module-level, global, or external variables here ****
static ChecksumdataStruct checksumData;
static char payloadMessage[PROTOCOL_MAX_PAYLOAD_LEN];
static ProtocolDataStruct protocalData;
static uint8_t convertResult;
static uint8_t convetedValue;
static extractString extractStringData;
static nDataGenerate nDataGenerateData;
static uint32_t valadateEncryptedGuess;
static uint32_t valadateHash;
static uint32_t getTurnOrder;
static TurnOrder getTurnOrderResult;
// **** Declare any function prototypes here ****
uint8_t HexToUint(char hexValue);
uint8_t CalculateChecksum(char *string);

/**************************************************
                FUNCTIONS
 ****************************************************/
int ProtocolEncodeCooMessage(char *message, const GuessData *data)
{
    sprintf(payloadMessage, PAYLOAD_TEMPLATE_COO, data->row, data->col); //store date into COO message template
    sprintf(message, MESSAGE_TEMPLATE, payloadMessage, CalculateChecksum(payloadMessage)); ////store date into COO message template with the checksum
    return strlen(message);
}

int ProtocolEncodeHitMessage(char *message, const GuessData *data)
{
    sprintf(payloadMessage, PAYLOAD_TEMPLATE_HIT, data->row, data->col, data->hit); //store date into HIT message template
    sprintf(message, MESSAGE_TEMPLATE, payloadMessage, CalculateChecksum(payloadMessage)); //store date into HIT message template with the checksum
    return strlen(message);
}

int ProtocolEncodeChaMessage(char *message, const NegotiationData *data)
{
    sprintf(payloadMessage, PAYLOAD_TEMPLATE_CHA, data->encryptedGuess, data->hash); //store date into CHA message template
    sprintf(message, MESSAGE_TEMPLATE, payloadMessage, CalculateChecksum(payloadMessage)); //store date into CHA message template with the checksum
    return strlen(message);
}

int ProtocolEncodeDetMessage(char *message, const NegotiationData *data)
{
    sprintf(payloadMessage, PAYLOAD_TEMPLATE_DET, data->guess, data->encryptionKey); //store date into DET message template
    sprintf(message, MESSAGE_TEMPLATE, payloadMessage, CalculateChecksum(payloadMessage)); //store date into DET message template with the checksum
    return strlen(message);
}

ProtocolParserStatus ProtocolDecode(char in, NegotiationData *nData, GuessData *gData)
{
    switch (protocalData.currentState) {


        /******************
            WAITING
         ******************/
    case(WAITING):
        if (in != '$') {
            protocalData.currentState = WAITING; //waiting for the $
        } else if (in == '$') {
            protocalData.currentIndex = 0; //index = 0
            protocalData.currentState = RECORDING; //next state is RECORDING
            protocalData.currentParsingState = PROTOCOL_PARSING_GOOD; //return GOOD
        }
        break;

        /******************
         RECORDING
         ******************/
    case(RECORDING):
        if (in != '*') {//check if the char is not *
            protocalData.protocalMessage[protocalData.currentIndex] = in; //Sentence[index] = Input
            protocalData.currentIndex++; //increment index
            protocalData.currentParsingState = PROTOCOL_PARSING_GOOD; //return GOOD
        } else if (in == '*') {//check if the char is *
            protocalData.currentState = FIRST_CHECKSUM_HALF; //next state is FIRST_CHECKSUM_HALF
            protocalData.currentParsingState = PROTOCOL_PARSING_GOOD; //return GOOD
        }
        break;

        /******************
         FIRST_CHECKSUM_HALF
         ******************/
    case(FIRST_CHECKSUM_HALF):
        convetedValue = HexToUint(in); // convert to hex for validation check.

        if ((convetedValue <= 15) && (convetedValue >= 0)) {//check if the hex value is valid
            protocalData.checksumData = convetedValue << 4; //store as the upper 4 bits of its checksum
            protocalData.currentState = SECOND_CHECKSUM_HALF; //next state is SECOND_CHECKSUM_HALF
            protocalData.currentParsingState = PROTOCOL_PARSING_GOOD; //return GOOD
        } else {
            protocalData.currentState = WAITING; //reset state to WAITING
            protocalData.currentParsingState = PROTOCOL_PARSING_FAILURE; //return FAILURE
        }
        break;


        /******************
         SECOND_CHECKSUM_HALF
         ******************/
    case(SECOND_CHECKSUM_HALF):

        convetedValue = HexToUint(in); // convert to hex for validation check.
        if ((convetedValue <= 15) && (convetedValue >= 0)) {//check if the hex value is valid
            protocalData.checksumData = protocalData.checksumData + convetedValue; //combine with the upper 4 bits stored before
            checksumData.checksumResult = CalculateChecksum(protocalData.protocalMessage); //calculate the checksum use function
            if (protocalData.checksumData == checksumData.checksumResult) {//check is the checksum is correct
                protocalData.protocalMessage[protocalData.currentIndex] = '\0'; //index = 0
                protocalData.currentState = NEWLINE; //next state is NEWLINE
                protocalData.currentParsingState = PROTOCOL_PARSING_GOOD; //return GOOD
            } else {
                protocalData.currentState = WAITING; //reset state to WAITING
                protocalData.currentParsingState = PROTOCOL_PARSING_FAILURE; //return FAILURE
            }
        } else {
            protocalData.currentState = WAITING; //reset state to WAITING
            protocalData.currentParsingState = PROTOCOL_PARSING_FAILURE; //return FAILURE
        }
        break;

        /******************
         NEWLINE
         ******************/
    case(NEWLINE):
        if (in == '\n') {//check if the input is '\n'

            //Check if the MSGID is COO
            if ((protocalData.protocalMessage[0] == 'C')&&
                    (protocalData.protocalMessage[1] == 'O')&&
                    (protocalData.protocalMessage[2] == 'O')) {

                //tokenize the message
                extractStringData.messageID = strtok(protocalData.protocalMessage, ",");
                extractStringData.firstData = strtok(NULL, ",");
                extractStringData.secondData = strtok(NULL, "*");
                extractStringData.checksum = strtok(NULL, "\n");

                //store the data as integer
                gData->row = atoi(extractStringData.firstData);
                gData->col = atoi(extractStringData.secondData);

                protocalData.currentState = WAITING; //reset state to WAITING
                protocalData.currentParsingState = PROTOCOL_PARSED_COO_MESSAGE; //return message category
            }//Check if the MSGID is HIT
            else if ((protocalData.protocalMessage[0] == 'H')&&
                    (protocalData.protocalMessage[1] == 'I')&&
                    (protocalData.protocalMessage[2] == 'T')) {

                //tokenize the message
                extractStringData.messageID = strtok(protocalData.protocalMessage, ",");
                extractStringData.firstData = strtok(NULL, ",");
                extractStringData.secondData = strtok(NULL, ",");
                extractStringData.hitData = strtok(NULL, "*");
                extractStringData.checksum = strtok(NULL, "\n");

                //store the data as integer
                gData->row = atoi(extractStringData.firstData);
                gData->col = atoi(extractStringData.secondData);
                gData->hit = atoi(extractStringData.hitData);
                protocalData.currentState = WAITING; //reset state to WAITING
                protocalData.currentParsingState = PROTOCOL_PARSED_HIT_MESSAGE; //return message category
            }//Check if the MSGID is CHA
            else if ((protocalData.protocalMessage[0] == 'C')&&
                    (protocalData.protocalMessage[1] == 'H')&&
                    (protocalData.protocalMessage[2] == 'A')) {

                //tokenize the message
                extractStringData.messageID = strtok(protocalData.protocalMessage, ",");
                extractStringData.firstData = strtok(NULL, ",");
                extractStringData.secondData = strtok(NULL, "*");
                extractStringData.checksum = strtok(NULL, "\n");

                //store the data as integer
                nData->encryptedGuess = atoi(extractStringData.firstData);
                nData->hash = atoi(extractStringData.secondData);

                protocalData.currentState = WAITING; //reset state to WAITING
                protocalData.currentParsingState = PROTOCOL_PARSED_CHA_MESSAGE; //return message category
            }//Check if the MSGID is DET
            else if ((protocalData.protocalMessage[0] == 'D')&&
                    (protocalData.protocalMessage[1] == 'E')&&
                    (protocalData.protocalMessage[2] == 'T')) {

                //tokenize the message
                extractStringData.messageID = strtok(protocalData.protocalMessage, ",");
                extractStringData.firstData = strtok(NULL, ",");
                extractStringData.secondData = strtok(NULL, "*");
                extractStringData.checksum = strtok(NULL, "\n");

                //store the data as integer
                nData->guess = atoi(extractStringData.firstData);
                nData->encryptionKey = atoi(extractStringData.secondData);

                protocalData.currentState = WAITING; //reset state to WAITING
                protocalData.currentParsingState = PROTOCOL_PARSED_DET_MESSAGE; //return message category
            } else {
                protocalData.currentState = WAITING; //reset state to WAITING
                protocalData.currentParsingState = PROTOCOL_PARSING_FAILURE; //return FAILURE
            }
        } else {
            protocalData.currentState = WAITING; //reset state to WAITING
            protocalData.currentParsingState = PROTOCOL_PARSING_FAILURE; //return FAILURE
        }
        break;
    }
    return protocalData.currentParsingState; //return the currentParsingState
}

void ProtocolGenerateNegotiationData(NegotiationData *data)
{
    data->guess = (rand() & 0xFFFF); //creating first random number
    data->encryptionKey = (rand() & 0xFFFF); //creating second random number
    data->encryptedGuess = data->guess ^ data->encryptionKey; //XOR to get the encrypted guess

    //separate the two 16bits in to four 8bits 
    nDataGenerateData.guessLower = data->guess & EIGHT_BITS_MASK;
    nDataGenerateData.guessUpper = data->guess >> ONE_BYTE;
    nDataGenerateData.keyLower = data->encryptionKey & EIGHT_BITS_MASK;
    nDataGenerateData.keyUpper = data->encryptionKey >> ONE_BYTE;

    //an 8-bit value that is the XOR() of all of the bytes
    //both the guess and the encryptionKey
    data->hash =  nDataGenerateData.guessUpper;
    data->hash ^= nDataGenerateData.guessLower;
    data->hash ^= nDataGenerateData.keyLower;
    data->hash ^= nDataGenerateData.keyUpper;
}

uint8_t ProtocolValidateNegotiationData(const NegotiationData *data)
{
    valadateEncryptedGuess = data->guess ^ data->encryptionKey; //calculate the encrypted guess

    //separate the two 16bits in to four 8bits 
    nDataGenerateData.guessLower = data->guess & EIGHT_BITS_MASK;
    nDataGenerateData.guessUpper = data->guess >> ONE_BYTE;
    nDataGenerateData.keyLower = data->encryptionKey & EIGHT_BITS_MASK;
    nDataGenerateData.keyUpper = data->encryptionKey >> ONE_BYTE;

    //an 8-bit value that is the XOR() of all of the bytes
    //both the guess and the encryptionKey
    valadateHash =  nDataGenerateData.guessUpper;
    valadateHash ^= nDataGenerateData.guessLower;
    valadateHash ^= nDataGenerateData.keyLower;
    valadateHash ^= nDataGenerateData.keyUpper;

    //if the calculated data is same as the data stored before
    if ((valadateEncryptedGuess == data->encryptedGuess) && (valadateHash == data->hash)) {
        return TRUE; //return TRUE
    } else {
        return FALSE; //otherwise return FALSE
    }
}

TurnOrder ProtocolGetTurnOrder(const NegotiationData *myData, const NegotiationData *oppData)
{
    getTurnOrderResult = TURN_ORDER_TIE; //reset to TURN_ORDER_TIE

    //XOR() of the 'encryptionKey' used by both agents and mask left the least-significant bit
    getTurnOrder = (myData->encryptionKey ^ oppData->encryptionKey) & LEAST_SIGNIFICANT_BIT_MASK;

    //if it's a 1 the player with the largest 'guess' goes first
    if (getTurnOrder == 1) {
        if (myData->guess > oppData->guess) {
            getTurnOrderResult = TURN_ORDER_START; //store the Turn order in getTurnOrderResult
        } else if (myData->guess < oppData->guess) {
            getTurnOrderResult = TURN_ORDER_DEFER; //store the Turn order in getTurnOrderResult
        }
    }//if it's a 0, the agent with the smallest 'guess' goes first
    else if (getTurnOrder == 0) {
        if (myData->guess < oppData->guess) {
            getTurnOrderResult = TURN_ORDER_START; //store the Turn order in getTurnOrderResult
        } else if (myData->guess > oppData->guess) {
            getTurnOrderResult = TURN_ORDER_DEFER; //store the Turn order in getTurnOrderResult
        }

    }//otherwise a tie is indicated with TURN_ORDER_TIE
    else {
        getTurnOrderResult = TURN_ORDER_TIE; //store the Turn order in getTurnOrderResult
    }
    return getTurnOrderResult; //return the TurngetTurnOrderResult
}
/**************************************************
                    HELPER FUNCTIONS
 ****************************************************/

//Helper Function: Convert Hex Char to Int;

uint8_t HexToUint(char hexData)
{
    if (hexData >= 'a' && hexData <= 'f') {//check if the hex value is a to f;
        convertResult = hexData - 87; //subtract 87 to get the integer value 

    } else if (hexData >= 'A' && hexData <= 'F') {//check if the hex value is A to F;
        convertResult = hexData - 55; //subtract 55 to get the integer value 

    } else if (hexData >= '0' && hexData <= '9') {//check if the hex value is 0 to 9;
        convertResult = hexData - 48; //subtract 44 to get the integer value 
    }
    return convertResult; //return the result of conversion
}


//Helper Function: Calculate the Checksum

uint8_t CalculateChecksum(char *string)
{
    checksumData.CheckdumCounter = 0; //reset the counter to 0
    checksumData.checksumResult = 0;
    while (string[checksumData.CheckdumCounter] != NULL && //check if the char is not NULL or reach '*' char
            string[checksumData.CheckdumCounter] != '*') {
        checksumData.checksumResult ^= string[checksumData.CheckdumCounter]; //XOR the char
        checksumData.CheckdumCounter++; //increment the counter
    }
    return checksumData.checksumResult; //return the result after XORd all chars
}