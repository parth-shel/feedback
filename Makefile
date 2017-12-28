goal: feedMake userDBMake git

feedMake:
	g++ -g -o feedback feedback.cpp

userDBMake:
	g++ -g -o userDB userdb.cpp

git:
	git add .
	git commit -m "made some changes"

clean:
	rm -rf feedback
