//	Feedback
//take user input for feedback form and save it as a
//.csv file that can be edited in MS Excel.
//
// v:1.0 July 27, 2017

//include headers
#include<conio.h>
#include<iostream.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//encryption layer libraby
#include "encrypto.h"

//declarations for input parameters from the terminal(file names)
char userData_fileName[32];
char feedbackData_fileName[32];

//this program assumes that there are no bogus values
//or redundancies in the users database (UserData.csv)
//this file should include 3 columns for roll nummber, password and record status
//NOTE: this program assumes that the field for roll number is a primary key to the table
//NOTE: record status is a binary field that records if that user has submitted their feedback (YES/NO)

//struct for user database
struct node {
	//for user -password matching
	unsigned long int rNum;
	unsigned long long int passwd;
	int record; //save record state for each unique roll number to avoid redundancies
	node * next;
} *head, *ptr;

//class question for each in the feedback
class Question {
	//data members are public
	public :
	char answerToQuestion[512];
	int rating;
};

//class subject for each in the feedback
class Subject {
	//data members are public
	public :
	char subjectName[32];
	char teacherName[32];
	Question questions[7];
};

//class feedback for storing the data
class FeedbackForm {
	//data members are public
	public :
	unsigned long int rollNumber;
	char studentName[32];
	char password[32];
	Subject subjects[5];
};

//global variable declarations
FeedbackForm feed;

//function to read from an existing user database and store it into a linked list
void readUserData() {
	head = NULL;
	FILE * file = fopen(userData_fileName, "r");
	if(file == NULL) {
		return;
	}
	int counter = 0;
	char * token;
	char * end;
	char buffer[256];
	unsigned long int r;
	int rec;
	unsigned long long int p;
	while(fgets(buffer, 256, (FILE*) file)) {

		token = strtok(buffer, ",");
		if(token != NULL) {
			r = strtoul(token, &end, 10);
		}

		token = strtok(NULL, ",");
		if(token != NULL) {
			p = strtoul(token, &end, 10); //stdlib::strtoull not available in Borland Turbo C compiler
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
			ptr->passwd = p;
			ptr->record = rec;
			ptr->next = NULL;
			head = ptr;
		}
		else if(counter > 1) {
			ptr->rNum = r;
			ptr->passwd = p;
			ptr->record = rec;
			ptr->next = head;
			head = ptr;
		}
		counter += 1;
	}

	fclose(file);
	free(token);
	free(end);
	return;
}

