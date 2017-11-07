// HASHER
//an encryption tool that hashes password inputs using an FNV1a (non-crypto) hashing algorithm
// v:0.1 - July 26, 2017

//include headers
#include<conio.h>
#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

//hashing function that uses the FNV1a hashing algo.
//FNV is an easy hashing algorithm and has low collision rates
unsigned long long int FNVhash(char * str) {
  unsigned int len = strlen(str);
  unsigned int fnvPrime = 0x811C9DC5; //FNV prime
  unsigned long long int hash = 0; //FNV offset basis
  unsigned int i = 0;

  for(i = 0; i < len; str++, i++) {
    hash *= fnvPrime;
    hash ^= (*str);
  }

  return hash;
}

void main() {
  clrscr();
  char pass[32];
  while(1) {
  printf("\n Enter Password : ");
  int ch;
  int x = 0;
  for(int i = 0;i < 31;i++) {
    if(i == 30) {
      pass[31] = '\0';
      break;
    }
    ch = getch();
    //sanitize the input to allow only A-Z, a-z, 0-9, '~', '@', '#', '$', '%', '^', '&', ',', '_', '-' and '.'
    if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '~' || ch == '!' || ch == '@' || ch == '#' || ch == '$' || ch == '%' || ch == '^' || ch == '&' || ch == ',' ||ch == '_' || ch == '-' || ch == '.') {
      printf("*");
      pass[i] = char(ch);
    }
    //backspace functionality
    else if(ch == '\b') {
      printf("\b");
      printf(" ");
      printf("\b");
      i -= 2;
    }
    //carriage return and newline functionality
    else if(ch == '\n' || ch == '\r') {
      pass[i] = '\0';
      break;
    }
    //garbage value
    else {
      x = 1;
      break;
    }
  }
  if(x == 1) {
    printf("\n You Entered An Illigal Character!");
  }
  //gets(password);
  printf("\n You Entered : %s", pass);
  unsigned int hashedPassword = FNVhash(pass);
  printf("\n Hashed Password : %llu", hashedPassword);
  getch();
}
}
