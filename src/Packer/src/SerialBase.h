#pragma once

#include "StandardLib.h"
#include "ISerial.h"

class SerialBase : ISerializer
{
public:
	SerialBase(std::string& data);
	virtual ~SerialBase() {};

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

	std::string ArraySeperator = DEFAULT_ARRAY_SEPERATOR;

protected:

	template <typename T>
	void inline Serialize(T& io)
	{
		auto result = std::format("{}", io);
		m_Data.append(result);
	};

	template <typename T>
	void SerializeArray(std::vector<T>& io)
	{
		for (auto iter = io.begin(); iter != io.end(); iter++)
		{
			if (iter != io.begin())
			{
				IO(ArraySeperator);
			}
			T holder = *iter;
			IO(holder);
		}
	};

	template <typename T, typename S>
	void ReadArray(std::vector<T>& io)
	{
		int ii = 0;
		auto tokens = SplitString(m_Data, ArraySeperator);
		io.resize(tokens.size());
		for (auto token : tokens)
		{
			S in(&token);
			T holder;
			in.IO(holder);
			io.at(ii) = holder;
			ii++;
		}
	};

	std::string& m_Data;

private:

	inline std::vector<std::string> SplitString(const std::string& str, const std::string& delim)
	{
		std::vector<std::string> tokens;
		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find(delim, prev);
			if (pos == std::string::npos) pos = str.length();
			std::string token = str.substr(prev, pos - prev);
			if (!token.empty()) tokens.push_back(token);
			prev = pos + delim.length();
		} while (pos < str.length() && prev < str.length());
		return tokens;
	};

	SerialBase() = delete;
	SerialBase(const SerialBase&) = delete;
	SerialBase& operator=(const SerialBase&) = delete;
};
