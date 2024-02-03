#pragma once

#include <string>
#include <vector>

#include "SerialIn.h"
#include "SerialOut.h"
#include "Pack.h"
#include "TypeDef.h"
#include "TypeReg.h"
#include "TypeBuilder.h"

template <typename T>
class BaseTypeBuilder : public TypeBuilder
{
public:
	BaseTypeBuilder(TypeFactory* pTypeFactory) : TypeBuilder(pTypeFactory) {};
	virtual ~BaseTypeBuilder() {};

	virtual int Unpackage(const std::string& key, any_type* object, IPack* pack)
	{
		int err = BUILD_OKAY;
		if (pack)
		{
			//key is unused, might need it later?
			std::string value = pack->GetValue();
			SerializerIn serializer(value);
			T* ptr = (T*)object;
			serializer.IO((*ptr));
		}
		else
		{
			err = BUILD_ERROR;
		}
		return err;
	};

	virtual int Package(const std::string& key, any_type* object, IPack** pack)
	{
		std::string type = GetBuilderTypeName();
		std::string value;
		SerializerOut serializer(value);
		T* ptr = (T*)object;
		serializer.IO((*ptr));

		//always build ValuePack for base types
		ValuePack* valpack = new ValuePack();
		valpack->SetKey(key);
		valpack->SetType(type);
		valpack->SetValue(value);

		(*pack) = valpack;
		return BUILD_OKAY;
	};
};
