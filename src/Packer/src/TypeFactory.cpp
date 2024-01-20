#include "TypeFactory.h"

TypeFactory* TypeFactory::m_Instance = NULL;

void TypeFactory::AddBuilderInternal(TypeBuilder* Builder)
{
  m_TypeBuilders.push_back(Builder);
};

TypeBuilder* TypeFactory::FindBuilder(long typeID)
{
  TypeBuilder* ret = NULL;
  std::vector<TypeBuilder*>::iterator iter;
  for(iter = m_TypeBuilders.begin(); iter != m_TypeBuilders.end(); iter++)
  {
    if((*iter)->IsBuilderType(typeID))
    {
      ret = (*iter);
      break;
    }
  }
  return ret;
};
TypeBuilder* TypeFactory::FindBuilder(std::string TypeName)
{
  TypeBuilder* ret = NULL;
  std::vector<TypeBuilder*>::iterator iter;
  for(iter = m_TypeBuilders.begin(); iter != m_TypeBuilders.end(); iter++)
  {
    if((*iter)->IsBuilderTypeName(TypeName))
    {
      ret = (*iter);
      break;
    }
  }
  return ret;
};

void TypeFactory::AddDefaultFactories(bool basetypes, bool vectortypes)
{
  GetInstance()->AddStandardFactories(basetypes, vectortypes);
};

TypeFactory::TypeFactory()
{
};

void TypeFactory::AddStandardFactories(bool basetypes, bool vectortypes)
{
  //all factories
  TypeBuilder* base = 0;
  long id = 0;

  if(basetypes | vectortypes)
  {
    base = new BaseTypeBuilder<bool>;
    id = TypeFactory::RegisterNewFactory((bool*)0, "BOOL", base);

    base = new BaseTypeBuilder<char>;
    id = TypeFactory::RegisterNewFactory((char*)0, "CHAR", base);
  
    base = new BaseTypeBuilder<unsigned char>;
    id = TypeFactory::RegisterNewFactory((unsigned char*)0, "UCHAR", base);

    base = new BaseTypeBuilder<short>;
    id = TypeFactory::RegisterNewFactory((short*)0, "SHORT", base);

    base = new BaseTypeBuilder<unsigned short>;
    id = TypeFactory::RegisterNewFactory((unsigned short*)0, "USHORT", base);

    base = new BaseTypeBuilder<int>;
    id = TypeFactory::RegisterNewFactory((int*)0, "INT", base);

    base = new BaseTypeBuilder<unsigned int>;
    id = TypeFactory::RegisterNewFactory((unsigned int*)0, "UINT", base);

    base = new BaseTypeBuilder<long>;
    id = TypeFactory::RegisterNewFactory((long*)0, "LONG", base);

    base = new BaseTypeBuilder<unsigned long>;
    id = TypeFactory::RegisterNewFactory((unsigned long*)0, "ULONG", base);

    base = new BaseTypeBuilder<float>;
    id = TypeFactory::RegisterNewFactory((float*)0, "FLOAT", base);

    base = new BaseTypeBuilder<double>;
    id = TypeFactory::RegisterNewFactory((double*)0, "DOUBLE", base);

    base = new BaseTypeBuilder<std::string>;
    id = TypeFactory::RegisterNewFactory((std::string*)0, "STRING", base);
  }
  if(vectortypes)
  {
    base = new VectorTypeBuilder<bool>;
    id = TypeFactory::RegisterNewFactory((std::vector<bool>*)0, "STLVECTOR_BOOL", base);

    base = new VectorTypeBuilder<char>;
    id = TypeFactory::RegisterNewFactory((std::vector<char>*)0, "STLVECTOR_CHAR", base);

    base = new VectorTypeBuilder<unsigned char>;
    id = TypeFactory::RegisterNewFactory((std::vector<unsigned char>*)0, "STLVECTOR_UCHAR", base);

    base = new VectorTypeBuilder<short>;
    id = TypeFactory::RegisterNewFactory((std::vector<short>*)0, "STLVECTOR_SHORT", base);

    base = new VectorTypeBuilder<unsigned short>;
    id = TypeFactory::RegisterNewFactory((std::vector<unsigned short>*)0, "STLVECTOR_USHORT", base);

    base = new VectorTypeBuilder<int>;
    id = TypeFactory::RegisterNewFactory((std::vector<int>*)0, "STLVECTOR_INT", base);

    base = new VectorTypeBuilder<unsigned int>;
    id = TypeFactory::RegisterNewFactory((std::vector<unsigned int>*)0, "STLVECTOR_UINT", base);

    base = new VectorTypeBuilder<long>;
    id = TypeFactory::RegisterNewFactory((std::vector<long>*)0, "STLVECTOR_LONG", base);

    base = new VectorTypeBuilder<unsigned long>;
    id = TypeFactory::RegisterNewFactory((std::vector<unsigned long>*)0, "STLVECTOR_ULONG", base);

    base = new VectorTypeBuilder<float>;
    id = TypeFactory::RegisterNewFactory((std::vector<float>*)0, "STLVECTOR_FLOAT", base);

    base = new VectorTypeBuilder<double>;
    id = TypeFactory::RegisterNewFactory((std::vector<double>*)0, "STLVECTOR_DOUBLE", base);

    base = new VectorTypeBuilder<std::string>;
    id = TypeFactory::RegisterNewFactory((std::vector<std::string>*)0, "STLVECTOR_STRING", base);

  }
};

TypeFactory::~TypeFactory()
{
  //delete all factories
  std::vector<TypeBuilder*>::iterator iter;
  for(iter = m_TypeBuilders.begin(); iter != m_TypeBuilders.end(); iter++)
  {
    delete (*iter);
    (*iter) = 0;
  }

  m_Instance = NULL;
};
TypeFactory* TypeFactory::GetInstance()
{
  if(m_Instance == NULL)
    m_Instance = new TypeFactory;
  return m_Instance;
};

void TypeFactory::DeleteInstance()
{
  if(m_Instance != NULL)
  {
    delete m_Instance;
  }
};

int TypeFactory::CallBuilder_BuildType(long TypeID, std::string key, any_type* object, Package pack)
{
  int ret = BUILD_OKAY;
  TypeBuilder* builder = FindBuilder(TypeID);
  if(builder != NULL)
  {
    std::string typen;
    pack->GetType(&typen); 
    if(builder->IsBuilderTypeName(typen)) //check for correct ptr and pack type
    {
      ret = builder->BuildType(key, object, pack);
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
int TypeFactory::CallBuilder_BuildPack(long TypeID, std::string key, any_type* object, Package* pack)
{
  int ret = BUILD_OKAY;
  TypeBuilder* builder = FindBuilder(TypeID);
  if(builder != NULL)
  {
    ret = builder->BuildPack(key, object, pack);
  }
  else
  {
    ret = BUILD_ERROR;
  }
  return ret;
};