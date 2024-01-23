#pragma once

#include "StandardLib.h"
#include "SerialBase.h"


class SerializerIn : public SerialBase
{
public:
	SerializerIn(std::string& data) :  SerialBase(data) {}
	virtual ~SerializerIn() {};

	virtual void IO(short& io);
	virtual void IO(unsigned short& io);
	virtual void IO(int& io);
	virtual void IO(unsigned int& io);
	virtual void IO(long& io);
	virtual void IO(unsigned long& io);
	virtual void IO(float& io);
	virtual void IO(double& io);
	virtual void IO(char& io);
	virtual void IO(unsigned char& io);
	virtual void IO(bool& io);
	virtual void IO(std::string& io);

	virtual void IO(std::vector<short>& io);
	virtual void IO(std::vector<unsigned short>& io);
	virtual void IO(std::vector<int>& io);
	virtual void IO(std::vector<unsigned int>& io);
	virtual void IO(std::vector<long>& io);
	virtual void IO(std::vector<unsigned long>& io);
	virtual void IO(std::vector<float>& io);
	virtual void IO(std::vector<double>& io);
	virtual void IO(std::vector<char>& io);
	virtual void IO(std::vector<unsigned char>& io);
	virtual void IO(std::vector<bool>& io);
	virtual void IO(std::vector <std::string>& io);

protected:
	
	SerializerIn() = delete;
	SerializerIn(const SerializerIn&) = delete;
	SerializerIn& operator=(const SerializerIn&) = delete;
};
