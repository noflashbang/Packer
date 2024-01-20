#ifndef TYPEFACTORY_H
#define TYPEFACTORY_H

#include <vector>

#include "Type.h"
#include "Pack.h"
#include "TypeReg.h"

class TypeFactory
{
public:

  ~TypeFactory();

  static TypeFactory* GetInstance();
  static void DeleteInstance();

  template<typename T>
  static long RegisterNewFactory(T* PointerOfType, std::string TypeName, TypeBuilder* Builder)
  {
    TypeRegistration typereg;
    TypeRegister::RegisterTypeInfo(PointerOfType, TypeName);
    TypeRegister::GetTypeRegister(PointerOfType, &typereg);
    Builder->SetTypeRegistration(typereg);
    GetInstance()->AddBuilderInternal(Builder);
    return typereg.GetTypeID();
  };

  template<typename T>
  static int BuildTypeFromPackage(std::string key, T* object, Package pack)
  {
    long tobuild = TypeRegister::GetTypeID(object);
    Package found = NULL;
    int located = pack->FindKeyShallow(key, TypeRegister::GetTypeName(object), &found); 
    if(!located)
      return BUILD_ERROR;
    
    return GetInstance()->CallBuilder_BuildType(tobuild, key, object, found);
  };
 
  template<typename T>
  static int BuildPackageFromType(std::string key, T* object, Package* pack)
  {
    long tobuild = TypeRegister::GetTypeID(object);
    return GetInstance()->CallBuilder_BuildPack(tobuild, key, object, pack);
  };

  template<typename T>
  static int BuildTypeFromString(std::string key, T* object, std::string objString)
  {
    long tobuild = TypeRegister::GetTypeID(object);
    int err = 0;
    Package pack = NULL;
    err = BasePack::FromStream(&objString, &pack);
    if(err != 1)
    {
      delete pack;
      return err;
    }

    Package found = NULL;
    int located = pack->FindKeyShallow(key, TypeRegister::GetTypeName(object), &found); //only look one deep
    if(!located)
      return BUILD_ERROR;

    err = GetInstance()->CallBuilder_BuildType(tobuild, key, object, found);
    delete pack;
    return err;
  };

  template<typename T>
  static int BuildStringFromType(std::string key, T* object, std::string* pObjString)
  {
    long tobuild = TypeRegister::GetTypeID(object);
    int err = 0;
    Package pack = NULL;
    err = GetInstance()->CallBuilder_BuildPack(tobuild, key, object, &pack);
    if(err != BUILD_OKAY)
    {
      delete pack;
      return err;
    }

    err = pack->GetStreamEscape(pObjString);
    delete pack;
    return err;
  };

  static void AddDefaultFactories(bool basetypes, bool vectortypes);

private:
  TypeFactory();
  
  void AddStandardFactories(bool basetypes, bool vectortypes);
  void AddBuilderInternal(TypeBuilder* Builder);
  TypeBuilder* FindBuilder(long TypeID);
  TypeBuilder* FindBuilder(std::string TypeName);
  int CallBuilder_BuildType(long typeID, std::string key, any_type* object, Package pack);
  int CallBuilder_BuildPack(long typeID, std::string key, any_type* object, Package* pack);

  static TypeFactory* m_Instance;
  std::vector<TypeBuilder*> m_TypeBuilders;

};
#endif