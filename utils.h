// Utilities Library
//definitions of data structures required
//
//v:1.1 - October 8, 2017

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
	int rating;
};

//class teacher for each teacher for a subject
class Teacher {
	//data members are public
public:
	char teacherName[32];
	Question questions[10];
};

//class subject for each in the feedback
class Subject {
	//data members are public
	public :
	char subjectName[32];
	Teacher teachers[3];
};

//class feedback for storing the data
class FeedbackForm {
	//data members are public
	public :
	unsigned long int rollNumber;
	char studentName[32];
	char password[32];
	Subject subjects[10];
  char additionalComments[512];
};

//questionare
const char *questions[] = {
	"Contents covered are according to syllabus.",
	"Lectures well prepared, organized & delivered.",
	"Fundamental concepts well taught.",
	"Effective communication skills.",
	"Effective use of teaching aids.",
	"Motivation for Co-curricular & Extra-curricular activities.",
	"Encourage & involve students in interactive sessions.",
	"Availability beyond classroom teaching.",
	"Additional efforts for academically weak students.",
	"Guide students to design, develop & analyze problems to provide solutions."
};
