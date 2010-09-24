#include "Registry.hpp"

namespace codetips
{

Registry::Registry()
	: factoryByName_(new FactoryByName)
{}

void Registry::add(Ref<Factory> factory)
{
	factoryByName_->insert(factory->name(), factory);
}

Ref<Factory> Registry::factoryByName(String name) const
{
	Ref<Factory, Owner> factory;
	factoryByName_->lookup(name, &factory);
	return factory;
}

} // namespace codetips
