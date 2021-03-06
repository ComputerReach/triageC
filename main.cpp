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
string removeSubString(string original, string toRemove);


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
		cout << getLinuxData();
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
	cout << "DEV: https://github.com/domtheporcupine/triageC" << endl;
	cout << "OFFICIAL: https://github.com/computerreach/triageC" << endl;
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
	cout << "------------------------" << endl;
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
		void

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
	This function is used to collect all the necessary information about
	a triaged linux computer
	
	@params
		none
	@return
		string | all collected data properly formatted, rather long
*/
string getLinuxData()
{
	string finalData = "--------------------\n";
	string data = runCmd("sudo dmidecode -t system");
	string temp = "";
	istringstream iss(data);
	for(string line; getline(iss, line);)
	{
		if(line.find("Manu") != string::npos)
		{
			line = removeSubString(line, "\t");
			finalData += line;
			finalData += "\n-------------------\n";	
		}
		if(line.find("Version") != string::npos)
		{
			line = removeSubString(line, "\t");
			line = line.replace(line.find("Version"), 7, "Model");
			finalData += line;
			finalData += "\n--------------------\n";
		}
		if(line.find("Serial") != string::npos)
		{
			line = removeSubString(line, "\t");
			finalData += line;
			finalData += "\n--------------------\n";
		}	

	}
	

	bool USB = false;
	bool ETHERNET = false;
	bool VGA = false;
	bool DVI = false;
	bool HDMI = false;
	bool PS2 = false;
	bool WIFI = false;
	bool MIC = false;
	bool HEAD = false;

	data = runCmd("sudo dmidecode");
	int usbCount = 0;
	istringstream ss(data);
	for(string line; getline(ss, line);)
	{
		if(line.find("External") != string::npos)
		{
			if(line.find("USB)") != string::npos)
			{
				USB = true;
				usbCount += 1;
			}
			if(line.find("Ethernet") != string::npos)
				ETHERNET = true;
			if(line.find("PS/2") != string::npos)
				PS2 = true;
			if(line.find("Headphone") != string::npos)
				HEAD = true;
			if(line.find("Microphone") != string::npos)
				MIC = true;
			if(line.find("DVI") != string::npos)
				DVI = true;
			if(line.find("HDMI") != string::npos)
				HDMI = true;
		}
	}


	data = runCmd("lspci");
	istringstream sis(data);
	for(string line; getline(sis, line);)
	{
		if(line.find("Network controller") != string::npos)
			WIFI = true;
		if(line.find("VGA compatible") != string::npos)
			VGA = true;
	}
	
	
	data = runCmd("sudo fdisk -l");
	istringstream ssi(data);	
	string hddSize;
	for(string line; getline(ssi, line);)
	{
		if(line.find("Disk /dev/sda") != string::npos)
		{
			line = line.replace(line.find("Disk /dev/sda:"), 14, "");
			line = line.replace(line.find(","), line.length()+1, "");
			hddSize = line;
		}
	}
	
	
	bool CD = false;
	bool DVD = false;
	bool CDR = false;
	bool DVDR = false;

	data = runCmd("cat /proc/sys/dev/cdrom/info");		
	istringstream nss(data);
	for(string line; getline(nss, line);)
	{
		if(line.find("play audio") != string::npos && line.find("1") != string::npos)
			CD = true;
		if(line.find("read DVD") != string::npos && line.find("1") != string::npos)
			DVD = true;
		if(line.find("write CD-RW") != string::npos && line.find("1") != string::npos)
			CDR = true;
		if(line.find("write DVD-R") != string::npos && line.find("1") != string::npos)
			DVDR = true;
	}

	finalData += "Video Devices:\n";
	if(VGA)
		finalData += "-VGA\n";
	if(DVI)
		finalData += "-DVI\n";
	if(HDMI)
		finalData += "-HDMI\n";
	finalData += "--------------------\nPorts:\n";
	if(ETHERNET)
		finalData += "-ETHERNET\n";
	if(USB)
	{
		string usbNum;
		ostringstream temp;
		temp << usbCount;
		finalData += "-USB x " + temp.str() + '\n';
	}
	if(PS2)
		finalData += "-PS/2\n";
	finalData += "--------------------\nOther Ports:\n";
	if(HEAD)
		finalData += "-Headphone\n";
	if(MIC)
		finalData += "-Microphone\n";
	finalData += "--------------------\n";
	finalData += "HDD Size: " + hddSize + '\n';
	finalData += "--------------------\n";
	if(CD && !DVD)
		finalData += "CD/DVD Drive: CD\n";
	if(CD && DVD && !CDR && !DVDR)
		finalData += "CD/DVD Drive: CD/DVD\n";
	if(CDR && DVD && !DVDR)
		finalData += "CD/DVD Drive: CD-RW/DVD\n";
	if(CDR && DVDR)
		finalData += "CD/DVD Drive: CD-RW/DVD-RW\n";
	if(!CD && !CDR && !DVD && !DVDR)	
		finalData += "CD/DVD Drive: N/A\n";
		
	finalData += "--------------------\nInternet:\n";
	if(WIFI)
		finalData += "Wifi Enabled\n";

	return finalData;
}
/*
	This function is meant to make the process of removing substrings/chars from strings easier
	
	@params:
		string original | the string from which the substring will be removed
		string toRemove | the substring that will be removed from the original
	@return
		string | the new string without the substring
*/
string removeSubString(string original, string toRemove)
{
	string temp;
	original.erase(remove(original.begin(), original.end(), '\t'), original.end());
	return original;
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
		
