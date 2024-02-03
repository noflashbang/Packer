#include "Pack.h"
#include <stack>

int IPack::FromStream(const std::string& stream, IPack** retpack)
{
	(*retpack) = NULL;

	std::string Key;
	std::string Type;
	std::string Value;

	std::stack<PARSE_STATE> statestack;

	statestack.push(STATE_NOSTATE);
	statestack.push(STATE_NOSTATE);

	bool multi = false;
	std::stack<MultiPack*> packstack;

	unsigned long size = 0;
	unsigned long begin = stream.find_first_of(PACK_B);
	unsigned long end = stream.find_last_of(PACK_E);
	unsigned long work = begin;
	unsigned long current = begin;
	if (begin == std::string::npos || end == std::string::npos)
	{
		return 0;
	}
	else
	{
		size = end - begin;
		if (size <= 0)
			return 0;
	}

	while (statestack.top() != STATE_ERROR && current <= end)
	{
		//look for state changes
		if (stream.at(current) == PACK_B)
		{
			if (statestack.top() == STATE_NOSTATE)
				statestack.pop();

			statestack.push(STATE_LESS);
		}
		else if (stream.at(current) == TYPE_B)
		{
			if (statestack.top() == STATE_NOSTATE)
				statestack.pop();

			statestack.push(STATE_BEGINP);
		}
		else if (stream.at(current) == TYPE_E)
		{
			if (statestack.top() == STATE_NOSTATE)
				statestack.pop();

			statestack.push(STATE_ENDP);
		}
		else if (stream.at(current) == VALUE)
		{
			if (statestack.top() == STATE_NOSTATE)
				statestack.pop();

			statestack.push(STATE_EQUAL);
		}
		else if (stream.at(current) == PACK_E)
		{
			if (statestack.top() == STATE_NOSTATE)
				statestack.pop();

			statestack.push(STATE_GREATER);
		}
		else if (stream.at(current) == ESCAPE)
		{
			if (statestack.top() == STATE_NOSTATE)
				statestack.pop();

			statestack.push(STATE_ESCAPE);
		}
		else
		{
			if (statestack.top() != STATE_NOSTATE)
				statestack.push(STATE_NOSTATE);
		}

		switch (statestack.top())
		{
		case STATE_NOSTATE:
		{
			current++;
			break;
		}
		case STATE_ERROR:
		{
			break;
		}
		case STATE_LESS:
		{
			statestack.pop(); //remove current state
			if (statestack.top() == STATE_EQUAL)
			{
				//start of a new multi
				MultiPack* pack = new MultiPack;
				pack->SetKey(Key);
				pack->SetType(Type);
				packstack.push(pack);
				multi = true;
				work = (current + 1);
			}
			else if (statestack.top() == STATE_NOSTATE || statestack.top() == STATE_GREATER)
			{
				//set the begining of key
				work = (current + 1);
			}
			else
			{
				statestack.push(STATE_LESS);
				statestack.push(STATE_ERROR);
				break;
			}
			statestack.pop(); //remove old
			statestack.push(STATE_LESS); //add self
			current++;
			break;
		}
		case STATE_BEGINP:
		{
			statestack.pop(); //remove current state
			if (statestack.top() == STATE_LESS)
			{
				//retrive Key
				Key = stream.substr(work, ((current)-work));
				//set the begining of type
				work = (current + 1);
			}
			else
			{
				statestack.push(STATE_BEGINP);
				statestack.push(STATE_ERROR);
				break;
			}
			statestack.pop(); //remove old
			statestack.push(STATE_BEGINP); //add self
			current++;
			break;
		}
		case STATE_ENDP:
		{
			statestack.pop(); //remove current state
			if (statestack.top() == STATE_BEGINP)
			{
				//retrive type
				Type = stream.substr(work, ((current)-work));
			}
			else
			{
				statestack.push(STATE_ENDP);
				statestack.push(STATE_ERROR);
				break;
			}
			statestack.pop(); //remove old
			statestack.push(STATE_ENDP); //add self
			current++;
			break;
		}
		case STATE_EQUAL:
		{
			statestack.pop(); //remove current state
			if (statestack.top() == STATE_ENDP)
			{
				//might be value or more packs
				work = (current + 1);
			}
			else
			{
				statestack.push(STATE_EQUAL);
				statestack.push(STATE_ERROR);
				break;
			}
			statestack.pop(); //remove old
			statestack.push(STATE_EQUAL); //add self
			current++;
			break;
		}
		case STATE_GREATER:
		{
			statestack.pop(); //remove current state
			if (statestack.top() == STATE_EQUAL)
			{
				//build a valuepack
				ValuePack* pack = new ValuePack;
				pack->SetKey(Key);
				pack->SetType(Type);
				Value = stream.substr(work, ((current)-work));
				pack->SetValueEscape(Value);
				if (multi)
				{
					packstack.top()->AddChild(pack);
				}
				else
				{
					if ((*retpack) == NULL)
					{
						(*retpack) = pack;
					}
				}
			}
			else if (statestack.top() == STATE_GREATER)
			{
				//end of the current pack
				unsigned long stacksize = packstack.size();
				if (stacksize == 1)
				{
					if ((*retpack) == NULL)
					{
						(*retpack) = packstack.top();
					}
				}
				else
				{
					IPack* hold = packstack.top();
					packstack.pop();
					packstack.top()->AddChild(hold);
				}
			}
			else
			{
				statestack.push(STATE_GREATER);
				statestack.push(STATE_ERROR);
				break;
			}
			statestack.pop(); //remove old
			statestack.push(STATE_GREATER); //add self
			current++;
			break;
		}
		case STATE_ESCAPE:
		{
			statestack.pop();
			current++;//escape
			current++;//escaped char

			break;
		}
		}
	}
	if (statestack.top() == STATE_ERROR)
		return 0;

	return 1;
};

