#include "Packer.h"
#include "Pack.h"

#include <stack>

Packer::BuildPack Packer::FromStream(const std::string& stream)
{
	std::shared_ptr<IPack> retpack = nullptr;

	std::string Key;
	std::string Type;
	std::string Value;

	std::stack<Packer::PARSE_STATE> statestack;

	statestack.push(Packer::PARSE_STATE::STATE_NOSTATE);
	statestack.push(Packer::PARSE_STATE::STATE_NOSTATE);

	bool multi = false;
	std::stack<std::shared_ptr<MultiPack>> packstack;

	unsigned long size = 0;
	unsigned long begin = stream.find_first_of(PACK_B);
	unsigned long end = stream.find_last_of(PACK_E);
	unsigned long work = begin;
	unsigned long current = begin;
	if (begin == std::string::npos || end == std::string::npos)
	{
		return BuildPack(BUILD_ERROR, nullptr);
	}
	else
	{
		size = end - begin;
		if (size <= 0)
			return BuildPack(BUILD_ERROR, nullptr);
	}

	while (statestack.top() != Packer::PARSE_STATE::STATE_ERROR && current <= end)
	{
		//look for state changes
		if (stream.at(current) == PACK_B)
		{
			if (statestack.top() == Packer::PARSE_STATE::STATE_NOSTATE)
				statestack.pop();

			statestack.push(Packer::PARSE_STATE::STATE_LESS);
		}
		else if (stream.at(current) == TYPE_B)
		{
			if (statestack.top() == Packer::PARSE_STATE::STATE_NOSTATE)
				statestack.pop();

			statestack.push(Packer::PARSE_STATE::STATE_BEGINP);
		}
		else if (stream.at(current) == TYPE_E)
		{
			if (statestack.top() == Packer::PARSE_STATE::STATE_NOSTATE)
				statestack.pop();

			statestack.push(Packer::PARSE_STATE::STATE_ENDP);
		}
		else if (stream.at(current) == VALUE)
		{
			if (statestack.top() == Packer::PARSE_STATE::STATE_NOSTATE)
				statestack.pop();

			statestack.push(Packer::PARSE_STATE::STATE_EQUAL);
		}
		else if (stream.at(current) == PACK_E)
		{
			if (statestack.top() == Packer::PARSE_STATE::STATE_NOSTATE)
				statestack.pop();

			statestack.push(Packer::PARSE_STATE::STATE_GREATER);
		}
		else if (stream.at(current) == ESCAPE)
		{
			if (statestack.top() == Packer::PARSE_STATE::STATE_NOSTATE)
				statestack.pop();

			statestack.push(Packer::PARSE_STATE::STATE_ESCAPE);
		}
		else
		{
			if (statestack.top() != Packer::PARSE_STATE::STATE_NOSTATE)
				statestack.push(STATE_NOSTATE);
		}

		switch (statestack.top())
		{
		case Packer::PARSE_STATE::STATE_NOSTATE:
		{
			current++;
			break;
		}
		case Packer::PARSE_STATE::STATE_ERROR:
		{
			break;
		}
		case Packer::PARSE_STATE::STATE_LESS:
		{
			statestack.pop(); //remove current state
			if (statestack.top() == Packer::PARSE_STATE::STATE_EQUAL)
			{
				//start of a new multi
				std::shared_ptr<MultiPack> pack = std::make_shared<MultiPack>();
				pack->SetKey(Key);
				pack->SetType(Type);
				packstack.push(pack);
				multi = true;
				work = (current + 1);
			}
			else if (statestack.top() == Packer::PARSE_STATE::STATE_NOSTATE || statestack.top() == Packer::PARSE_STATE::STATE_GREATER)
			{
				//set the begining of key
				work = (current + 1);
			}
			else
			{
				statestack.push(Packer::PARSE_STATE::STATE_LESS);
				statestack.push(Packer::PARSE_STATE::STATE_ERROR);
				break;
			}
			statestack.pop(); //remove old
			statestack.push(Packer::PARSE_STATE::STATE_LESS); //add self
			current++;
			break;
		}
		case Packer::PARSE_STATE::STATE_BEGINP:
		{
			statestack.pop(); //remove current state
			if (statestack.top() == Packer::PARSE_STATE::STATE_LESS)
			{
				//retrive Key
				Key = stream.substr(work, ((current)-work));
				//set the begining of type
				work = (current + 1);
			}
			else
			{
				statestack.push(Packer::PARSE_STATE::STATE_BEGINP);
				statestack.push(Packer::PARSE_STATE::STATE_ERROR);
				break;
			}
			statestack.pop(); //remove old
			statestack.push(Packer::PARSE_STATE::STATE_BEGINP); //add self
			current++;
			break;
		}
		case Packer::PARSE_STATE::STATE_ENDP:
		{
			statestack.pop(); //remove current state
			if (statestack.top() == Packer::PARSE_STATE::STATE_BEGINP)
			{
				//retrive type
				Type = stream.substr(work, ((current)-work));
			}
			else
			{
				statestack.push(Packer::PARSE_STATE::STATE_ENDP);
				statestack.push(Packer::PARSE_STATE::STATE_ERROR);
				break;
			}
			statestack.pop(); //remove old
			statestack.push(Packer::PARSE_STATE::STATE_ENDP); //add self
			current++;
			break;
		}
		case Packer::PARSE_STATE::STATE_EQUAL:
		{
			statestack.pop(); //remove current state
			if (statestack.top() == Packer::PARSE_STATE::STATE_ENDP)
			{
				//might be value or more packs
				work = (current + 1);
			}
			else
			{
				statestack.push(Packer::PARSE_STATE::STATE_EQUAL);
				statestack.push(Packer::PARSE_STATE::STATE_ERROR);
				break;
			}
			statestack.pop(); //remove old
			statestack.push(Packer::PARSE_STATE::STATE_EQUAL); //add self
			current++;
			break;
		}
		case Packer::PARSE_STATE::STATE_GREATER:
		{
			statestack.pop(); //remove current state
			if (statestack.top() == Packer::PARSE_STATE::STATE_EQUAL)
			{
				//build a valuepack
				std::shared_ptr<ValuePack> pack = std::make_shared<ValuePack>();
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
					if (retpack.get() == nullptr)
					{
						retpack = pack;
					}
				}
			}
			else if (statestack.top() == Packer::PARSE_STATE::STATE_GREATER)
			{
				//end of the current pack
				unsigned long stacksize = packstack.size();
				if (stacksize == 1)
				{
					if (retpack.get() == nullptr)
					{
						retpack = packstack.top();
					}
				}
				else
				{
					std::shared_ptr<IPack> hold = packstack.top();
					packstack.pop();
					packstack.top()->AddChild(hold);
				}
			}
			else
			{
				statestack.push(Packer::PARSE_STATE::STATE_GREATER);
				statestack.push(Packer::PARSE_STATE::STATE_ERROR);
				break;
			}
			statestack.pop(); //remove old
			statestack.push(Packer::PARSE_STATE::STATE_GREATER); //add self
			current++;
			break;
		}
		case Packer::PARSE_STATE::STATE_ESCAPE:
		{
			statestack.pop();
			current++;//escape
			current++;//escaped char

			break;
		}
		}
	}
	if (statestack.top() == Packer::PARSE_STATE::STATE_ERROR)
	{
		return BuildPack(BUILD_ERROR, nullptr);
	}

	return BuildPack(BUILD_OKAY, retpack);
};