#include <algorithm>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <sys/utsname.h>
using namespace std;

void helpMenu();
string comments();
void writeToFile(string collectedInfo, string triageNum);
string getTriageNumber(string theId);
string getMacData();
string getLinuxData();
bool macOrLinux();
string runCmd(string cmd);

int main(int argc, char *argv[])
{
	string data, idNum="";
	for(int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
		{
			helpMenu();
			exit(0);
		}
		else if(strcmp(argv[i], "-v") ==0)
		{
			cout << "Version 0.0.1 Artisan Deblin" << endl;
			exit(0);
		}
		else if(strcmp(argv[i], "-crn") == 0)
		{
			if(argc < i+2)
			{
				cout << "You have not entered a number, program will now exit" << endl;
				exit(1);
			}
			idNum = argv[i+1];
		}
		else
		{
			cout << "Unknown option \'" << argv[i] << "\';" << endl;
			cout << "ABORT!ABORT!ABORT!\nABANDON SHIP!!!!!!\nMAYDAY!!!MAYDAY!!!" << endl;
			cout << "YOU FORGOT HOW TO READ THE HELP MENU!!!" << endl;
			cout << "Try using \'-h\' or \'--help\' for usage options." << endl;
			cout << "Press any key to continue" << endl;
			getline(cin, data);
			exit(1);
		}
		
	}

	cout << "---------------------------------------------------------------" << endl;
	cout << "-------Hello and welcome to the automatic triage script--------" << endl;
	cout << "----rewritten in C by Dominic DiPasquale for Computer Reach----" << endl;
	cout << "---------------------------------------------------------------" << endl;
	idNum = getTriageNumber(idNum);
	if(macOrLinux())
	{
		cout << "Linux computer detected!..." << endl;
		getLinuxData();
	}
	else
	{
		cout << "OSX computer detected!..." << endl;
		getMacData();
	}
	data = data + comments();
	writeToFile(data, idNum);
}
/*
	Displays a simple help menu, outlining the different command line options
	@params:
		 none
	@return:
		void
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

	@params:
		string  theId | value of nothing if the number was not given as a command line argument
		or some value passed as a command line argument

	@return:
		string | minimum of 5 charcters
*/
string getTriageNumber(string theId)
{
	if(theId.length() >= 5)
		return theId;
	cout << "Please enter the Computer Reach given identification code\n: ";
	cin >> theId;

	if(theId.length() < 5)
	{
		{
			cout << "-----" << endl;
			cout << "Code is too few characters, please enter a 5 digit code" << endl;
			cout << "or contact Kevin/Dom\n: ";
			cin >> theId;
		}while(theId.length() < 5);
	}
	cin.clear();
	cin.ignore();
	return theId;
}
/*
	This function prompts the user for additional comments as well as asking
	for the 'okay' to confirm the computer and generated report are acceptable
	if this is not given the program will ask until the user enters the designated
	keyword

	@params:
		none

	@return:
		string | the comments as given by the user
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
	bool commentCheck = false;
	do
	{
		if(comment.find("@okay") == string::npos && comment.find("@notokay") == string::npos)
		{
			cout << "No, seriously I need you to have a minimum of '@okay' or '@notokay' in that comment." << endl;
			cout << "Press enter to continue..." << endl;
			getline(cin, comment);
			cout << "-------------------------" << endl;
			cout << "Please enter your comments or just '@okay' or '@notokay'\n: ";
			getline(cin, comment);
		} 
		else 
		{
			commentCheck = true;
		}
	}while(!commentCheck);
	return comment;
}
/*
	This function writes all the collected data to a .triage.txt file
	with the same name as the CRT number

	@params:
		string collectedInfo | info collected by the script and the comments given by the user
		string triageNum | the CRT identification number
	@return:
		none

*/
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
/*
*/
string getLinuxData()
{
	string data = runCmd("sudo dmidecode -t system");
	string temp = "";
	istringstream iss(data);
	for(string line; getline(iss, line);)
	{
		if(line.find("Manu") != string::npos)
		{
			line.erase(remove(line.begin(), line.end(), '\t'), line.end());

			cout << line << endl;
		
		}
		if(line.find("Version") != string::npos)
			cout << line << endl;
		if(line.find("Serial") != string::npos)
			cout << line << endl;	

	}
	return data;
}
/*
	This function is meant to make the process of calling for output of system commands easier
	it uses popen and pipes to capture output as a string, if the command fails to run the
	entire program will exit
	
	@params:
		string cmd | the command that is to be run
	@return:
		string | the entire output of the command that was run
*/
string runCmd(string cmd)
{
	FILE* pipe = popen(cmd.c_str(), "r");
	if(!pipe)
	{
		cout << "The command '" << cmd << "' has failed!" << endl;
		cout << "This is not good, please tell Kevin or Dom!" << endl;
		cout << "Program will now exit!" << endl;
		exit(1);
	}
	char buffer[128];
	string temp = "";
	while(!feof(pipe))
	{
		if(fgets(buffer,128,pipe) != NULL)
			temp += buffer;
	}
	pclose(pipe);
	return temp;
}
/*
	This function is used to collect data about a Mac computer through the use of system_profiler
	right now at computer reach the GUI version is used, but this can be much quicker

	@params:
		none
	@return:
		string | a multi-line string with all of the collected data nicely formatted
*/	
string getMacData()
{
	/*
		Manufacturer
		Model
		Serial Number
			system_profiler SPHardwareDataType
		------------------
		USB ports 
			system_profiler SPUSBDataType 'Built-in USB'
		------------------
		Video ports (VGA/HDMI/DVI)
		PS2
			user must enter these values
		------------------
		Ethernet
		Wifi
			system_profiler SPNetworkDataType 'Wi-Fi'
		-----------------
		Headphone
		Mic
			system_profiler SPAudioDataType
		----------------
		CD/DVD drive
			system_profiler SPDiscBurningDataType
		----------------
		OS version
			system_prfiler SPSoftwareDataType
		Processor Speed
		Ram Size
			
		HDD Size
		64/32bit
			uname -a
	*/
	string allData = "-----------------------\nManufacturer: Apple\n";
	string data = runCmd("system_profiler SPHardwareDataType");
	
	istringstream iss(data);
    for(string line; getline(iss, line);)
    {
		if(line.find("Model Identifier") != string::npos)
			cout << line << endl;	

		if(line.find("Serial") != string::npos)
			cout << line << endl;
	}
	return "";
}
/*
	This function uses the uname command to determine the type of system it is running on as the commands 
	used in this program are specific to either Linux or OSX

	@params:
		none
	@return:
		bool | true if the computer is a Liunx computer and false if the computer is OSX
*/
bool macOrLinux()
{
	string sysType;
	struct utsname darwinLinux;
	uname(&darwinLinux);
	sysType = darwinLinux.sysname;
	if(sysType.compare("Linux") == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
		
