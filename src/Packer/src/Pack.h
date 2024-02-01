#ifndef PACK_H
#define PACK_H

#pragma warning(disable: 4786)

#include <string>
#include <vector>
#include <stack>
#include <map>

class BasePack
{
public:
	BasePack() {};
	virtual ~BasePack() {};

	virtual int GetKey(std::string* key) = 0;
	virtual int GetType(std::string* type) = 0;

	virtual int GetValueEscape(std::string* value) = 0;
	virtual int GetValue(std::string* value) = 0;

	virtual int GetStreamEscape(std::string* stream) = 0;
	virtual int GetStream(std::string* stream) = 0;

	typedef enum _STATE_
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

	static int FromStream(std::string* stream, BasePack** pack);

	virtual int FindKeyShallow(std::string key, std::string type, BasePack** found) = 0;
	virtual int FindKeyDeep(std::string key, std::string type, BasePack** found, int levels) = 0;
	virtual bool IsKey(std::string key, std::string type) = 0;

	virtual bool HasChildren() = 0;
	virtual void GetChildMap(std::map<std::string, BasePack* >* childMap) = 0;
};

typedef BasePack* Package;

class ValuePack : public BasePack
{
public:
	ValuePack() { m_Key = ""; m_Type = ""; m_Value = ""; };
	virtual ~ValuePack() { m_Key = ""; m_Type = ""; m_Value = ""; };

	virtual int GetKey(std::string* key);
	virtual int GetType(std::string* type);
	virtual int GetValueEscape(std::string* value);
	virtual int GetValue(std::string* value);

	int SetKey(std::string key);
	int SetType(std::string type);
	int SetValue(std::string value);
	int SetValueEscape(std::string value);

	virtual int GetStreamEscape(std::string* stream);
	virtual int GetStream(std::string* stream);

	virtual int FindKeyShallow(std::string key, std::string type, BasePack** found);
	virtual int FindKeyDeep(std::string key, std::string type, BasePack** found, int levels);
	virtual bool IsKey(std::string key, std::string type);

	virtual bool HasChildren();
	virtual void GetChildMap(std::map<std::string, BasePack* >* childMap);

private:
	std::string m_Key;
	std::string m_Type;
	std::string m_Value;
};

class MultiPack : public BasePack
{
public:
	MultiPack() { m_Key = ""; m_Type = ""; };
	virtual ~MultiPack()
	{
		m_Key = "";
		m_Type = "";
		std::vector<BasePack*>::iterator iter;
		for (iter = m_Children.begin(); iter != m_Children.end(); iter++)
		{
			delete (*iter);
			(*iter) = NULL;
		}
	};

	virtual int GetKey(std::string* key);
	virtual int GetType(std::string* type);
	virtual int GetValueEscape(std::string* value);
	virtual int GetValue(std::string* value);

	int SetKey(std::string key);
	int SetType(std::string type);

	virtual int GetStreamEscape(std::string* stream);
	virtual int GetStream(std::string* stream);

	virtual int FindKeyShallow(std::string key, std::string type, BasePack** found);
	virtual int FindKeyDeep(std::string key, std::string type, BasePack** found, int levels);
	virtual bool IsKey(std::string key, std::string type);

	virtual bool HasChildren();
	virtual void GetChildMap(std::map<std::string, BasePack* >* childMap);

	void AddChild(BasePack* child);

private:
	std::string m_Key;
	std::string m_Type;
	std::vector<BasePack*> m_Children;
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
#endif
