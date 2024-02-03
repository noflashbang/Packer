#include "Pack.h"
#include <stack>

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
std::shared_ptr<IPack> ValuePack::FindKeyShallow(const std::string& key, const std::string& type)
{
	return FindKeyDeep(key, type, 1);
};
std::shared_ptr<IPack> ValuePack::FindKeyDeep(const std::string& key, const std::string& type, int levels)
{
	return nullptr;
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

void ValuePack::GetChildMap(std::map<std::string, std::weak_ptr<IPack>>* childMap)
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

std::shared_ptr<IPack> MultiPack::FindKeyShallow(const std::string& key, const std::string& type)
{
	return FindKeyDeep(key, type, 1);
};
std::shared_ptr<IPack> MultiPack::FindKeyDeep(const std::string& key, const std::string& type, int levels)
{
	if (IsKey(key, type))
	{
		return nullptr;
	}
	else if (levels <= 0)
	{
		return nullptr;
	}
	else
	{		
		for (auto iter : m_Children)
		{
			if (iter->IsKey(key, type))
			{
				return iter;
			}

			std::shared_ptr<IPack> result = iter->FindKeyDeep(key, type, (levels - 1));
			if (result != nullptr)
			{
				return result;
			}
		}	
	}
	return nullptr;
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

void MultiPack::GetChildMap(std::map<std::string, std::weak_ptr<IPack>>* childMap)
{
	childMap->clear();

	//copy each child into the map

	for (auto iter : m_Children)
	{
		std::string key = iter->GetKey();
		childMap->insert(std::pair<std::string, std::weak_ptr<IPack>>(key, iter));
	}
};

void MultiPack::AddChild(std::shared_ptr<IPack> child)
{
	m_Children.push_back(child);
};