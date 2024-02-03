#pragma once

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <memory>


class IPack
{
public:
	IPack() {};
	virtual ~IPack() {};

	virtual std::string GetKey() const = 0;
	virtual std::string GetType()  const = 0;

	virtual std::string GetValueEscape()  const = 0;
	virtual std::string GetValue()  const = 0;

	virtual std::string GetStreamEscape()  const = 0;
	virtual std::string GetStream()  const = 0;

	virtual std::shared_ptr<IPack> FindKeyShallow(const std::string& key, const std::string& type) = 0;
	virtual std::shared_ptr<IPack> FindKeyDeep(const std::string& key, const std::string& type, int levels) = 0;
	virtual bool IsKey(const std::string& key, const std::string& type) = 0;

	virtual bool HasChildren() = 0;
	virtual void GetChildMap(std::map<std::string, std::weak_ptr<IPack>>* childMap) = 0;
};




