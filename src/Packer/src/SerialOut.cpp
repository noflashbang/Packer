#include "SerialOut.h"

void SerializerOut::IO(short& io)
{
	Serialize(io);
}

void SerializerOut::IO(unsigned short& io)
{
	Serialize(io);
}

void SerializerOut::IO(int& io)
{
	Serialize(io);
}

void SerializerOut::IO(unsigned int& io)
{
	Serialize(io);
}

void SerializerOut::IO(long& io)
{
	Serialize(io);
}

void SerializerOut::IO(unsigned long& io)
{
	Serialize(io);
}

void SerializerOut::IO(float& io)
{
	Serialize(io);
}

void SerializerOut::IO(double& io)
{
	Serialize(io);
}

void SerializerOut::IO(char& io)
{
	Serialize(io);
}

void SerializerOut::IO(unsigned char& io)
{
	Serialize(io);
}

void SerializerOut::IO(bool& io)
{
	static std::string TrueString = "TRUE";
	static std::string FalseString = "FALSE";
	if (io)
		this->IO(TrueString);
	else
		this->IO(FalseString);
}
void SerializerOut::IO(std::string& io)
{
	m_Data.append(io);
}

void SerializerOut::IO(std::vector<short>& io)
{
	SerializeArray(io);
}

void SerializerOut::IO(std::vector<unsigned short>& io)
{
	SerializeArray(io);
}

void SerializerOut::IO(std::vector<int>& io)
{
	SerializeArray(io);
}

void SerializerOut::IO(std::vector<unsigned int>& io)
{
	SerializeArray(io);
}

void SerializerOut::IO(std::vector<long>& io)
{
	SerializeArray(io);
}

void SerializerOut::IO(std::vector<unsigned long>& io)
{
	SerializeArray(io);
}

void SerializerOut::IO(std::vector<float>& io)
{
	SerializeArray(io);
}

void SerializerOut::IO(std::vector<double>& io)
{
	SerializeArray(io);
}

void SerializerOut::IO(std::vector<char>& io)
{
	SerializeArray(io);
}

void SerializerOut::IO(std::vector<unsigned char>& io)
{
	SerializeArray(io);
}

void SerializerOut::IO(std::vector<bool>& io)
{
	SerializeArray(io);
}

void SerializerOut::IO(std::vector<std::string>& io)
{
	SerializeArray(io);
}