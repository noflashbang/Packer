#include "SerialIn.h"

void SerializerIn::IO(short& io)
{
	io = stoi(m_Data);
}

void SerializerIn::IO(unsigned short& io)
{
	io = stoul(m_Data);
}

void SerializerIn::IO(int& io)
{
	io = stoi(m_Data);
}

void SerializerIn::IO(unsigned int& io)
{
	io = stoul(m_Data);
}

void SerializerIn::IO(long& io)
{
	io = stol(m_Data);
}

void SerializerIn::IO(unsigned long& io)
{
	io = stoul(m_Data);
}

void SerializerIn::IO(float& io)
{
	io = stof(m_Data);
}

void SerializerIn::IO(double& io)
{
	io = stod(m_Data);
}

void SerializerIn::IO(char& io)
{
	io = m_Data[0];
}

void SerializerIn::IO(unsigned char& io)
{
	io = m_Data[0];
}

void SerializerIn::IO(bool& io)
{
	static std::string TrueString = "TRUE";

	std::string hold;
	this->IO(hold);

	std::transform(hold.begin(), hold.end(), hold.begin(), ::toupper);

	if (hold.compare(TrueString) == 0)
	{
		io = true;
	}
	else
	{
		io = false;
	}
}
void SerializerIn::IO(std::string& io)
{
	io = m_Data;
}

void SerializerIn::IO(std::vector<short>& io)
{
	ReadArray<short, SerializerIn>(io);
}

void SerializerIn::IO(std::vector<unsigned short>& io)
{
	ReadArray<unsigned short, SerializerIn>(io);
}

void SerializerIn::IO(std::vector<int>& io)
{
	ReadArray<int, SerializerIn>(io);
}

void SerializerIn::IO(std::vector<unsigned int>& io)
{
	ReadArray<unsigned int, SerializerIn>(io);
}

void SerializerIn::IO(std::vector<long>& io)
{
	ReadArray<long, SerializerIn>(io);
}

void SerializerIn::IO(std::vector<unsigned long>& io)
{
	ReadArray<unsigned long, SerializerIn>(io);
}

void SerializerIn::IO(std::vector<float>& io)
{
	ReadArray<float, SerializerIn>(io);
}

void SerializerIn::IO(std::vector<double>& io)
{
	ReadArray<double, SerializerIn>(io);
}

void SerializerIn::IO(std::vector<char>& io)
{
	ReadArray<char, SerializerIn>(io);
}

void SerializerIn::IO(std::vector<unsigned char>& io)
{
	ReadArray<unsigned char, SerializerIn>(io);
}

void SerializerIn::IO(std::vector<bool>& io)
{
	ReadArray<bool, SerializerIn>(io);
}

void SerializerIn::IO(std::vector<std::string>& io)
{
	ReadArray<std::string, SerializerIn>(io);
}