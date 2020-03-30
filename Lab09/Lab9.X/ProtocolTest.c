//Xingyu Zhou, Mingyu Ding
//miding@ucsc.edu

// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Protocol.h"
#include "Agent.h"
#include "CircularBuffer.h"
#include "Leds.h"
#include "Oled.h"
#include "Buttons.h"
#include "Uart1.h"
#include "Field.h"
#include "OledDriver.h"
#include "FieldOled.h"

// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****

//for the test of hex to int
char testHexOne = 'a';
char testHexTwo = 'C';
char testHexThree = '6';
uint8_t hexConvertResult;

//for the test of checksum calculation
char testCalculateChecksumOne[] = "COO,0,8";
char testCalculateChecksumTwo[] = "DET,7321,36898";
uint8_t CalculateChecksumResult;

//for the test of encode
GuessData testGuessData[1];
NegotiationData testNegotiationData[1];
char testMessage[PROTOCOL_MAX_MESSAGE_LEN];
int encodeRerurnValue;
int stringCompareResult;
int testMessageIndex;

//for the test of decode
ProtocolParserStatus testDecodeStatus;
GuessData decodedGuessData[1];
NegotiationData decodedtestNegotiationData[1];

//for the test of generate and validate negotiation data
NegotiationData generatedtestNegotiationData[1];
NegotiationData gvalidatedtestNegotiationData[1];
int nDataValidatingCheckResult;

//for the test of get turn
NegotiationData getTurnTestNegotiationDataOne[1];
NegotiationData getTurnTestNegotiationDataTwo[1];
TurnOrder testTurnOrder;

//counters
int subCount = 0;
int allCount = 0;
// **** Declare any function prototypes here ****
uint8_t HexToUint(char hexValue);
uint8_t CalculateChecksum(char *string);

