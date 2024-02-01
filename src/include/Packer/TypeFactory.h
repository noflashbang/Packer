#pragma once

#include <vector>

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

	template<typename T>
	long RegisterNewFactory(T* PointerOfType, std::string TypeName, TypeBuilder* Builder)
	{
		TypeRegistration typereg;
		TypeRegister::RegisterTypeInfo(PointerOfType, TypeName);
		TypeRegister::GetTypeRegister(PointerOfType, &typereg);
		Builder->SetTypeRegistration(typereg);
		AddBuilderInternal(Builder);
		return typereg.GetTypeID();
	};

	template<typename T>
	int BuildTypeFromPackage(std::string key, T* object, Package pack)
	{
		long tobuild = TypeRegister::GetTypeID(object);
		Package found = NULL;
		int located = pack->FindKeyShallow(key, TypeRegister::GetTypeName(object), &found);
		if (!located)
			return BUILD_ERROR;

		return CallBuilder_BuildType(tobuild, key, object, found);
	};

	template<typename T>
	int BuildPackageFromType(std::string key, T* object, Package* pack)
	{
		long tobuild = TypeRegister::GetTypeID(object);
		return CallBuilder_BuildPack(tobuild, key, object, pack);
	};

	template<typename T>
	int BuildTypeFromString(std::string key, T* object, std::string objString)
	{
		long tobuild = TypeRegister::GetTypeID(object);
		int err = 0;
		Package pack = NULL;
		err = BasePack::FromStream(&objString, &pack);
		if (err != 1)
		{
			delete pack;
			return err;
		}

		Package found = NULL;
		int located = pack->FindKeyShallow(key, TypeRegister::GetTypeName(object), &found); //only look one deep
		if (!located)
			return BUILD_ERROR;

		err = CallBuilder_BuildType(tobuild, key, object, found);
		delete pack;
		return err;
	};

	template<typename T>
	int BuildStringFromType(std::string key, T* object, std::string* pObjString)
	{
		long tobuild = TypeRegister::GetTypeID(object);
		int err = 0;
		Package pack = NULL;
		err = CallBuilder_BuildPack(tobuild, key, object, &pack);
		if (err != BUILD_OKAY)
		{
			delete pack;
			return err;
		}

		err = pack->GetStreamEscape(pObjString);
		delete pack;
		return err;
	};

	void AddDefaultFactories(bool basetypes, bool vectortypes);

private:
	

	void AddStandardFactories(bool basetypes, bool vectortypes);
	void AddBuilderInternal(TypeBuilder* Builder);
	TypeBuilder* FindBuilder(long TypeID);
	TypeBuilder* FindBuilder(std::string TypeName);
	int CallBuilder_BuildType(long typeID, std::string key, any_type* object, Package pack);
	int CallBuilder_BuildPack(long typeID, std::string key, any_type* object, Package* pack);

	std::vector<TypeBuilder*> m_TypeBuilders;
};
