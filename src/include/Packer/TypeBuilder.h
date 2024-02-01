#pragma once

#include <string>
#include <vector>


#include "Pack.h"
#include "TypeDef.h"
#include "TypeReg.h"

//forward
class TypeFactory;

class TypeBuilder
{
public:
	TypeBuilder(TypeFactory* pTypeFactory);
	virtual ~TypeBuilder() {};

	virtual int BuildType(std::string key, any_type* object, Package pack) = 0;
	virtual int BuildPack(std::string key, any_type* object, Package* pack) = 0;
	void SetTypeRegistration(TypeRegistration type);
	long GetBuilderTypeID();
	std::string GetBuilderTypeName();
	bool IsBuilderType(long ID);
	bool IsBuilderTypeName(std::string name);

protected:
	TypeFactory* m_TypeFactory;
private:
	TypeRegistration m_TypeReg;
};
