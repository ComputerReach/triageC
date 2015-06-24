#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
using namespace std;

void helpMenu();
string comments();
void writeToFile(string collectedInfo, string triageNum);
string getTriageNumber(string theId);

int main(int argc, char *argv[])
{
	string data, idNum="";
	for(int i = 0; i < argc; i++)
	{
		if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
		{
			helpMenu();
			exit(0);
		}
		if(strcmp(argv[i], "-v") ==0)
		{
			cout << "Version 0.0.1 Artisan Deblin" << endl;
			exit(0);
		}
		if(strcmp(argv[i], "-crn") == 0)
		{
			if(argc < i+2)
			{
				cout << "You have not entered a number, program will now exit" << endl;
				exit(1);
			}	
			idNum = argv[i+1];
		}
	}	

	cout << "---------------------------------------------------------------" << endl;
	cout << "-------Hello and welcome to the automatic triage script--------" << endl;
	cout << "----rewritten in C by Dominic DiPasquale for Computer Reach----" << endl;
	cout << "---------------------------------------------------------------" << endl;	
	idNum = getTriageNumber(idNum);
	data = data + comments();
	writeToFile(data, idNum);
}
/*
	Displays a simple help menu, outlining the different command line options
	@param | none
	@return | void
*/
void helpMenu()
{
	cout << "This is the triage application, used to extract data from" << endl;
	cout << "refurbished Mac and Linux computers. It is written in C/C++" << endl;
	cout << "by Dominic DiPasquale. Source is avaliable here:" << endl;
	cout << "https://github.com/domtheporcupine/triageC" << endl;
	cout << "----------------------" << endl;
	cout << "Usage: crdata [options]" << endl;
	cout << "Options:" << endl;
	cout << "-h,--help\t\tDisplay this help" << endl;
	cout << "-v\t\t\tDisplay version number" << endl;
	cout << "-crn,<arg>\t\tSpecify CR number" << endl;
	cout << "\n\nPlease report all bugs to Kevin/Dom" << endl;

}
/*
	This function makes sure that the CRT number has been assigned and is a minimum of
	5 numbers long.
	
	@param string | value of nothing if the number was not given as a command line argument
	 or some value passed as a command line argument
	
	@return: string | minimum of 5 charcters	
*/
string getTriageNumber(string theId)
{
	if(theId.length() >= 5)
		return theId;
	cout << "Please enter the Computer Reach given identification code\n: ";
	string id;
	cin >> id;
	if(theId.length() < 5)
	{
		do
		{
			cout << "-----" << endl;
			cout << "Code is too few characters, please enter a 5 digit code" << endl;
			cout << "or contact Kevin/Dom\n:";
			cin >> id;
		}while(theId.length() < 5);
	}
	return id;
}
/*
	This function prompts the user for additional comments as well as asking
	for the 'okay' to confirm the computer and generated report are acceptable
	
	@param: none
	
	@return: string | the comments as given by the user
*/
string comments()
{
	cout << "Please review the above synopsis and provide any comments you deem" << endl;
	cout << "neccesary below. As a minimum you must enter '@okay' if the " << endl;
	cout << "majority of the information is correct, or '@notokay' if the info " << endl;
	cout << "is spotty and inaccurate" << endl;
	cout << "------------------------\n: ";
	string comment;
	getline(cin, comment);
	cout << comment;
	return comment;
}
void writeToFile(string collectedInfo, string triageNum)
{
	ofstream newFile;
	string temp = triageNum+".triage.txt";
	newFile.open(temp.c_str());
	if(newFile.is_open())
	{
		newFile << collectedInfo;
	}
	newFile.close();
} 
