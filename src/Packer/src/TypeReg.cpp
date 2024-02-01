#include "TypeReg.h"

long TypeRegistration::m_NextID = 1;

TypeRegistration::TypeRegistration()
{
	m_TypeID = -1;
	m_TypeName = "VOID";
};
TypeRegistration::TypeRegistration(std::string TypeName)
{
	m_TypeID = GetNextID();
	MoveNextID();
	m_TypeName = TypeName;
	std::transform(m_TypeName.begin(), m_TypeName.end(), m_TypeName.begin(), ::toupper);
};
TypeRegistration::TypeRegistration(const TypeRegistration& typereg)
{
	m_TypeID = typereg.m_TypeID;
	m_TypeName = typereg.m_TypeName;
};
TypeRegistration& TypeRegistration::operator=(const TypeRegistration& other)
{
	if (this != &other)
	{
		m_TypeID = other.m_TypeID;
		m_TypeName = other.m_TypeName;
	}
	return (*this);
};

TypeRegistration::~TypeRegistration()
{
	//nothing
};
long TypeRegistration::GetTypeID()
{
	return m_TypeID;
};
std::string TypeRegistration::GetTypeName()
{
	return m_TypeName;
};
bool TypeRegistration::IsType(long ID)
{
	return (ID == m_TypeID);
};
bool TypeRegistration::IsTypeName(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::toupper);
	return (name == m_TypeName);
};
long TypeRegistration::GetNextID()
{
	return m_NextID;
};
void TypeRegistration::MoveNextID()
{
	m_NextID++;
};