//function to save user data to database
void saveUserData() {
	FILE * file = fopen(userData_fileName, "w");
	if(file == NULL) { //file error
		return;
	}
	fprintf(file, "Roll Number,Password,Feedback Recorded\n");
	ptr = head;
	while(ptr != NULL) {
		fprintf(file, "%lu,", ptr->rNum);
		fprintf(file, "%llu,", ptr->passwd);
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
int checkList(unsigned long int num) {
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
int checkPass(unsigned long int rNum, unsigned long long int pass) {
	ptr = head;
	while(ptr != NULL) {
		if(rNum == ptr->rNum && (unsigned int)(pass) == (unsigned int)(ptr->passwd)) { //user-password matches
			return 1;
		}
		else if(rNum == ptr->rNum && (unsigned int)(pass) != (unsigned int)(ptr->passwd)) { //user exists but password doesn't match
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

//TODO: add new user list functions here

//function to clear the user list for garbage collection
void clearUserList() {
	node * temp;
	ptr = head;
	while(ptr != NULL) {
		temp = ptr->next;
		free(ptr);
		ptr = temp;
	}
	free(ptr);
	free(temp);
	//head = NULL;
	free(head);
}

//function to input data from console
//return 0 if feedback was successfully recorded and -1 otherwise
int inputData() {
	char choice;
	do {
		clrscr();
		printf("\t\t\t\t Feedback Form\n");
		printf("\n Enter your Name : ");
		gets(feed.studentName);
		if(strcmp(feed.studentName, "") == 0) {
			printf(" ERROR : You did not enter a valid name.");
		}
		else {
			printf(" Enter your Roll Number : ");
			cin >> feed.rollNumber;
			if(checkList(feed.rollNumber) == 1) { //user has given feedback
				printf(" ERROR : This user has already given their feedback!");
			}
			else if(checkList(feed.rollNumber) == -1) { //user does not exist in the database
				printf(" ERROR : This user does not exist!");
			}
			else if(checkList(feed.rollNumber) == 0) { //user hasn't given feedback
				printf(" Enter your Password : ");
				//gets(feed.password);
				getPassword(feed.password);
				unsigned long long int temp = FNVhash(feed.password);
				if(feed.password == NULL) {
					printf("\n ERROR : You entered an invalid password!");
				}
				else if(checkPass(feed.rollNumber, temp) == 0) { //password doesn't match
					printf("\n ERROR : Password does not match!");
				}
				else if(checkPass(feed.rollNumber, temp) == -1) { //user does not exist in the database
					printf("\n ERROR : This user does not exist!");
				}
				else if(checkPass(feed.rollNumber, temp) == 1) { //user-password matches
					updateFeedbackStatus(feed.rollNumber);
					/*for(int i = 0;i < 5;i++) {
						printf("\n Enter #%d Subject's Name : ", i+1);
						gets(feed.subjects[i].subjectName);
						printf(" Enter #%d Teacher's Name : ", i+1);
						gets(feed.subjects[i].teacherName);
						for(int j = 0;j < 7;j++) {
							printf(" Answer to Q. #%d : ", j+1);
							gets(feed.subjects[i].questions[j].answerToQuestion);
							printf(" Q. #%d's rating : ", j+1);
							cin >> feed.subjects[i].questions[j].rating;
						}
					}*/
					printf("\n Feedback recorded.");
					return 0;
				}
			}
		}
		printf("\n\n Do you want to try again? (Y/N) : ");
		cin >> choice;
	} while(choice == 'Y' || choice =='y');
	return -1;
}

//function to save current data
void saveData() {
	FILE * file = fopen(feedbackData_fileName, "r");
	if(file == NULL) { //file does not exist
		file = fopen(feedbackData_fileName, "w"); //open in write mode
		fprintf(file, "Roll Number,Name,Password (FNV hashed)\n");
	}
	else { //file exists
		file = fopen(feedbackData_fileName, "a"); //open in append mode
	}

	fprintf(file, "%lu,", feed.rollNumber);
	fprintf(file, "%s,", feed.studentName);
	fprintf(file, "%llu,", FNVhash(feed.password));
	/*for(int i = 0;i < 5;i++) {
		fprintf(file, "%s,", feed.subjects[i].subjectName);
		fprintf(file, "%s,", feed.subjects[i].teacherName);
		for(int j = 0;j < 7;j++) {
			fprintf(file, "%s,", feed.subjects[i].questions[j].answerToQuestion);
			fprintf(file, "%d,", feed.subjects[i].questions[j].rating);
		}
	}*/
	fprintf(file, "\n");

	fclose(file);
	printf("\n Finished Saving... ");
	return;
}

//main function
void main(int argc, char ** argv) {
	//usage: feedback <userData_fileName> <feedbackData_fileName>
	if(argc < 3) {
		//fprintf(stderr, "usage: feedback <userData_fileName> <feedbackData_fileName>");
		//exit(-1);

		//defaulting values:
		strcpy(userData_fileName, "UserData.csv");
		strcpy(feedbackData_fileName, "FeedbackData.csv");
	}
	else {
		strcpy(userData_fileName, argv[1]);
		strcpy(feedbackData_fileName, argv[2]);
	}
	clrscr();
	readUserData();
	char choice;
	do {
		clrscr();
		int success = inputData();
		if(success == 0) { //feedback successfully recorded
			saveData();
			printf("\n\n Do you want to continue? (Y/N) : ");
			cin >> choice;
		}
		else if(success == -1) {
			break;
		}
	} while(choice == 'y' || choice == 'Y');
	saveUserData();
	clearUserList();
	return;
}
