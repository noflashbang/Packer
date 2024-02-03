#pragma once

#include <string>
#include <vector>
#include <stack>
#include <map>

class IPack
{
public:
	IPack() {};
	virtual ~IPack() {};

	virtual std::string GetKey() const = 0;
	virtual std::string GetType()  const = 0;

	virtual std::string GetValueEscape()  const = 0;
	virtual std::string GetValue()  const = 0;

	virtual std::string GetStreamEscape()  const = 0;
	virtual std::string GetStream()  const = 0;

	typedef enum STATE_
	{
		STATE_NOSTATE = -1,
		STATE_ERROR = 0,
		STATE_LESS = 1, //'<'
		STATE_BEGINP = 2, //'('
		STATE_ENDP = 3, //')'
		STATE_EQUAL = 4, //'='
		STATE_GREATER = 5, //'>'
		STATE_ESCAPE = 6, //'@'
	} PARSE_STATE;

#define PACK_B '<'
#define PACK_E '>'
#define TYPE_B '('
#define TYPE_E ')'
#define VALUE  '='
#define ESCAPE '@'

	static int FromStream(const std::string& stream, IPack** pack);

	virtual int FindKeyShallow(const std::string& key, const std::string& type, IPack** found) = 0;
	virtual int FindKeyDeep(const std::string& key, const std::string& type, IPack** found, int levels) = 0;
	virtual bool IsKey(const std::string& key, const std::string& type) = 0;

	virtual bool HasChildren() = 0;
	virtual void GetChildMap(std::map<std::string, IPack*>* childMap) = 0;
};

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

	virtual int FindKeyShallow(const std::string& key, const std::string& type, IPack** found);
	virtual int FindKeyDeep(const std::string& key, const std::string& type, IPack** found, int levels);
	virtual bool IsKey(const std::string& key, const std::string& type);

	virtual bool HasChildren();
	virtual void GetChildMap(std::map<std::string, IPack*>* childMap);

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
		std::vector<IPack*>::iterator iter;
		for (iter = m_Children.begin(); iter != m_Children.end(); iter++)
		{
			delete (*iter);
			(*iter) = NULL;
		}
	};

	virtual std::string GetKey() const;
	virtual std::string GetType() const;
	virtual std::string GetValueEscape() const;
	virtual std::string GetValue() const;

	void SetKey(const std::string& key);
	void SetType(const std::string& type);

	virtual std::string GetStreamEscape() const;
	virtual std::string GetStream() const;

	virtual int FindKeyShallow(const std::string& key, const std::string& type, IPack** found);
	virtual int FindKeyDeep(const std::string& key, const std::string& type, IPack** found, int levels);
	virtual bool IsKey(const std::string& key, const std::string& type);

	virtual bool HasChildren();
	virtual void GetChildMap(std::map<std::string, IPack*>* childMap);

	void AddChild(IPack* child);

private:
	std::string m_Key;
	std::string m_Type;
	std::vector<IPack*> m_Children;
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

