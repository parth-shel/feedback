//	Feedback
//take user input for feedback form and save it as a
//.csv file that can be edited in MS Excel.
//
// v:1.6 October 8, 2017

//include headers
#include<conio.h>
#include<iostream.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>
#include<time.h>

//encryption layer libraby
#include "encrypto.h"

//utilities Library
#include "utils.h"

//declarations for input parameters from the terminal(file names)
char userData_fileName[32];
char feedbackData_fileName[32];

//this program assumes that there are no bogus values
//or redundancies in the users database (UserData.csv)
//this file should include 3 columns for roll nummber, password and record status
//NOTE: this program assumes that the field for roll number is a primary key to the table
//NOTE: record status is a binary field that records if that user has submitted their feedback (YES/NO)

//global variable declarations
FeedbackForm feed;

//function to read from an existing user database and store it into a linked list
void readUserData() {
	head = NULL;
	FILE * file = fopen(userData_fileName, "r");
	if(file == NULL) { //file doesn't exist
		return;
	}
	int counter = 0;
	char * token;
	char * end;
	char buffer[512];
	unsigned long int r;
	int rec;
	unsigned long long int p;
	while(fgets(buffer, 512, (FILE*) file)) {

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

//function to sort the user list
//the parameter order decides whether the list is sorted in ascending or descending order
//ascending if order is 1 and otherwise if order is -1
void sortUserList(int order) {
	int swapped = 0;
	unsigned long int rNum;
	unsigned long long int passwd;
	int record;

	node * temp = NULL;
	do {
		swapped = 0;
		ptr = head;
		while(ptr->next != temp) {
			if(ptr->rNum > ptr->next->rNum && order == 1) { //ascending order
				rNum = ptr->rNum;
				passwd = ptr->passwd;
				record = ptr->record;

				ptr->rNum = ptr->next->rNum;
				ptr->passwd = ptr->next->passwd;
				ptr->record = ptr->next->record;

				ptr->next->rNum = rNum;
				ptr->next->passwd = passwd;
				ptr->next->record = record;

				swapped = 1;
			}
			else if(ptr->rNum < ptr->next->rNum && order == -1) { //descending order
				rNum = ptr->rNum;
				passwd = ptr->passwd;
				record = ptr->record;

				ptr->rNum = ptr->next->rNum;
				ptr->passwd = ptr->next->passwd;
				ptr->record = ptr->next->record;

				ptr->next->rNum = rNum;
				ptr->next->passwd = passwd;
				ptr->next->record = record;

				swapped = 1;
			}
			ptr = ptr->next;
		}
		temp = ptr;
	} while(swapped);
}

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
			//input sanitization
			while(cin.fail()) {
				cin.clear(); //clear input buffer to restore cin to a usable state
				cin.ignore(INT_MAX, '\n'); //ignore last input
				printf(" ERROR : You can only enter numbers.");
				printf("\n Enter your Roll Number : ");
				cin >> feed.rollNumber;
			}
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
					for(int i = 0;i < 10;i++) {
						//printf("\n Enter Subject #%d's Name : ", i+1);
						if(i < 5) {
							printf("\n Enter Theory Subject  #%d's Name : ", i+1);
						}
						else if(i > 4) {
							printf("\n Enter Practical Subject #%d's Name : ", i-4);
						}
						gets(feed.subjects[i].subjectName);
						while(strcmp(feed.subjects[i].subjectName, "") == 0) {
							printf(" ERROR : You did not enter a valid subject's name.");
							//printf("\n Enter Subject #%d's Name : ", i+1);
							if(i < 5) {
								printf("\n Enter Teory Subject  #%d's Name : ", i+1);
							}
							else if(i > 4) {
								printf("\n Enter Practical Subject #%d's Name : ", i-4);
							}
							gets(feed.subjects[i].subjectName);
						}
						int k = 0;
						do {
							printf(" Enter Teacher #%d's Name : ", k+1);
							gets(feed.subjects[i].teachers[k].teacherName);
							while(strcmp(feed.subjects[i].teachers[k].teacherName, "") == 0 && k == 0) {
								printf(" ERROR : You did not enter a valid teacher's name.");
								printf("\n Enter Teacher #%d's Name : ", k+1);
								gets(feed.subjects[i].teachers[k].teacherName);
							}
							if(strcmp(feed.subjects[i].teachers[k].teacherName, "") == 0 && k > 0) {
								break;
							}
							for(int j = 0;j < 10;j++) {
								do {
									//printf(" Q. #%d's rating : ", j+1);
									printf("\n Q. #%d - %s : ", j+1, questions[j]);
									cin >> feed.subjects[i].teachers[k].questions[j].rating;
									//input sanitization
									while(cin.fail()) {
										cin.clear(); //clear input buffer to restore cin to a usable state
										cin.ignore(INT_MAX, '\n'); //ignore last input
										printf(" ERROR : You can only enter numbers.");
										//printf("\n Q. #%d's rating : ", j+1);
										printf("\n Q. #%d - %s : ", j+1, questions[j]);
										cin >> feed.subjects[i].teachers[k].questions[j].rating;
									}
									//limit the user rating to between 1 and 5
									if(feed.subjects[i].teachers[k].questions[j].rating < 1 || feed.subjects[i].teachers[k].questions[j].rating > 5) {
										printf(" ERROR : The rating must be between 1 and 5.\n");
									}
								} while(feed.subjects[i].teachers[k].questions[j].rating < 1 || feed.subjects[i].teachers[k].questions[j].rating > 5);
							}
							k += 1;
						} while(strcmp(feed.subjects[i].teachers[k-1].teacherName, "") != 0 || k == 1);
					}
					printf("\n Additional Comments (if any) : ");
					gets(feed.additionalComments);
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
	time_t t;
	time(&t);

	FILE * file = fopen(feedbackData_fileName, "r");
	if(file == NULL) { //file does not exist
		file = fopen(feedbackData_fileName, "w"); //open in write mode
		fprintf(file, "Roll Number,Name,Time,");
		for(int i = 0;i < 10;i++) {
			//fprintf(file, "Subject #%d's Name,", i+1);
			if(i < 5) {
				fprintf(file, "Theory Subject #%d's Name,", i+1);
			}
			else if(i > 4) {
				fprintf(file, "Practical Subject #%d's Name,", i-4);
			}
			for(int k = 0;k < 3;k++) {
				fprintf(file, "Teacher #%d's Name,", k+1);
				for(int j = 0;j < 10;j++) {
					fprintf(file, "Q. #%d,", j+1);
				}
			}
		}
		fprintf(file, "Additional Comments,");
		fprintf(file, "\n");
	}
	else { //file exists
		file = fopen(feedbackData_fileName, "a"); //open in append mode
	}

	fprintf(file, "%lu,", feed.rollNumber);
	fprintf(file, "%s,", feed.studentName);
	//fprintf(file, "%llu,", FNVhash(feed.password));
	fprintf(file, "%.*s,", 24, ctime(&t)); //timestamping
	for(int i = 0;i < 10;i++) {
		fprintf(file, "%s,", feed.subjects[i].subjectName);
		for(int k = 0;k < 3;k++) {
			fprintf(file, "%s,", feed.subjects[i].teachers[k].teacherName);
			for(int j = 0;j < 7;j++) {
				//if(strcmp(feed.subjects[i].teachers[k].teacherName, "") == 0) {
				if(feed.subjects[i].teachers[k].questions[j].rating == 0) {
					fprintf(file, ",");
				}
				else {
					fprintf(file, "%d,", feed.subjects[i].teachers[k].questions[j].rating);
				}
			}
		}
	}
	fprintf(file, "%s,", feed.additionalComments);
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
	//sortUserList(1); //sort the list in ascending order
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
			//continue;
		}
	} while(choice == 'y' || choice == 'Y');
	saveUserData();
	clearUserList();
	return;
}
