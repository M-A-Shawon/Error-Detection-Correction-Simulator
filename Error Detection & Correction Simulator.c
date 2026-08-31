#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void hammingSimulator();
void crcSimulator();

void generateHamming(int data[], int h[]);
int calculateSyndrome(int h[]);
void displayBits(int arr[], int size);

void xorOperation(char a[], char b[], int pos, int genLen);
void calculateCRC(char data[], char generator[], char remainder[]);
int checkCRC(char codeword[], char generator[]);


int main()
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("====================================================\n");
        printf("       ERROR DETECTION & CORRECTION SIMULATOR\n");
        printf("====================================================\n");
        printf("1. Hamming Code (7,4)\n");
        printf("2. CRC Error Detection\n");
        printf("3. Exit\n");
        printf("====================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                hammingSimulator();
                break;

            case 2:
                crcSimulator();
                break;

            case 3:
                printf("\nThank you for using the simulator!\n");
                return 0;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}

void hammingSimulator()
{
    int data[4];
    int hamming[7];
    int received[7];

    int errorPosition;
    int detectedError;

    printf("====================================================\n");
    printf("              HAMMING CODE (7,4)\n");
    printf("====================================================\n");

    printf("Enter 4-bit data: ");

    for (int i = 0; i < 4; i++)
    {
        scanf("%d", &data[i]);

        if (data[i] != 0 && data[i] != 1)
        {
            printf("Invalid input! Only 0 or 1 allowed.\n");
            return;
        }
    }

    generateHamming(data, hamming);

    printf("\nOriginal Data     : ");
    displayBits(data, 4);

    printf("Generated Hamming : ");
    displayBits(hamming, 7);


    for (int i = 0; i < 7; i++)
    {
        received[i] = hamming[i];
    }


    printf("\n----------------------------------------------------\n");
    printf("ERROR INTRODUCTION\n");
    printf("----------------------------------------------------\n");

    printf("Enter error position (1-7)\n");
    printf("Enter 0 for no error: ");

    scanf("%d", &errorPosition);

    if (errorPosition < 0 || errorPosition > 7)
    {
        printf("Invalid position!\n");
        return;
    }


    if (errorPosition != 0)
    {
        received[errorPosition - 1] =
            1 - received[errorPosition - 1];

        printf("\nError introduced at position: %d\n",
               errorPosition);
    }
    else
    {
        printf("\nNo error introduced.\n");
    }

    printf("Received Data      : ");
    displayBits(received, 7);



    detectedError = calculateSyndrome(received);


    printf("\n----------------------------------------------------\n");
    printf("ERROR ANALYSIS\n");
    printf("----------------------------------------------------\n");

    if (detectedError == 0)
    {
        printf("Result: NO ERROR DETECTED.\n");
    }
    else
    {
        printf("Result: ERROR DETECTED.\n");
        printf("Error Position: %d\n", detectedError);


        received[detectedError - 1] =
            1 - received[detectedError - 1];

        printf("\nCorrected Hamming Code: ");
        displayBits(received, 7);
    }


    printf("\nRecovered Original Data: ");

    printf("%d%d%d%d\n",
           received[2],
           received[4],
           received[5],
           received[6]);

    printf("\n====================================================\n");
}


void generateHamming(int data[], int h[])
{
    /*
       Hamming (7,4) positions:

       Position:  1 2 3 4 5 6 7
                  P P D P D D D

       P = Parity
       D = Data
    */

    h[2] = data[0];
    h[4] = data[1];
    h[5] = data[2];
    h[6] = data[3];


    h[0] =
        (h[2] + h[4] + h[6]) % 2;

    h[1] =
        (h[2] + h[5] + h[6]) % 2;

    h[3] =
        (h[4] + h[5] + h[6]) % 2;
}


int calculateSyndrome(int h[])
{
    int s1, s2, s4;
    int errorPosition;


    s1 =
        (h[0] +
         h[2] +
         h[4] +
         h[6]) % 2;


    s2 =
        (h[1] +
         h[2] +
         h[5] +
         h[6]) % 2;



    s4 =
        (h[3] +
         h[4] +
         h[5] +
         h[6]) % 2;



    errorPosition =
        s1 +
        (2 * s2) +
        (4 * s4);

    return errorPosition;
}


void displayBits(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d", arr[i]);
    }

    printf("\n");
}


