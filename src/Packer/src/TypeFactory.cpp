#include "TypeFactory.h"

#include "BaseTypeBuilder.h"
#include "VectorTypeBuilder.h"

void TypeFactory::AddBuilderInternal(std::unique_ptr<TypeBuilder>& builder)
{
	m_TypeBuilders.push_back(std::move(builder));
};

TypeBuilder* TypeFactory::FindBuilder(long typeID)
{
	TypeBuilder* ret = NULL;
	std::vector<std::unique_ptr<TypeBuilder>>::iterator iter;
	for (iter = m_TypeBuilders.begin(); iter != m_TypeBuilders.end(); iter++)
	{
		if ((*iter)->IsBuilderType(typeID))
		{
			ret = (*iter).get();
			break;
		}
	}
	return ret;
};
TypeBuilder* TypeFactory::FindBuilder(std::string TypeName)
{
	TypeBuilder* ret = NULL;
	std::vector<std::unique_ptr<TypeBuilder>>::iterator iter;
	for (iter = m_TypeBuilders.begin(); iter != m_TypeBuilders.end(); iter++)
	{
		if ((*iter)->IsBuilderTypeName(TypeName))
		{
			ret = (*iter).get();
			break;
		}
	}
	return ret;
};

TypeFactory::TypeFactory()
{
};

void TypeFactory::AddDefaultFactories()
{
	long id = 0;
	id = RegisterNewFactory<bool, BaseTypeBuilder<bool>>("BOOL");
	id = RegisterNewFactory<char, BaseTypeBuilder<char>>("CHAR");
	id = RegisterNewFactory<unsigned char, BaseTypeBuilder<unsigned char>>("UCHAR");
	id = RegisterNewFactory<short, BaseTypeBuilder<short>>("SHORT");
	id = RegisterNewFactory<unsigned short, BaseTypeBuilder<unsigned short>>("USHORT");
	id = RegisterNewFactory<int, BaseTypeBuilder<int>>("INT");
	id = RegisterNewFactory<unsigned int, BaseTypeBuilder<unsigned int>>("UINT");
	id = RegisterNewFactory<long, BaseTypeBuilder<long>>("LONG");
	id = RegisterNewFactory<unsigned long, BaseTypeBuilder<unsigned long>>("ULONG");
	id = RegisterNewFactory<float, BaseTypeBuilder<float>>("FLOAT");
	id = RegisterNewFactory<double, BaseTypeBuilder<double>>("DOUBLE");
	id = RegisterNewFactory<std::string, BaseTypeBuilder<std::string>>("STRING");

	id = RegisterNewFactory<std::vector<bool>, VectorTypeBuilder<bool>>("BOOL_VEC");
	id = RegisterNewFactory<std::vector<char>, VectorTypeBuilder<char>>("CHAR_VEC");
	id = RegisterNewFactory<std::vector<unsigned char>, VectorTypeBuilder<unsigned char>>("UCHAR_VEC");
	id = RegisterNewFactory<std::vector<short>, VectorTypeBuilder<short>>("SHORT_VEC");
	id = RegisterNewFactory<std::vector<unsigned short>, VectorTypeBuilder<unsigned short>>("USHORT_VEC");
	id = RegisterNewFactory<std::vector<int>, VectorTypeBuilder<int>>("INT_VEC");
	id = RegisterNewFactory<std::vector<unsigned int>, VectorTypeBuilder<unsigned int>>("UINT_VEC");
	id = RegisterNewFactory<std::vector<long>, VectorTypeBuilder<long>>("LONG_VEC");
	id = RegisterNewFactory<std::vector<unsigned long>, VectorTypeBuilder<unsigned long>>("ULONG_VEC");
	id = RegisterNewFactory<std::vector<float>, VectorTypeBuilder<float>>("FLOAT_VEC");
	id = RegisterNewFactory<std::vector<double>, VectorTypeBuilder<double>>("DOUBLE_VEC");
	id = RegisterNewFactory<std::vector<std::string>, VectorTypeBuilder<std::string>>("STRING_VEC");
};

TypeFactory::~TypeFactory()
{
}

int TypeFactory::CallBuilder_Unpackage(long TypeID, std::string key, any_type* object, IPack* pPack)
{
	int ret = BUILD_OKAY;
	TypeBuilder* builder = FindBuilder(TypeID);
	if (builder != NULL)
	{
		std::string typen = pPack->GetType();
		if (builder->IsBuilderTypeName(typen)) //check for correct ptr and pack type
		{
			ret = builder->Unpackage(key, object, pPack);
		}
		else
		{
			ret = BUILD_ERROR;
		}
	}
	else
	{
		ret = BUILD_ERROR;
	}
	return ret;
};
int TypeFactory::CallBuilder_Package(long TypeID, std::string key, any_type* object, IPack** pack)
{
	int ret = BUILD_OKAY;
	TypeBuilder* builder = FindBuilder(TypeID);
	if (builder != NULL)
	{
		ret = builder->Package(key, object, pack);
	}
	else
	{
		ret = BUILD_ERROR;
	}
	return ret;
};