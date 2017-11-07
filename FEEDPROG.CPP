//	feedProg

//this program assumes that there are no bogus values
//or redundancies in the users database (UserData.csv)
//this file should include 3 columns for roll nummber, password and record status
//NOTE: this program assumes that the field for roll number is a primary key to the table
//NOTE: record status is a binary field that records if that user has submitted their feedback (YES/NO)

//include headers
#include<conio.h>
#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

//struct for user database
struct node {
	//for user -password matching
	unsigned long int rNum;
	char passwd[32];
	int record; //save record state for each unique roll number to avoid redundancies
	node * next;
} *head, *ptr;

//function to read from an existing user database and store it into a linked list
void readUserData() {
	head = NULL;
	FILE * file = fopen("UserData.csv", "r");
	if(file == NULL) {
		return;
	}
 	ptr = new node;
	int counter = 0;
	char * token;
	char buffer[256];
	unsigned long int r;
	int rec;
	char p[32];
	while(fgets(buffer, 256, (FILE*) file)) {

		token = strtok(buffer, ",");
		r = atoi(token);

		token = strtok(NULL, ",");
		if(token != NULL) {
			strcpy(p, token);
		}

		token = strtok(NULL, ",");
		if(strcmp(token, "YES\n") == 0) {
			rec = 1;
		}
		else if(strcmp(token, "NO\n") == 0) {
			rec = 0;
		}

		ptr = new node;
		if(counter == 1) {
			ptr->rNum = r;
			strcpy(ptr->passwd, p);
			ptr->record = rec;
			ptr->next = NULL;
			head = ptr;
		}
		else if(counter > 1) {
			ptr->rNum = r;
			strcpy(ptr->passwd, p);
			ptr->record = rec;
			ptr->next = head;
			head = ptr;
		}
		counter += 1;
	}

	fclose(file);
	return;
}

//function to save user data to database
void saveUserData() {
	FILE * file = fopen("UserData.csv", "w");
	if(file == NULL) { //file error
		return;
	}
	fprintf(file, "Roll Number,Password,Feedback Recorded\n");
	ptr = head;
	while(ptr != NULL) {
		fprintf(file, "%d,", ptr->rNum);
		fprintf(file, "%s,", ptr->passwd);
		if(ptr->record == 1) {
			fprintf(file, "YES\n");
		}
		else if(ptr->record == 0) {
			fprintf(file, "NO\n");
		}
	 	ptr = ptr->next;
	}
	fclose(file);
	return;
}

//function to check if a given user has recorded their feedback
//by searching the given roll number against the list
//returns 0 if the user hasn't recorded their feedback, 1 otherwise
//and -1 if the user doesn't exist in the database
int checkList(unsigned int long num) {
	ptr = head;
	while(ptr != NULL) {
		if(ptr->rNum == num && ptr->record == 1) { //user has recorded their feedback already
			return 1;
		}
		else if(ptr->rNum == num && ptr->record == 0) { //user hasn't recorded their feedback yet
			return 0;
		}
		ptr = ptr->next;
	}
	return -1; //user doesn't exist in the database
}

//function to check roll number agains password for user-password matching
//returns 1 password matches, 0 if password doesn't match
//and -1 if the user does not exist in the database
int checkPass(unsigned long int rNum, char * pass) {
	ptr = head;
	while(ptr != NULL) {
		if(rNum == ptr->rNum && strcmp(pass, ptr->passwd) == 0) { //user-password matches
			return 1;
		}
		else if(rNum == ptr->rNum && strcmp(pass, ptr->passwd) != 0) { //user exists but password doesn't match
			return 0;
		}
		ptr = ptr->next;
	}
	return -1; //user doesn't exist in the database
}

//function to update record status of a user based on the roll number
void updateFeedbackStatus(unsigned long int rNum) {
	ptr = head;
	while(ptr != NULL) {
		if(ptr->rNum == rNum) {
			ptr->record = 1; //status updated
		}
		ptr = ptr->next;
	}
}

//main function
void main() {
	unsigned long int rno;
	char pass[32];
	char c;
	clrscr();
	readUserData();
	do {
		printf("\n Enter Roll Number : ");
		cin >> rno;
		if(checkList(rno) == 1) { //user has given feedback
			printf(" ERROR : this user has already given their feedback!");
		}
		else if(checkList(rno) == -1) { //user does not exist in the database
			printf(" ERROR : user does not exist!");
		}
		else if(checkList(rno) == 0) { //user hasn't given feedback
			printf("\n Enter Password : ");
			gets(pass);
			if(checkPass(rno, pass) == 0) { //password doesn't match
				printf(" ERROR : password does not match!");
			}
			else if(checkPass(rno, pass) == -1) { //user does not exist in the database
				printf(" ERROR : user does not exist!");
			}
			else if(checkPass(rno, pass) == 1) { //user-password matches
				updateFeedbackStatus(rno);
				printf("\n Feedback recorded.");
				getch();
				break;
			}
		}
		printf("\n Do you want to try again? (Y/N) : ");
		cin >> c;
	} while(c == 'Y' || c == 'y');
	saveUserData();
	return;
}
