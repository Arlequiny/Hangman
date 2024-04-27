#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include "hangman.h"


int get_word(char secret[]){
    // check if file exists first and is readable
    FILE *fp = fopen(WORDLIST_FILENAME, "rb");
    if( fp == NULL ){
        fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
        return 1;
    }

    // get the filesize first
    struct stat st;
    stat(WORDLIST_FILENAME, &st);
    long int size = st.st_size;

    do{
        // generate random number between 0 and filesize
        long int random = (rand() % size) + 1;

        // seek to the random position of file
        fseek(fp, random, SEEK_SET);

        // get next word in row ;)
        int result = fscanf(fp, "%*s %20s", secret);
        if( result != EOF )
            break;
    }while(1);

    fclose(fp);

    return 0;
}


int is_word_guessed(const char secret[], const char letters_guessed[]) {
    int findFlag = 0;
    for (int i = 0; i < strlen(secret); i++) {
        findFlag = 0;
        for (int j = 0; j < strlen(letters_guessed); j++) {
            if (secret[i] == letters_guessed[j]) {
                findFlag = 1;
                break;
            }
        }
        if (findFlag == 0) {
            return 0;
        }   
    }
    return 1;
}


void get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[]) {
    for (int i = 0; i < strlen(secret); i++) {
        for (int j = 0; j < strlen(letters_guessed); j++) {
            if (secret[i] == letters_guessed[j]) {
                guessed_word[i]=secret[i];
                break;
            } else {
                guessed_word[i]='_';
            }
        }
    } 
}


void get_available_letters(const char letters_guessed[], char available_letters[]){
    int findFlag = 0;
    char alph[27] = "abcdefghijklmnopqrstuvwxyz";
    int l = 0;
    for (int i = 0, l = 0; i < strlen(alph); i++) {
        findFlag = 0;
        for (int j = 0; j < strlen(letters_guessed); j++) {
            if(letters_guessed[j] == alph[i]) {
                findFlag = 1;
                break;
            }
        }
        if(findFlag == 0) {
            available_letters[l]=alph[i];
            l++;
        }
    }
    available_letters[l]='\0';
}


int find_letter(const char letter, const char string[]) {
    for (int i = 0; i < strlen(string); i++)
    {
        if(string[i] == letter) {
            return 1;
        }
    }
    return 0;
}


void hangman(const char secret[]){
    int uses = 8;
    int secretlen = strlen(secret);
    char letters_guessed[30] = "";
    int lg_i = 0;
    char available_letters[30] = "";
    char guessed_word[20] = "";

    int letter_flag = 0;
    int string_flag = 0;

    printf("Welcome to the game, Hangman!\n I am thinking of a word that is %i letters long.\n", secretlen);

    while (!is_word_guessed(secret, letters_guessed) && uses > 0) {
        letter_flag = 0;
        printf("\n\n-----------------------------\n");
        printf("You have %d guesses left.\n", uses);
        get_available_letters(letters_guessed, available_letters);

        printf("Available letters: %s\n", available_letters);
        printf("Please guess a letter: ");
        char tmp[30];
        fgets(tmp, 30, stdin);
        if (tmp[1] == '\n') {
            char letter = '\0';
            if(tmp[0]>=97 && tmp[0] <= 122) {

                letter = tmp[0];

                if(find_letter(letter,letters_guessed)){
                    printf("Oops! You've already guessed that letter: \n");
                    letter_flag = 1;
                } else {
                    letters_guessed[lg_i]=tmp[0];
                    lg_i++;
                }


            } else if (tmp[0] >= 65 && tmp[0] <= 90){

                letter = tmp[0] + 32;
                if(find_letter(letter,letters_guessed)){
                    printf("Oops! You've already guessed that letter: \n");
                    letter_flag = 1;
                } else {
                    tmp[0]+=32;
                    letters_guessed[lg_i]=tmp[0];
                    lg_i++;
                }


            } else {
                printf("Oops! '%c' is not a valid letter. ", tmp[0]);
            }

            get_guessed_word(secret, letters_guessed, guessed_word);
            
            if(letter_flag == 0){
                if(find_letter(letter, secret)){

                    printf("Good guess: ");

                } else {

                    printf("Oops! That letter is not in my word: ");
                    uses--;
                }
            }

            for(int i = 0; i < strlen(guessed_word); i++){
                printf("%c ", guessed_word[i]);
            }

        } else {

            string_flag = 1;
            for(int i = 0; i < strlen(tmp); i++){
                tmp[i] = tolower(tmp[i]);
            }
            if(is_word_guessed(secret,tmp)){
                printf("\nCongratulations, you won!");
            } else {
                printf("\nSorry, bad guesses. The word was %s.\n", secret);
            }
            break;
        }
        //break;
    }
    if(string_flag == 0) {
        if(uses == 0){
            printf("\nSorry, you ran out of guesses. The word was %s.\n", secret);
        } else {
            printf("\nCongratulations, you won!");
        }
    }
}