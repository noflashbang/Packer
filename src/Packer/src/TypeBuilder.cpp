#include "TypeBuilder.h"

TypeBuilder::TypeBuilder(TypeFactory* pTypeFactory)
{
	m_TypeFactory = pTypeFactory;
};
void TypeBuilder::SetTypeRegistration(TypeRegistration type)
{
	m_TypeReg = type;
};
long TypeBuilder::GetBuilderTypeID()
{
	return m_TypeReg.GetTypeID();
}
std::string TypeBuilder::GetBuilderTypeName()
{
	return m_TypeReg.GetTypeName();
};
bool TypeBuilder::IsBuilderType(long ID)
{
	return m_TypeReg.IsType(ID);
};
bool TypeBuilder::IsBuilderTypeName(std::string name)
{
	return m_TypeReg.IsTypeName(name);
};