std::string ValuePack::GetKey() const
{
	return m_Key;
};
std::string ValuePack::GetType() const
{
	return m_Type;
};
std::string ValuePack::GetValueEscape() const
{
	std::string hold = m_Value;
	Escape2String(&hold);
	return hold;
};
std::string ValuePack::GetValue() const
{
	return m_Value;
};

void ValuePack::SetKey(const std::string& key)
{
	m_Key = key;
};
void ValuePack::SetType(const std::string& type)
{
	m_Type = type;
};
void ValuePack::SetValueEscape(const std::string& value)
{
	std::string hold = value;
	Escape2String(&hold);
	m_Value = hold;
};
void ValuePack::SetValue(const std::string& value)
{
	m_Value = value;
};
std::string ValuePack::GetStreamEscape() const
{
	std::string ret = "";
	ret.push_back(PACK_B);
	ret.append(m_Key);
	ret.push_back(TYPE_B);
	ret.append(m_Type);
	ret.push_back(TYPE_E);
	ret.push_back(VALUE);

	std::string hold = m_Value;
	String2Escape(&hold);
	ret.append(hold);
	ret.push_back(PACK_E);

	return ret;
};
std::string ValuePack::GetStream() const
{
	std::string ret = "";
	ret.push_back(PACK_B);
	ret.append(m_Key);
	ret.push_back(TYPE_B);
	ret.append(m_Type);
	ret.push_back(TYPE_E);
	ret.push_back(VALUE);
	ret.append(m_Value);
	ret.push_back(PACK_E);

	return ret;
};
int ValuePack::FindKeyShallow(const std::string& key, const std::string& type, IPack** found)
{
	int ret = 1;
	if (IsKey(key, type))
	{
		(*found) = this;
		ret = 1;
	}
	else
	{
		(*found) = NULL;
		ret = 0;
	}
	return ret;
};
int ValuePack::FindKeyDeep(const std::string& key, const std::string& type, IPack** found, int levels)
{
	int ret = 1;
	if (IsKey(key, type))
	{
		(*found) = this;
		ret = 1;
	}
	else
	{
		(*found) = NULL;
		ret = 0;
	}
	return ret;
};
bool ValuePack::IsKey(const std::string& key, const std::string& type)
{
	if (m_Type == type)
	{
		if (key == m_Key)
			return 1;
		else
			return 0;
	}
	else
		return 0;
};

