#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <vector>
#include "Serial.h"
#include "Pack.h"
#include "TypeReg.h"

#pragma warning(disable: 4786)

typedef void any_type;

#define BUILD_OKAY 1
#define BUILD_ERROR -1

class TypeBuilder
{
public:
  TypeBuilder();
  virtual ~TypeBuilder(){};

  virtual int BuildType(std::string key, any_type* object, Package pack) = 0;
  virtual int BuildPack(std::string key, any_type* object, Package* pack) = 0;
  void SetTypeRegistration(TypeRegistration type);
  long GetBuilderTypeID();
  std::string GetBuilderTypeName();
  bool IsBuilderType(long ID);
  bool IsBuilderTypeName(std::string name);

private:
  TypeRegistration m_TypeReg;
};

template <typename T>
class BaseTypeBuilder : public TypeBuilder
{
public:
  BaseTypeBuilder(){};
  virtual ~BaseTypeBuilder(){};

  virtual int BuildType(std::string key, any_type* object, Package pack)
  {
    int err = BUILD_OKAY;
    if(pack)
    {
      //key is unused, might need it later?
      std::string value;
      pack->GetValue(&value);
      CTextFBSerializerI serializer((char*)value.c_str(), value.size());
      T* ptr = (T*)object;
      serializer.IO((*ptr));
    }
    else
    {
      err = BUILD_ERROR;
    }
    return err;
  };

  virtual int BuildPack(std::string key, any_type* object, Package* pack)
  {
    std::string type = GetBuilderTypeName();
    std::string value;
    char valuestr[1024];
    memset(valuestr, 0, 1024);
    CTextFBSerializerO serializer(valuestr, 1024);
    T* ptr = (T*)object;
    serializer.IO((*ptr));
    value = valuestr;

    //always build ValuePack for base types
    ValuePack* valpack = new ValuePack();
    valpack->SetKey(key);
    valpack->SetType(type);
    valpack->SetValue(value);

    (*pack) = valpack;
    return BUILD_OKAY;
  };
};

template <typename T>
class VectorTypeBuilder : public TypeBuilder
{
public:
  VectorTypeBuilder()
  {
  };
  virtual ~VectorTypeBuilder(){};

  virtual int BuildType(std::string key, any_type* object, Package pack)
  {
    int err = BUILD_OKAY;
    std::vector<T>* vector = (std::vector<T>*)object;
    std::string type = GetBuilderTypeName();
   
    if(pack->HasChildren())
    {
      std::map<std::string, BasePack* > childmap;
      pack->GetChildMap(&childmap);
      std::map<std::string, BasePack* >::iterator iter;

      unsigned long size = 0;
      iter = childmap.find("SIZE");
      if(iter != childmap.end())
      {
        err = TypeFactory::BuildTypeFromPackage("SIZE", &size, iter->second);
        if(err != BUILD_OKAY)
          return err;
      }
      for(unsigned long iter2 = 0; iter2 < size; iter2++)
      {
        char numstr[10];
        memset(numstr, 0, 10);
        _itoa_s(iter2, numstr, 10, 10);
            
        T realelem;
        iter = childmap.find(numstr);
        if(iter != childmap.end())
        {
          err = TypeFactory::BuildTypeFromPackage(numstr, &realelem, iter->second);
          if(err != BUILD_OKAY)
            return err;

          vector->push_back(realelem);
        }
        else
        {
          T hold(0);
          vector->push_back(hold);
        }
      }
    }
    return err;
  };

  virtual int BuildPack(std::string key, any_type* object, Package* pack)
  {
    int err = BUILD_OKAY;
    std::vector<T>* vector = (std::vector<T>*)object;
    MultiPack* buildpack = new MultiPack;

    //add size
    unsigned long size = vector->size();
    Package sizepack;
    err = TypeFactory::BuildPackageFromType("SIZE", &size, &sizepack);
    buildpack->AddChild(sizepack);

    //now add all the elements
    for(unsigned long iter =0; iter < size; iter++)
    {
      char numstr[10];
      memset(numstr, 0, 10);
      _itoa_s(iter, numstr, 10, 10);
      Package elempack;
      err = TypeFactory::BuildPackageFromType(numstr, &((*vector)[iter]), &elempack);
      if(err == BUILD_OKAY)
      {
        buildpack->AddChild(elempack);
      }
      else
      {
        break;
      }
    }
    buildpack->SetKey(key);

    std::string type = GetBuilderTypeName();
    buildpack->SetType(type);

    (*pack) = buildpack;
    return err;
  };
};

#endif