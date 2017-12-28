goal: feedMake userDBMake

feedMake:
	g++ -g -o feedback feedback.cpp

userDBMake:
	g++ -g -o userDB userdb.cpp

clean:
	rm -rf feedback
