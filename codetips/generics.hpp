#ifndef CODETIPS_GENERICS_HPP
#define CODETIPS_GENERICS_HPP

#include <ftl/atoms>
#include <ftl/String.hpp>
#include <ftl/List.hpp>

namespace codetips
{

using namespace ftl;

class TypeTip: public Instance
{
public:
	TypeTip(String type): type_(type) {}
	String type_;
};

class MemberTip: public TypeTip
{
public:
	MemberTip(String name, String type, String description = "")
		: TypeTip(type),
		  name_(name),
		  description_(description)
	{}
	String name_;
	String description_;
};

class ClassTip: public Instance
{
public:
	List< Ref<MemberTip, Owner> > members_;
};

class Factory: public Instance
{
public:
	virtual String name() const = 0;
	virtual Ref<Instance> produce(String path, off_t offset) = 0;
};

} // namespace codetips

#endif // CODETIPS_GENERICS_HPP
