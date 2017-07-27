// user database building script

#include<conio.h>
#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include"encrypto.h"

void main() {
  clrscr();
  printf("\t\t\t   User Database Builder\n");
  printf("\n new users will be added to the database (UserData.csv)\n until zero (0) is entered into the roll number field.\n");
  unsigned long int rollnumber;
  char password[32];
  unsigned long long int hashedPassword;
  do {
    printf("\n Enter Roll Number : ");
    cin >> rollnumber;

    if(rollnumber == 0) {
      break;
    }
    printf(" Enter Password : ");
    getPassword(password);
    if(password != NULL && rollnumber > 0) {
      hashedPassword = FNVhash(password);
      FILE * file = fopen("UserData.csv", "r");
    	if(file == NULL) {
    		file = fopen("UserData.csv", "w");
    		fprintf(file, "Roll Number,Password (FNV hashed),Feedback Recorded\n");
    	}
    	else {
    		file = fopen("UserData.csv", "a");
    	}
      fprintf(file, "%lu,", rollnumber);
      fprintf(file, "%llu,", hashedPassword);
      fprintf(file, "NO\n");
      fclose(file);
      printf("\n DONE!");
    }
  } while(password != NULL && rollnumber > 0);
}