int main()
{
    BOARD_Init();



    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    printf("\r\nWelcome to the Protocol testing suite\r\n");
    printf("\n-------------------------------------------\n\n");


    /**********************
     *Test HexToUint() function
     * *********************/

    //test one
    hexConvertResult = HexToUint(testHexOne);
    if (hexConvertResult == 10) {
        subCount++;
    } else {
        subCount += 0;
    }

    //test two
    hexConvertResult = HexToUint(testHexTwo);
    if (hexConvertResult == 12) {
        subCount++;
    } else {
        subCount += 0;
    }

    //test three
    hexConvertResult = HexToUint(testHexThree);
    if (hexConvertResult == 6) {
        subCount++;
    } else {
        subCount += 0;
    }

    //print test result
    if (subCount == 3) {
        printf(" 1.HexToUint function test: PASSED! (%d/3)\n",subCount);
        allCount++; //increment the all-counter
        subCount = 0; //reset the sub counter
    } else {
        printf(" 1.HexToUint function test: FAILED! (%d/3)\n",subCount);
        allCount += 0;
        subCount = 0;
    }


    /**********************
     *Test CalculateChecksum() function
     * *********************/

    //test one
    CalculateChecksumResult = CalculateChecksum(testCalculateChecksumOne);
    if (CalculateChecksumResult == 0x4b) {
        subCount++;
    } else {
        subCount += 0;
    }

    //test two
    CalculateChecksumResult = CalculateChecksum(testCalculateChecksumTwo);
    if (CalculateChecksumResult == 0x6e) {
        subCount++;
    } else {
        subCount += 0;
    }

    //print test result
    if (subCount == 2) {
        printf(" 2.CalculateChecksum function test: PASSED! (%d/2)\n",subCount);
        allCount++; //increment the all-counter
        subCount = 0; //reset the sub counter
    } else {
        printf(" 2.CalculateChecksum function test: FAILED! (%d/2)\n",subCount);
        allCount += 0;
        subCount = 0;
    }


    /**********************
     *Test ProtocolEncodeCooMessage() function
     * *********************/


    //test one
    testGuessData->row = 0;
    testGuessData->col = 2;
    encodeRerurnValue = ProtocolEncodeCooMessage(testMessage, testGuessData);
    stringCompareResult = strcmp(testMessage, "$COO,0,2*41\n");
    if ((stringCompareResult == 0) && (encodeRerurnValue == 12)) {
        subCount++;
    } else {
        subCount += 0;
    }

    //test two
    testGuessData->row = 1;
    testGuessData->col = 6;
    ProtocolEncodeCooMessage(testMessage, testGuessData);
    stringCompareResult = strcmp(testMessage, "$COO,1,6*44\n");
    if ((stringCompareResult == 0) && (encodeRerurnValue == 12)) {
        subCount++;
    } else {
        subCount += 0;
    }

    //print test result
    if (subCount == 2) {
        printf(" 3.ProtocolEncodeCooMessage function test: PASSED! (%d/2)\n",subCount);
        allCount++; //increment the all-counter
        subCount = 0; //reset the sub counter
    } else {
        printf(" 3.ProtocolEncodeCooMessage function test: FAILED! (%d/2)\n",subCount);
        allCount += 0;
        subCount = 0;
    }

    /**********************
     *Test ProtocolEncodeHitMessage() function
     * *********************/

    //test one
    testGuessData->row = 3;
    testGuessData->col = 8;
    testGuessData->hit = 1;
    encodeRerurnValue = ProtocolEncodeHitMessage(testMessage, testGuessData);
    stringCompareResult = strcmp(testMessage, "$HIT,3,8,1*43\n");
    if ((stringCompareResult == 0) && (encodeRerurnValue == 14)) {
        subCount++;
    } else {
        subCount += 0;
    }

    //test two
    testGuessData->row = 5;
    testGuessData->col = 6;
    testGuessData->hit = 1;
    encodeRerurnValue = ProtocolEncodeHitMessage(testMessage, testGuessData);
    stringCompareResult = strcmp(testMessage, "$HIT,5,6,1*4b\n");
    if ((stringCompareResult == 0) && (encodeRerurnValue == 14)) {
        subCount++;
    } else {
        subCount += 0;
    }

    //print test result
    if (subCount == 2) {
        printf(" 4.ProtocolEncodeHitMessage function test: PASSED! (%d/2)\n",subCount);
        allCount++; //increment the all-counter
        subCount = 0; //reset the sub counter
    } else {
        printf(" 4.ProtocolEncodeHitMessage function test: FAILED! (%d/2)\n",subCount);
        allCount += 0;
        subCount = 0;
    }


    /**********************
     *Test ProtocolEncodeChaMessage() function
     * *********************/

    //test one
    testNegotiationData->encryptedGuess = 37348;
    testNegotiationData->hash = 117;
    encodeRerurnValue = ProtocolEncodeChaMessage(testMessage, testNegotiationData);
    stringCompareResult = strcmp(testMessage, "$CHA,37348,117*46\n");
    if ((stringCompareResult == 0) && (encodeRerurnValue == 18)) {
        subCount++;
    } else {
        subCount += 0;
    }

    //test two
    testNegotiationData->encryptedGuess = 36027;
    testNegotiationData->hash = 55;
    encodeRerurnValue = ProtocolEncodeChaMessage(testMessage, testNegotiationData);
    stringCompareResult = strcmp(testMessage, "$CHA,36027,55*7a\n");
    if ((stringCompareResult == 0) && (encodeRerurnValue == 17)) {
        subCount++;
    } else {
        subCount += 0;
    }

    //print test result
    if (subCount == 2) {
        printf(" 5.ProtocolEncodeChaMessage function test: PASSED! (%d/2)\n",subCount);
        allCount++; //increment the all-counter
        subCount = 0; //reset the sub counter
    } else {
        printf(" 5.ProtocolEncodeChaMessage function test: FAILED! (%d/2)\n",subCount);
        allCount += 0;
        subCount = 0;
    }

    /**********************
     *Test ProtocolEncodeDetMessage() function
     * *********************/

    //test one
    testNegotiationData->guess = 9578;
    testNegotiationData->encryptionKey = 46222;
    encodeRerurnValue = ProtocolEncodeDetMessage(testMessage, testNegotiationData);
    stringCompareResult = strcmp(testMessage, "$DET,9578,46222*66\n");
    if ((stringCompareResult == 0) && (encodeRerurnValue == 19)) {
        subCount++;
    } else {
        subCount += 0;
    }

    //test two
    testNegotiationData->guess = 32990;
    testNegotiationData->encryptionKey = 21382;
    encodeRerurnValue = ProtocolEncodeDetMessage(testMessage, testNegotiationData);
    stringCompareResult = strcmp(testMessage, "$DET,32990,21382*5e\n");
    if ((stringCompareResult == 0) && (encodeRerurnValue == 20)) {
        subCount++;
    } else {
        subCount += 0;
    }

    //print test result
    if (subCount == 2) {
        printf(" 6.ProtocolEncodeDetMessage function test: PASSED! (%d/2)\n",subCount);
        allCount++; //increment the all-counter
        subCount = 0; //reset the sub counter
    } else {
        printf(" 6.ProtocolEncodeDetMessage function test: FAILED! (%d/2)\n",subCount);
        allCount += 0;
        subCount = 0;
    }

    /**********************
     *Test ProtocolDecode() function
     * *********************/

    //test one
    testGuessData->row = 2;
    testGuessData->col = 3;
    testGuessData->hit = 1;
    encodeRerurnValue = ProtocolEncodeHitMessage(testMessage, testGuessData); //create new message

    while (testDecodeStatus != PROTOCOL_PARSED_HIT_MESSAGE) {//decode message
        testDecodeStatus = ProtocolDecode(testMessage[testMessageIndex], decodedtestNegotiationData, decodedGuessData);
        testMessageIndex++;
    }
    testMessageIndex = 0;

    //check the value
    if ((decodedGuessData->row == 2) && (decodedGuessData->col == 3)&&(decodedGuessData->hit == 1)) {
        subCount++;
    } else {
        subCount += 0;
    }


    //test two
    testNegotiationData->encryptedGuess = 62132;
    testNegotiationData->hash = 70;
    encodeRerurnValue = ProtocolEncodeChaMessage(testMessage, testNegotiationData); //create new message

    while (testDecodeStatus != PROTOCOL_PARSED_CHA_MESSAGE) {//decode message
        testDecodeStatus = ProtocolDecode(testMessage[testMessageIndex], decodedtestNegotiationData, decodedGuessData);
        testMessageIndex++;
    }
    testMessageIndex = 0;

    //check the value
    if ((decodedtestNegotiationData->encryptedGuess == 62132) && (decodedtestNegotiationData->hash == 70)) {
        subCount++;
    } else {
        subCount += 0;
    }


    //print test result
    if (subCount == 2) {
        printf(" 7.ProtocolDecode function test: PASSED! (%d/2)\n",subCount);
        allCount++; //increment the all-counter
        subCount = 0; //reset the sub counter
    } else {
        printf(" 7.ProtocolDecode function test: FAILED! (%d/2)\n",subCount);
        allCount += 0;
        subCount = 0;
    }



    /**********************
     *Test ProtocolGenerateNegotiationData() function
     * *********************/


    //test one
    srand(SWITCH_STATES()); //
    nDataValidatingCheckResult = 0;
    ProtocolGenerateNegotiationData(generatedtestNegotiationData);
    generatedtestNegotiationData->encryptedGuess++;
    nDataValidatingCheckResult = ProtocolValidateNegotiationData(generatedtestNegotiationData);

    if (nDataValidatingCheckResult == FALSE) {
        subCount++;
    } else {
        subCount += 0;
    }

    //test two
    srand(SWITCH_STATES());
    nDataValidatingCheckResult = 0;
    ProtocolGenerateNegotiationData(generatedtestNegotiationData);
    nDataValidatingCheckResult = ProtocolValidateNegotiationData(generatedtestNegotiationData);

    if (nDataValidatingCheckResult == TRUE) {
        subCount++;
    } else {
        subCount += 0;
    }


    //print test result
    if (subCount == 2) {
        printf(" 8.ProtocolGenerateNegotiationData function test: PASSED! (%d/2)\n",subCount);
        allCount++; //increment the all-counter
        printf(" 9.ProtocolValidateNegotiationData function test: PASSED! (%d/2)\n",subCount);
        allCount++; //increment the all-counter
        subCount = 0; //reset the sub counter
    } else {
        printf(" 8.ProtocolGenerateNegotiationData function test: FAILED! (%d/2)\n",subCount);
        printf(" 9.ProtocolValidateNegotiationData function test: FAILED! (%d/2)\n",subCount);
        allCount += 0;
        subCount = 0;
    }


    /**********************
     *Test ProtocolGetTurnOrder() function
     * *********************/


    //test one
    //test the ProtocolGetTurnOrder with send in two same args
    //result should be tie
    srand(SWITCH_STATES());
    ProtocolGenerateNegotiationData(getTurnTestNegotiationDataOne);
    testTurnOrder = ProtocolGetTurnOrder(getTurnTestNegotiationDataOne, getTurnTestNegotiationDataOne);

    if (testTurnOrder == TURN_ORDER_TIE) {
        subCount++;
    } else {
        subCount += 0;
    }

    //test two
    //test the ProtocolGetTurnOrder with send in two modified args
    //result should be TURN_ORDER_DEFER
    srand(SWITCH_STATES());
    ProtocolGenerateNegotiationData(getTurnTestNegotiationDataOne);
    getTurnTestNegotiationDataTwo->encryptionKey = getTurnTestNegotiationDataOne->encryptionKey + 1;
    getTurnTestNegotiationDataTwo->guess = getTurnTestNegotiationDataOne->guess + 1;
    testTurnOrder = ProtocolGetTurnOrder(getTurnTestNegotiationDataOne, getTurnTestNegotiationDataTwo);

    if (testTurnOrder == TURN_ORDER_DEFER) {
        subCount++;
    } else {
        subCount += 0;
    }

    //test three
    //test the ProtocolGetTurnOrder with send in two modified args
    //result should be TURN_ORDER_START
    srand(SWITCH_STATES());
    ProtocolGenerateNegotiationData(getTurnTestNegotiationDataOne);
    getTurnTestNegotiationDataTwo->encryptionKey = getTurnTestNegotiationDataOne->encryptionKey;
    getTurnTestNegotiationDataTwo->guess = getTurnTestNegotiationDataOne->guess + 1;
    testTurnOrder = ProtocolGetTurnOrder(getTurnTestNegotiationDataOne, getTurnTestNegotiationDataTwo);

    if (testTurnOrder == TURN_ORDER_START) {
        subCount++;
    } else {
        subCount += 0;
    }

    //print test result
    if (subCount == 3) {
        printf("10.ProtocolGetTurnOrder function test: PASSED! (%d/3)\n",subCount);
        allCount++; //increment the all-counter      
        subCount = 0; //reset the sub counter
    } else {
        printf("10.ProtocolGetTurnOrder function test: FAILED! (%d/3)\n",subCount);
        allCount += 0;
        subCount = 0;
    }


    /**********************
     *print the statistical info.
     * *********************/

    printf("\n-------------------------------------------\n");
    double passedPercent = (allCount / 10.0) * 100;
    printf("%d out of 10 tests passed (%.1f%%) \n \n", allCount, passedPercent);

    /******************************************************************************
     * Your code goes in between this comment and the preceeding one with asterisks
     *****************************************************************************/

    while (1);
}