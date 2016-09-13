#pragma once

#include <fstream>

class FileInterface
{
public:
	FileInterface();
	~FileInterface();

	bool OpenStream();
	void WriteToFile(const char* fname);

	std::string getFileName();
	void setFileName(std::string);
	bool opened;

private:
	std::string fileName;

protected:
	FILE *fp;

};

FileInterface::FileInterface()
{
	opened = false;
}

FileInterface::~FileInterface()
{
	if(opened)
	{
		fclose(fp);
		opened = false;
	}
}

bool FileInterface::OpenStream()
{
	const char* fname = fileName.c_str();

	if((fp = fopen(fname, "w+")) == NULL)
	{
		std::cout<<"[ERROR] Logger: Cannot open output file!" << std::endl;
		std::cout<<"Logging into STDOUT now" << std::endl;
		return false;
	}

	opened = true;

	return true;
}

void FileInterface::WriteToFile(const char* str)
{
	if(!opened)
	{
		std::cout<<"[ERROR] Logger: Cannot write: file is not opened" << std::endl;
		return;
	}

	fprintf(fp, "%s", str); 
	fflush(fp);
}

std::string FileInterface::getFileName()
{
	return fileName;
}

void FileInterface::setFileName(std::string fname)
{
	fileName = fname;
}
