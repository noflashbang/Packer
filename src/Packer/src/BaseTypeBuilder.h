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

	virtual int Unpackage(const std::string& key, any_type* object, std::shared_ptr<IPack> pack)
	{
		if (pack != nullptr)
		{
			//key is unused, might need it later?
			std::string value = pack->GetValue();
			SerializerIn serializer(value);
			T* ptr = (T*)object;
			serializer.IO((*ptr));
			return BuildStatus::BUILD_OKAY;
		}
		else
		{
			return BuildStatus::BUILD_ERROR;
		}
	};

	virtual Packer::BuildPack Package(const std::string& key, any_type* object)
	{
		std::string type = GetBuilderTypeName();
		std::string value;
		SerializerOut serializer(value);
		T* ptr = (T*)object;
		serializer.IO((*ptr));

		//always build ValuePack for base types
		std::shared_ptr<ValuePack> valpack = std::make_shared<ValuePack>();
		valpack->SetKey(key);
		valpack->SetType(type);
		valpack->SetValue(value);

		return { BUILD_OKAY, valpack };
	};
};
