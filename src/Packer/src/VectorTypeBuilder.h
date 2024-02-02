#pragma once

#include <string>
#include <vector>

#include "SerialIn.h"
#include "SerialOut.h"
#include "Pack.h"
#include "TypeReg.h"
#include "TypeBuilder.h"
#include "TypeFactory.h"



template <typename T>
class VectorTypeBuilder : public TypeBuilder
{
public:
	VectorTypeBuilder(TypeFactory* pTypeFactory) : TypeBuilder(pTypeFactory) {};
	virtual ~VectorTypeBuilder() {};

	virtual int BuildType(std::string key, any_type* object, Package pack)
	{
		int err = BUILD_OKAY;
		std::vector<T>* vector = (std::vector<T>*)object;
		std::string type = GetBuilderTypeName();

		if (pack->HasChildren())
		{
			std::map<std::string, IPack* > childmap;
			pack->GetChildMap(&childmap);
			std::map<std::string, IPack* >::iterator iter;

			unsigned long size = 0;
			iter = childmap.find("SIZE");
			if (iter != childmap.end())
			{
				err = m_TypeFactory->BuildTypeFromPackage("SIZE", &size, iter->second);
				if (err != BUILD_OKAY)
					return err;
			}
			for (unsigned long iter2 = 0; iter2 < size; iter2++)
			{
				char numstr[10];
				memset(numstr, 0, 10);
				_itoa_s(iter2, numstr, 10, 10);

				T realelem;
				iter = childmap.find(numstr);
				if (iter != childmap.end())
				{
					err = m_TypeFactory->BuildTypeFromPackage(numstr, &realelem, iter->second);
					if (err != BUILD_OKAY)
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
		err = m_TypeFactory->BuildPackageFromType("SIZE", &size, &sizepack);
		buildpack->AddChild(sizepack);

		//now add all the elements
		for (unsigned long iter = 0; iter < size; iter++)
		{
			char numstr[10];
			memset(numstr, 0, 10);
			_itoa_s(iter, numstr, 10, 10);
			Package elempack;

			auto elem = vector->at(iter);
			
			err = m_TypeFactory->BuildPackageFromType(numstr, &elem, &elempack);
			if (err == BUILD_OKAY)
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
