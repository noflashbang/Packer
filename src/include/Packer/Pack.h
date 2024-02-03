#pragma once

#include <string>
#include <vector>
#include <stack>
#include <map>

#include "IPack.h"
#include "Packer.h"

class ValuePack : public IPack
{
public:
	ValuePack() { m_Key = ""; m_Type = ""; m_Value = ""; };
	virtual ~ValuePack() { m_Key = ""; m_Type = ""; m_Value = ""; };

	virtual std::string GetKey() const;
	virtual std::string GetType() const;
	virtual std::string GetValueEscape() const;
	virtual std::string GetValue() const;

	void SetKey(const std::string& key);
	void SetType(const std::string& type);
	void SetValue(const std::string& value);
	void SetValueEscape(const std::string& value);

	virtual std::string GetStreamEscape() const;
	virtual std::string GetStream() const;

	virtual std::shared_ptr<IPack> FindKeyShallow(const std::string& key, const std::string& type);
	virtual std::shared_ptr<IPack> FindKeyDeep(const std::string& key, const std::string& type, int levels);
	virtual bool IsKey(const std::string& key, const std::string& type);

	virtual bool HasChildren();
	virtual void GetChildMap(std::map<std::string, std::weak_ptr<IPack>>* childMap);

private:
	std::string m_Key;
	std::string m_Type;
	std::string m_Value;
};

class MultiPack : public IPack
{
public:
	MultiPack() { m_Key = ""; m_Type = ""; };
	virtual ~MultiPack()
	{
		m_Key = "";
		m_Type = "";
	};

	virtual std::string GetKey() const;
	virtual std::string GetType() const;
	virtual std::string GetValueEscape() const;
	virtual std::string GetValue() const;

	void SetKey(const std::string& key);
	void SetType(const std::string& type);

	virtual std::string GetStreamEscape() const;
	virtual std::string GetStream() const;

	virtual std::shared_ptr<IPack> FindKeyShallow(const std::string& key, const std::string& type);
	virtual std::shared_ptr<IPack> FindKeyDeep(const std::string& key, const std::string& type, int levels);
	virtual bool IsKey(const std::string& key, const std::string& type);

	virtual bool HasChildren();
	virtual void GetChildMap(std::map<std::string, std::weak_ptr<IPack>>* childMap);

	void AddChild(std::shared_ptr<IPack> child);

private:
	std::string m_Key;
	std::string m_Type;
	std::vector<std::shared_ptr<IPack>> m_Children;
};

inline void String2Escape(std::string* str)
{
	std::string hold = "";
	for (unsigned ii = 0; ii < str->size(); ii++)
	{
		char tmp = str->at(ii);
		if (tmp == PACK_B)
		{
			hold += ESCAPE;
			hold += PACK_B;
		}
		else if (tmp == PACK_E)
		{
			hold += ESCAPE;
			hold += PACK_E;
		}
		else if (tmp == TYPE_B)
		{
			hold += ESCAPE;
			hold += TYPE_B;
		}
		else if (tmp == TYPE_E)
		{
			hold += ESCAPE;
			hold += TYPE_E;
		}
		else if (tmp == VALUE)
		{
			hold += ESCAPE;
			hold += VALUE;
		}
		else if (tmp == ESCAPE)
		{
			hold += ESCAPE;
			hold += ESCAPE;
		}
		else
		{
			hold += tmp;
		}
	}
	(*str) = hold;
};

inline void Escape2String(std::string* str)
{
	std::string hold = "";
	for (unsigned ii = 0; ii < str->size(); ii++)
	{
		char tmp = str->at(ii);
		if (tmp == ESCAPE)
		{
			ii++;
			tmp = str->at(ii);
		}
		hold += tmp;
	}
	(*str) = hold;
};

