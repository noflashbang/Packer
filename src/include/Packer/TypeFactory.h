#pragma once

#include <vector>
#include <type_traits>

#include "Pack.h"
#include "TypeReg.h"
#include "TypeBuilder.h"


//forward
class TypeBuilder;

class TypeFactory
{
public:
	TypeFactory();
	~TypeFactory();

	template<typename T, typename B>
	long RegisterNewFactory(const std::string& typeName)
	{
		static_assert(std::is_base_of<TypeBuilder, B>::value, "B Type must be derived from TypeBuilder");
		
		T* pointerOfType = nullptr;

		std::unique_ptr<TypeBuilder> builder = std::make_unique<B>(this);
		TypeRegistration typereg;
		TypeRegister::RegisterTypeInfo(pointerOfType, typeName);
		TypeRegister::GetTypeRegister(pointerOfType, &typereg);
		builder->SetTypeRegistration(typereg);
		AddBuilderInternal(builder);
		return typereg.GetTypeID();
	};

	template<typename T>
	int BuildTypeFromPackage(const std::string& key, T* object, IPack* pack)
	{
		long tobuild = TypeRegister::GetTypeID(object);
		IPack* found = NULL;
		int located = pack->FindKeyShallow(key, TypeRegister::GetTypeName(object), &found);
		if (!located)
			return BUILD_ERROR;

		return CallBuilder_Unpackage(tobuild, key, object, found);
	};

	template<typename T>
	int BuildPackageFromType(std::string key, T* object, IPack** pack)
	{
		long tobuild = TypeRegister::GetTypeID(object);
		return CallBuilder_Package(tobuild, key, object, pack);
	};

	template<typename T>
	int BuildTypeFromString(std::string key, T* object, std::string objString)
	{
		long tobuild = TypeRegister::GetTypeID(object);
		int err = 0;
		IPack* pack = NULL;
		err = IPack::FromStream(objString, &pack);
		if (err != 1)
		{
			delete pack;
			return err;
		}

		IPack* found = NULL;
		int located = pack->FindKeyShallow(key, TypeRegister::GetTypeName(object), &found); //only look one deep
		if (!located)
			return BUILD_ERROR;

		err = CallBuilder_Unpackage(tobuild, key, object, found);
		delete pack;
		return err;
	};

	template<typename T>
	int BuildStringFromType(std::string key, T* object, std::string* pObjString)
	{
		long tobuild = TypeRegister::GetTypeID(object);
		int err = 0;
		IPack* pack = NULL;
		err = CallBuilder_Package(tobuild, key, object, &pack);
		if (err != BUILD_OKAY)
		{
			delete pack;
			return err;
		}

		*pObjString = pack->GetStreamEscape();
		delete pack;
		return err;
	};

	void AddDefaultFactories();

private:
	
	void AddBuilderInternal(std::unique_ptr<TypeBuilder>& builder);
	TypeBuilder* FindBuilder(long TypeID);
	TypeBuilder* FindBuilder(std::string TypeName);
	int CallBuilder_Unpackage(long typeID, std::string key, any_type* object, IPack* pack);
	int CallBuilder_Package(long typeID, std::string key, any_type* object, IPack** pack);

	std::vector<std::unique_ptr<TypeBuilder>> m_TypeBuilders;
};
