#pragma once

#include <vector>
#include <type_traits>
#include <memory>

#include "Packer.h"
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
		//help get a better error message
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
	int BuildTypeFromPackage(const std::string& key, T* object, std::weak_ptr<IPack> pack)
	{
		long tobuild = TypeRegister::GetTypeID(object);
		auto current = pack.lock();
		if (current == nullptr)
		{
			return BUILD_ERROR;
		}
		if (current->IsKey(key, TypeRegister::GetTypeName(object)))
		{
			return CallBuilder_Unpackage(tobuild, key, object, current);
		}
		else
		{
			current = current->FindKeyShallow(key, TypeRegister::GetTypeName(object));
			if (current == nullptr)
			{
				return BUILD_ERROR;
			}
			return CallBuilder_Unpackage(tobuild, key, object, current);
		}
	};

	template<typename T>
	Packer::BuildPack BuildPackageFromType(std::string key, T* object)
	{
		long tobuild = TypeRegister::GetTypeID(object);
		return CallBuilder_Package(tobuild, key, object);
	};

	template<typename T>
	int BuildTypeFromString(std::string key, T* object, std::string objString)
	{
		long tobuild = TypeRegister::GetTypeID(object);
		int err = 0;
		
		Packer::BuildPack package = Packer::FromStream(objString);
		if (package.Status != BuildStatus::BUILD_OKAY)
		{
			return package.Status;
		}

		//std::shared_ptr<IPack> found = package.Package->FindKeyShallow(key, TypeRegister::GetTypeName(object)); //only look one deep
		//if (found == nullptr)
		//{
		//	return BuildStatus::BUILD_ERROR;
		//}			

		err = CallBuilder_Unpackage(tobuild, key, object, package.Package);
		return err;
	};

	template<typename T>
	std::string BuildStringFromType(std::string key, T* object)
	{
		long tobuild = TypeRegister::GetTypeID(object);
		Packer::BuildPack pack = CallBuilder_Package(tobuild, key, object);
		if (pack.Status != BuildStatus::BUILD_OKAY)
		{
			return "";
		}
		return pack.Package->GetStreamEscape();
	};

	void AddDefaultFactories();

private:
	
	void AddBuilderInternal(std::unique_ptr<TypeBuilder>& builder);
	TypeBuilder* FindBuilder(long TypeID);
	TypeBuilder* FindBuilder(std::string TypeName);
	int CallBuilder_Unpackage(long typeID, std::string key, any_type* object, std::shared_ptr<IPack> pack);
	Packer::BuildPack CallBuilder_Package(long typeID, std::string key, any_type* object);

	std::vector<std::unique_ptr<TypeBuilder>> m_TypeBuilders;
};
