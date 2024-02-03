#pragma once

#include <string>
#include <vector>
#include <algorithm>

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

	long GetTypeID() const;
	std::string GetTypeName() const;

	bool IsType(long ID) const;
	bool IsTypeName(const std::string& name) const;

private:
	TypeRegistration(std::string TypeName);
	long GetNextID();
	void MoveNextID();

	static long m_NextID;

	long m_TypeID;
	std::string m_TypeName;
};

/*
 This whole class is pretty hacky, but I love it all the same.
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
		static TypeRegistration typereg;
		if (typereg.GetTypeID() == -1)
		{
			typereg = (*tr);
		}
		(*tr) = typereg;
	};
};
