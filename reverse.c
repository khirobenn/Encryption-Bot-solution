#include <stdio.h>
#include <stdlib.h>

#define SIZE_TAB_OF_BINARY 216

// In this function we will find the difference between the address of the first byte and the byte of our character.
int findIndex(char c, char allChars[]){
    for (int i = 0; i < 64; i++){
        if(c == allChars[i])
            return i;
    }
}

void charToBinary(char c, int tab[], int start, char allChars[]){

    int tabToReverse[6] = {0, 0, 0, 0, 0, 0};
    int entier = findIndex(c, allChars); // After founding the difference, we can convert the number to a binary value.
    
    // As we the max value can return findIndex is 63 which can be reprented in 6 bits, so we will convert this number to 6 bits binary value.
    for(int i=0; i<6; i++){
        tabToReverse[i] = entier%2;
        entier = entier/2;
        if(!entier)
            break;
    }

    for(int j=start, i=6; i--, j<start+6; j++){
        tab[j] = tabToReverse[i];
    }
}

char binaryToChar(int tabOfBinary[], int start){
    int two_power_7 = 128; // 2^7 = 128 (cause we will start by the most significant bit).
    int toReturn = 0; 
    for(int i=start; i<start+8; i++){
        toReturn = toReturn + two_power_7*tabOfBinary[i];
        two_power_7 = two_power_7/2;
    }
    return (char)toReturn; // we return the char which has the ascii value we calculated
}

int main(){
    int tabOfBinary[SIZE_TAB_OF_BINARY]; // This is the array we will fill with the values of 0 and 1, it's size is 216.
    char allChars[] = "RSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQabdeefghijklmnopqrstuvwxyz"; // This is the representation of the variables in the memory in the function "FUN_001013e9", it's size is (63 = 2^6 - 1).
    FILE* f = fopen("flag.enc", "r"); 
    char stringToReverse[37];
    fgets(stringToReverse, 37, f); // reading the text of the flag file

    // We will do a for loop to fill all the array tabOfBinary
    // We can see that 36*6 = 216 which is the size of the array
    for(int i=0; i<36; i++){
        charToBinary(stringToReverse[i], tabOfBinary, 6*i, allChars);
    }

    // After filling the array we will take 8 bits by 8 bits and convert it to decimal then to a char and print it.
    // We can see that 26*8 = 216 which is the size if the array
    for(int i=0; i<27; i++){
        printf("%c", binaryToChar(tabOfBinary, 8*i));
    }

    fclose(f);
    return 0;
}

// This is all, I hope you undertood my solution. If not, you can contact me on discord : _khireddine_benmeziane

// Sorry for my english too ;)