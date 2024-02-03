#include "TypeBuilder.h"

TypeBuilder::TypeBuilder(TypeFactory* pTypeFactory)
{
	m_TypeFactory = pTypeFactory;
};

void TypeBuilder::SetTypeRegistration(const TypeRegistration& typeReg)
{
	m_TypeReg = typeReg;
};

long TypeBuilder::GetBuilderTypeID() const
{
	return m_TypeReg.GetTypeID();
}
std::string TypeBuilder::GetBuilderTypeName() const
{
	return m_TypeReg.GetTypeName();
};
bool TypeBuilder::IsBuilderType(long ID) const
{
	return m_TypeReg.IsType(ID);
};
bool TypeBuilder::IsBuilderTypeName(const std::string& name) const
{
	return m_TypeReg.IsTypeName(name);
};