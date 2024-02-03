#pragma once

#include <string>
#include <vector>
#include <memory>

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

	virtual int Unpackage(const std::string& key, any_type* object, std::shared_ptr<IPack> pack) = 0;
	virtual Packer::BuildPack Package(const std::string& key, any_type* object) = 0;

	void SetTypeRegistration(const TypeRegistration& typeReg);

	long GetBuilderTypeID() const;
	std::string GetBuilderTypeName() const;
	
	bool IsBuilderType(long ID) const;
	bool IsBuilderTypeName(const std::string& name) const;

protected:
	TypeFactory* m_TypeFactory;

private:
	TypeRegistration m_TypeReg;
};
