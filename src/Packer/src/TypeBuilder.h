#pragma once

#include <string>
#include <vector>

#include "SerialIn.h"
#include "SerialOut.h"
#include "Pack.h"
#include "TypeReg.h"

#pragma warning(disable: 4786)

typedef void any_type;

#define BUILD_OKAY 1
#define BUILD_ERROR -1

class TypeBuilder
{
public:
	TypeBuilder();
	virtual ~TypeBuilder() {};

	virtual int BuildType(std::string key, any_type* object, Package pack) = 0;
	virtual int BuildPack(std::string key, any_type* object, Package* pack) = 0;
	void SetTypeRegistration(TypeRegistration type);
	long GetBuilderTypeID();
	std::string GetBuilderTypeName();
	bool IsBuilderType(long ID);
	bool IsBuilderTypeName(std::string name);

private:
	TypeRegistration m_TypeReg;
};
