#ifndef TYPEREG_H
#define TYPEREG_H

#include <string>
#include <vector>

inline void TR_StringToUpper(std::string* pString)
{
  for(unsigned aa=0;aa<pString->size(); aa++)
  {
    char current = pString->at(aa);
    if(current >= 'a' && current <= 'z')
    {
      current -= ' '; //-32
      pString->at(aa) = current;
    }
  }
};

//forwards
class TypeRegister;

class TypeRegistration
{
  friend class TypeRegister;
public:
  TypeRegistration();
  TypeRegistration(const TypeRegistration& typereg);
  TypeRegistration& operator=(const TypeRegistration& other);

  ~TypeRegistration();

  long GetTypeID();
  std::string GetTypeName();

  bool IsType(long ID);
  bool IsTypeName(std::string name);

    
private:
  TypeRegistration(std::string TypeName);
  long GetNextID();
  void MoveNextID();
  
  static long m_NextID;
  
  long m_TypeID;
  std::string m_TypeName;
};

/*
 This whole class is pretty hacky,
 but it was the only way I could think of
 to automate the type registration process.
*/
class TypeRegister
{
public:

  template <typename T> 
  static long GetTypeID(T* null)
  {
    TypeRegistration type;
    RegisterOrGetType(null, &type);
    return type.GetTypeID();
  };

  template <typename T> 
  static std::string GetTypeName(T* null)
  {
    TypeRegistration type;
    RegisterOrGetType(null, &type);
    return type.GetTypeName();
  };

  template <typename T> 
  static void RegisterTypeInfo(T* null, std::string Name)
  {
    TypeRegistration type(Name);
    RegisterOrGetType(null, &type);
  };

  template <typename T> 
  static void GetTypeInfo(T* null, std::string* Name, long* ID)
  {
    TypeRegistration hold;
    RegisterOrGetType(null, &hold);
    (*Name) = hold.GetTypeName();
    (*ID) = hold.GetTypeID();
  };

  template <typename T> 
  static void GetTypeRegister(T* null, TypeRegistration* typereg)
  {
    RegisterOrGetType(null, typereg);
  };

private:
  template <typename T> 
  static void RegisterOrGetType(T* null, TypeRegistration* tr)
  {
    /*Originally this code used a static class rather than a static pointer
     I had to change this because the static class was being reinitialized on the 
     third call to this function(for a specific type) thus overwriting the correct id and name.
     This is a bug in VC6.0
    */
    //static TypeRegistration* typereg = NULL;
    //if(typereg == NULL)
    //{
    //  typereg = new TypeRegistration((*tr)); 
    //  DeleteRegistration::AddRegistrationForDelete(&typereg);
    //}
    //if(typereg->GetTypeID() == -1)
    //{
    //  (*typereg) = (*tr);
    //}
    //(*tr) = (*typereg);

    //Original code(works in VS2008!)
    static TypeRegistration typereg;
    if(typereg.GetTypeID() == -1)
    {
      typereg = (*tr);
    }
    (*tr) = typereg;
  };
};

#endif