bool ValuePack::HasChildren()
{
	return false;
};

void ValuePack::GetChildMap(std::map<std::string, IPack*>* childMap)
{
	//no childern
	childMap->clear();
};


std::string MultiPack::GetKey() const
{
	return m_Key;
};

std::string MultiPack::GetType() const
{
return m_Type;
};

std::string MultiPack::GetValueEscape() const
{
	std::string hold = "";
	for (auto iter = m_Children.begin(); iter != m_Children.end(); iter++)
	{
		hold.append((*iter)->GetStreamEscape());
	}
	return hold;
};

std::string MultiPack::GetValue() const
{
	std::string hold = "";
	for (auto iter = m_Children.begin(); iter != m_Children.end(); iter++)
	{
		hold.append((*iter)->GetStream());
	}
	return hold;
};

void MultiPack::SetKey(const std::string& key)
{
	m_Key = key;
};

void MultiPack::SetType(const std::string& type)
{
	m_Type = type;
};

std::string MultiPack::GetStreamEscape() const
{
	std::string ret = "";
	ret.push_back(PACK_B);
	ret.append(m_Key);
	ret.push_back(TYPE_B);
	ret.append(m_Type);
	ret.push_back(TYPE_E);
	ret.push_back(VALUE);
	for (auto iter = m_Children.begin(); iter != m_Children.end(); iter++)
	{
		ret.append((*iter)->GetStreamEscape());
	}
	ret.push_back(PACK_E);
	return ret;
};

std::string MultiPack::GetStream() const
{
	std::string ret = "";
	ret.push_back(PACK_B);
	ret.append(m_Key);
	ret.push_back(TYPE_B);
	ret.append(m_Type);
	ret.push_back(TYPE_E);
	ret.push_back(VALUE);
	for (auto iter = m_Children.begin(); iter != m_Children.end(); iter++)
	{
		ret.append((*iter)->GetStream());
	}
	ret.push_back(PACK_E);
	return ret;
};

int MultiPack::FindKeyShallow(const std::string& key, const std::string& type, IPack** found)
{
	return FindKeyDeep(key, type, found, 1);
};
int MultiPack::FindKeyDeep(const std::string& key, const std::string& type, IPack** found, int levels)
{
	int ret = -1;
	bool deep = true;
	//stop on zero, -1 is allowed for whole tree search
	if (levels == 0)
		deep = false;

	if (IsKey(key, type))
	{
		(*found) = this;
		ret = 1;
	}
	else
	{
		if (deep)
		{
			std::vector<IPack*>::iterator iter;
			for (iter = m_Children.begin(); iter != m_Children.end(); iter++)
			{
				ret = -1;
				ret = (*iter)->FindKeyDeep(key, type, found, (levels - 1));
				if (ret == 1)
					break;
			}
		}
	}
	return ret;
};

bool MultiPack::IsKey(const std::string& key, const std::string& type)
{
	if (m_Type == type)
	{
		if (key == m_Key)
			return 1;
		else
			return 0;
	}
	else
		return 0;
};

bool MultiPack::HasChildren()
{
	return (m_Children.size() > 0);
};

void MultiPack::GetChildMap(std::map<std::string, IPack*>* childMap)
{
	childMap->clear();
	std::vector<IPack*>::iterator iter;
	for (iter = m_Children.begin(); iter != m_Children.end(); iter++)
	{
		std::string key = (*iter)->GetKey();
		childMap->insert(std::pair<std::string, IPack*>(key, (*iter)));
	}
};

void MultiPack::AddChild(IPack* child)
{
	m_Children.push_back(child);
};