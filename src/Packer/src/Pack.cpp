#include "Pack.h"
#include <stack>

int BasePack::FromStream(std::string* stream, BasePack** retpack)
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
  unsigned long begin = stream->find_first_of(PACK_B);
  unsigned long end = stream->find_last_of(PACK_E);
  unsigned long work = begin;
  unsigned long current = begin;
  if(begin == std::string::npos || end == std::string::npos)
  {
    return 0;
  }
  else
  {
    size = end - begin;
    if(size <= 0)
      return 0;
  }
  
  while(statestack.top() != STATE_ERROR && current <= end)
  {
    //look for state changes
    if(stream->at(current) == PACK_B)
    {
      if(statestack.top() == STATE_NOSTATE)
        statestack.pop();

      statestack.push(STATE_LESS);
    }
    else if(stream->at(current) == TYPE_B)
    {
      if(statestack.top() == STATE_NOSTATE)
        statestack.pop();

      statestack.push(STATE_BEGINP);
    }
    else if(stream->at(current) == TYPE_E)
    {
      if(statestack.top() == STATE_NOSTATE)
        statestack.pop();

      statestack.push(STATE_ENDP);
    }
    else if(stream->at(current) == VALUE)
    {
      if(statestack.top() == STATE_NOSTATE)
        statestack.pop();

      statestack.push(STATE_EQUAL);
    }
    else if(stream->at(current) == PACK_E)
    {
      if(statestack.top() == STATE_NOSTATE)
        statestack.pop();

      statestack.push(STATE_GREATER);
    }
    else if(stream->at(current) == ESCAPE)
    {
      if(statestack.top() == STATE_NOSTATE)
        statestack.pop();

      statestack.push(STATE_ESCAPE);
    }
    else
    {
      if(statestack.top() != STATE_NOSTATE)
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
        if(statestack.top() == STATE_EQUAL)
        {
          //start of a new multi
          MultiPack* pack = new MultiPack;
          pack->SetKey(Key);
          pack->SetType(Type);
          packstack.push(pack);
          multi = true;
          work = (current+1);
        }
        else if(statestack.top() == STATE_NOSTATE || statestack.top() == STATE_GREATER)
        {
          //set the begining of key
          work = (current+1);
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
        if(statestack.top() == STATE_LESS)
        {
          //retrive Key
          Key = stream->substr(work, ((current) - work));
          //set the begining of type
          work = (current+1);
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
        if(statestack.top() == STATE_BEGINP)
        {
          //retrive type
          Type = stream->substr(work, ((current) - work));
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
        if(statestack.top() == STATE_ENDP)
        {
          //might be value or more packs
          work = (current+1);
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
        if(statestack.top() == STATE_EQUAL)
        {
          //build a valuepack
          ValuePack* pack = new ValuePack;
          pack->SetKey(Key);
          pack->SetType(Type);
          Value = stream->substr(work, ((current) - work));
          pack->SetValueEscape(Value);
          if(multi)
          {
            packstack.top()->AddChild(pack);
          }
          else
          {
            if((*retpack) == NULL)
            {
              (*retpack) = pack;
            }
          }
        }
        else if(statestack.top() == STATE_GREATER)
        {
          //end of the current pack
          unsigned long stacksize = packstack.size();
          if(stacksize == 1)
          {
            if((*retpack) == NULL)
            {
              (*retpack) = packstack.top();
            }
          }
          else
          {
            BasePack* hold = packstack.top();
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
  if(statestack.top() == STATE_ERROR)
    return 0;

  return 1;
};

int ValuePack::GetKey(std::string *key)
{
  (*key) = m_Key;
  return 1;
};
int ValuePack::GetType(std::string *type)
{
  (*type) = m_Type;
  return 1;
};
int ValuePack::GetValueEscape(std::string *value)
{
  std::string hold = m_Value;
  String2Escape(&hold);
  (*value) += hold;
  return 1;
};
int ValuePack::GetValue(std::string *value)
{
  (*value) += m_Value;
  return 1;
};

int ValuePack::SetKey(std::string key)
{
  m_Key = key;
  return 1;
};
int ValuePack::SetType(std::string type)
{
  m_Type = type;
  return 1;
};
int ValuePack::SetValueEscape(std::string value)
{
  Escape2String(&value);
  m_Value = value;
  return 1;
};
int ValuePack::SetValue(std::string value)
{
  m_Value = value;
  return 1;
};
int ValuePack::GetStreamEscape(std::string *stream)
{
  (*stream) += PACK_B;
  (*stream) += m_Key;
  (*stream) += TYPE_B;
  (*stream) += m_Type;
  (*stream) += TYPE_E;
  (*stream) += VALUE;
  std::string hold = m_Value;
  String2Escape(&hold);
  (*stream) += hold;
  (*stream) += PACK_E;
  return 1;
};
int ValuePack::GetStream(std::string *stream)
{
  (*stream) += PACK_B;
  (*stream) += m_Key;
  (*stream) += TYPE_B;
  (*stream) += m_Type;
  (*stream) += TYPE_E;
  (*stream) += VALUE;
  (*stream) += m_Value;
  (*stream) += PACK_E;
  return 1;
};
int ValuePack::FindKeyShallow(std::string key, std::string type, BasePack** found)
{
  int ret = 1;
  if(IsKey(key, type))
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
int ValuePack::FindKeyDeep(std::string key, std::string type, BasePack** found, int levels)
{
  int ret = 1;
  if(IsKey(key, type))
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
bool ValuePack::IsKey(std::string key, std::string type)
{
  if(m_Type == type)
  {
    if(key == m_Key)
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

void ValuePack::GetChildMap(std::map<std::string, BasePack* >* childMap)
{
  //no childern
  childMap->clear();
};

int MultiPack::GetKey(std::string *key)
{
  (*key) = m_Key;
  return 1;
};
int MultiPack::GetType(std::string *type)
{
  (*type) = m_Type;
  return 1;
};
int MultiPack::GetValueEscape(std::string *value)
{
  std::vector<BasePack*>::iterator iter;
  for(iter = m_Children.begin(); iter != m_Children.end(); iter++)
  {
    (*iter)->GetStreamEscape(value);
  }
  return 1;
};
int MultiPack::GetValue(std::string *value)
{
  std::vector<BasePack*>::iterator iter;
  for(iter = m_Children.begin(); iter != m_Children.end(); iter++)
  {
    (*iter)->GetStream(value);
  }
  return 1;
};
int MultiPack::SetKey(std::string key)
{
  m_Key = key;
  return 1;
};
int MultiPack::SetType(std::string type)
{
  m_Type = type;
  return 1;
};

int MultiPack::GetStreamEscape(std::string *stream)
{
  (*stream) += PACK_B;
  (*stream) += m_Key;
  (*stream) += TYPE_B;
  (*stream) += m_Type;
  (*stream) += TYPE_E;
  (*stream) += VALUE;
  std::vector<BasePack*>::iterator iter;
  for(iter = m_Children.begin(); iter != m_Children.end(); iter++)
  {
    (*iter)->GetStreamEscape(stream);
  }
  (*stream) += PACK_E;
  return 1;
};

int MultiPack::GetStream(std::string *stream)
{
  (*stream) += PACK_B;
  (*stream) += m_Key;
  (*stream) += TYPE_B;
  (*stream) += m_Type;
  (*stream) += TYPE_E;
  (*stream) += VALUE;
  std::vector<BasePack*>::iterator iter;
  for(iter = m_Children.begin(); iter != m_Children.end(); iter++)
  {
    (*iter)->GetStream(stream);
  }
  (*stream) += PACK_E;
  return 1;
};

int MultiPack::FindKeyShallow(std::string key, std::string type, BasePack** found)
{
  return FindKeyDeep(key, type, found, 1);
};
int MultiPack::FindKeyDeep(std::string key, std::string type, BasePack** found, int levels)
{
  int ret = -1;
  bool deep = true;
  //stop on zero, -1 is allowed for whole tree search
  if(levels == 0)
    deep = false;

  if(IsKey(key, type))
  {
    (*found) = this;
    ret = 1;
  }
  else
  {
    if(deep)
    {
      std::vector<BasePack*>::iterator iter;
      for(iter = m_Children.begin(); iter != m_Children.end(); iter++)
      {
        ret = -1;
        ret = (*iter)->FindKeyDeep(key, type, found, (levels-1));
        if(ret == 1)
          break;
      }
    }
  }
  return ret;
};

bool MultiPack::IsKey(std::string key, std::string type)
{
  if(m_Type == type)
  {
    if(key == m_Key)
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

void MultiPack::GetChildMap(std::map<std::string, BasePack* >* childMap)
{
  childMap->clear();
  std::vector<BasePack*>::iterator iter;
  for(iter = m_Children.begin(); iter != m_Children.end(); iter++)
  {
    std::string key = "";
    (*iter)->GetKey(&key);
    childMap->insert(std::pair<std::string, BasePack* >(key, (*iter)));
  }
};

void MultiPack::AddChild(BasePack* child)
{
  m_Children.push_back(child);
};