void crcSimulator()
{
    char data[200];
    char generator[100];

    char remainder[300];
    char codeword[300];
    char received[300];

    int dataLength;
    int generatorLength;
    int codewordLength;

    int errorPosition;

    printf("====================================================\n");
    printf("              CRC ERROR DETECTION\n");
    printf("====================================================\n");


    printf("Enter data bits: ");
    scanf("%s", data);


    printf("Enter generator polynomial: ");
    scanf("%s", generator);


    dataLength = strlen(data);
    generatorLength = strlen(generator);


    for (int i = 0; i < dataLength; i++)
    {
        if (data[i] != '0' && data[i] != '1')
        {
            printf("Invalid data! Only 0 and 1 allowed.\n");
            return;
        }
    }


    for (int i = 0; i < generatorLength; i++)
    {
        if (generator[i] != '0' &&
            generator[i] != '1')
        {
            printf("Invalid generator!\n");
            return;
        }
    }


    if (generatorLength < 2)
    {
        printf("Generator must contain at least 2 bits.\n");
        return;
    }


    calculateCRC(data, generator, remainder);


    printf("\n----------------------------------------------------\n");
    printf("CRC CALCULATION\n");
    printf("----------------------------------------------------\n");

    printf("Original Data : %s\n", data);

    printf("Generator     : %s\n", generator);

    printf("CRC Remainder : %s\n",
           remainder);


    strcpy(codeword, data);

    strcat(codeword, remainder);


    printf("Transmitted Codeword: %s\n",
           codeword);


    strcpy(received, codeword);

    codewordLength = strlen(received);


    printf("\n----------------------------------------------------\n");
    printf("ERROR INTRODUCTION\n");
    printf("----------------------------------------------------\n");

    printf("Enter error position (1-%d)\n",
           codewordLength);

    printf("Enter 0 for no error: ");

    scanf("%d", &errorPosition);


    if (errorPosition < 0 ||
        errorPosition > codewordLength)
    {
        printf("Invalid error position!\n");
        return;
    }


    if (errorPosition != 0)
    {
        int index = errorPosition - 1;

        if (received[index] == '0')
            received[index] = '1';
        else
            received[index] = '0';

        printf("\nError introduced at position: %d\n",
               errorPosition);
    }
    else
    {
        printf("\nNo error introduced.\n");
    }


    printf("Received Codeword: %s\n",
           received);

    printf("\n----------------------------------------------------\n");
    printf("CRC ERROR ANALYSIS\n");
    printf("----------------------------------------------------\n");

    if (checkCRC(received, generator))
    {
        printf("Result: NO ERROR DETECTED.\n");
    }
    else
    {
        printf("Result: ERROR DETECTED!\n");
    }

    printf("\n====================================================\n");
}


void calculateCRC(char data[],
                  char generator[],
                  char remainder[])
{
    char temp[300];

    int dataLength =
        strlen(data);

    int generatorLength =
        strlen(generator);


    strcpy(temp, data);


    for (int i = 0;
         i < generatorLength - 1;
         i++)
    {
        temp[dataLength + i] = '0';
    }

    temp[dataLength +
         generatorLength - 1] = '\0';


    for (int i = 0;
         i < dataLength;
         i++)
    {
        if (temp[i] == '1')
        {
            xorOperation(
                temp,
                generator,
                i,
                generatorLength
            );
        }
    }

    int remainderLength =
        generatorLength - 1;

    int start =
        dataLength;

    for (int i = 0;
         i < remainderLength;
         i++)
    {
        remainder[i] =
            temp[start + i];
    }

    remainder[remainderLength] =
        '\0';
}


void xorOperation(char a[],
                  char b[],
                  int pos,
                  int genLen)
{
    for (int i = 0;
         i < genLen;
         i++)
    {
        if (a[pos + i] == b[i])
        {
            a[pos + i] = '0';
        }
        else
        {
            a[pos + i] = '1';
        }
    }
}


int checkCRC(char codeword[],
             char generator[])
{
    char temp[300];

    int codeLength =
        strlen(codeword);

    int generatorLength =
        strlen(generator);


    strcpy(temp, codeword);


    for (int i = 0;
         i <= codeLength - generatorLength;
         i++)
    {
        if (temp[i] == '1')
        {
            xorOperation(
                temp,
                generator,
                i,
                generatorLength
            );
        }
    }


    for (int i = codeLength -
                   generatorLength + 1;
         i < codeLength;
         i++)
    {
        if (temp[i] != '0')
        {
            return 0;
        }
    }

    return 1;
}
