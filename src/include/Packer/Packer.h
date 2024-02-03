#pragma once

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <memory>

#include "IPack.h"
#include "TypeDef.h"


namespace Packer
{
	//class PackParser
	//{
	//public:
	//	PackParser();
	//	~PackParser();
	//
	//	static std::string PackToString(const std::shared_ptr<IPack>& pack);
	//	static std::shared_ptr<IPack> PackFromString(const std::string& pack);
	//};

	struct BuildPack
	{
		BuildStatus Status;
		std::shared_ptr<IPack> Package;

		BuildPack(BuildStatus status, std::shared_ptr<IPack> pack): Status(status), Package(pack)
		{
		}
	};

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

	BuildPack FromStream(const std::string& stream);
};



