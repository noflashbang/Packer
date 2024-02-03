#pragma once

#include "StandardLib.h"

#define DEFAULT_ARRAY_SEPERATOR ","

class ISerializer
{
public:
	virtual ~ISerializer() {};

	virtual void IO(short& io) = 0;
	virtual void IO(unsigned short& io) = 0;
	virtual void IO(int& io) = 0;
	virtual void IO(unsigned int& io) = 0;
	virtual void IO(long& io) = 0;
	virtual void IO(unsigned long& io) = 0;
	virtual void IO(float& io) = 0;
	virtual void IO(double& io) = 0;
	virtual void IO(char& io) = 0;
	virtual void IO(unsigned char& io) = 0;
	virtual void IO(bool& io) = 0;
	virtual void IO(std::string& io) = 0;

	virtual void IO(std::vector<short>& io) = 0;
	virtual void IO(std::vector<unsigned short>& io) = 0;
	virtual void IO(std::vector<int>& io) = 0;
	virtual void IO(std::vector<unsigned int>& io) = 0;
	virtual void IO(std::vector<long>& io) = 0;
	virtual void IO(std::vector<unsigned long>& io) = 0;
	virtual void IO(std::vector<float>& io) = 0;
	virtual void IO(std::vector<double>& io) = 0;
	virtual void IO(std::vector<char>& io) = 0;
	virtual void IO(std::vector<unsigned char>& io) = 0;
	virtual void IO(std::vector<bool>& io) = 0;
	virtual void IO(std::vector <std::string>& io) = 0;
};

