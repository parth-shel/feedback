// ENCRYPTO Hash Library
//an encryption tool that hashes password inputs using an FNV1a (non-crypto) hashing algorithm
//
// parth_shel
// v:1.2 - July 26, 2017

//hashing function that uses the FNV1a hashing algo.
//FNV is an easy hashing algorithm and has low collision rates
unsigned long long int FNVhash(char * str) {
  unsigned int len = strlen(str);
  unsigned int fnvPrime = 0x811C9DC5; //FNV prime
  unsigned long long int hash = 0; //FNV offset basis

  for(unsigned int i = 0; i < len; str++, i++) {
    hash *= fnvPrime;
    hash ^= (*str);
  }

  return hash;
}

//function to get password input from console while masking the entered string with *s
void getPassword(char * pass) {
  int ch;
  int flag = 0;
  for(int i = 0;i < 31;i++) {
    if(i == 30) {
      pass[31] = '\0';
      printf("\n");
      break;
    }
    ch = getch();
    //sanitize the input to allow only A-Z, a-z, 0-9 and special characters
    if(ch >= ' ' && ch <= '~') {
      printf("*"); //comment this section if you don't want to mask the password
      pass[i] = char(ch);
    }
    //backspace functionality
    else if(ch == '\b' && i == 0) {
      /*printf("\b");
      printf(" ");
      printf("\b");*/
      i -= 1;
    }
    else if(ch == '\b') {
      printf("\b");
      printf(" ");
      printf("\b");
      i -= 2;
    }
    //carriage return and newline functionality
    else if((ch == '\n' || ch == '\r') && i < 7) {
      flag = 1;
      break;
    }
    else if(ch == '\n' || ch == '\r') {
      printf("\n");
      pass[i] = '\0';
      break;
    }
    //garbage value
    else {
      flag = 1;
      break;
    }
  }
  if(flag == 1) {
    pass = NULL; //set pass to null if any illegal character was entered
  }
}
