#include <iostream>
#include "nlog.h"
using namespace std;

class something
{
private:
	int a, b, c;

public:
	string str1, str2;

	void print_str();
};

void something::print_str()
{
	cout << str1 << " " << str2 + "AZAZZAAA";
}

int main()
{
	//LALALALALA
	
	Log mylogger1;
	Log mylogger2;

	mylogger2.setLoggingFile("neuron.log");

	mylogger1.setLoggingType(_stdout);
	mylogger2.setLoggingType(_file);

	mylogger1.setLoggingLevel(ERROR);
	mylogger2.setLoggingLevel(WARNING);

	mylogger2.setLoggingProfile("CUSTOM_1");

	mylogger1 << "Logger1 info";
	mylogger2 << "Logger2 info line";


	for(int i = 0; i < 10; i++)
	{
		mylogger1 << i;
		mylogger2 << 2*i;

		if(i % 3 == 0)
			mylogger2.setLoggingLevel(INFO);

		if(i % 7 == 0)
			mylogger2.setLoggingLevel(ERROR);
	}





	//mylogger1.setLoggingProfile("PROFILE 1");
//mylogger1 << "Profile logging";
	//mylogger1.logNow(INFO) << "SUKA";

	return 0;

}
