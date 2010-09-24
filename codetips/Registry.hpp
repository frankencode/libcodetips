#ifndef CODETIPS_REGISTRY_HPP
#define CODETIPS_REGISTRY_HPP

#include <ftl/PrefixTree.hpp>
#include <ftl/Singleton.hpp>
#include "generics.hpp"

namespace codetips
{

class Registry: public Instance, public Singleton<Registry>
{
public:
	void add(Ref<Factory> factory);
	Ref<Factory> factoryByName(String name) const;
	
private:
	friend class Singleton<Registry>;
	Registry();
	
	typedef PrefixTree<char, Ref<Factory, Owner> > FactoryByName;
	Ref<FactoryByName, Owner> factoryByName_;
};

} // namespace codetips

#endif // CODETIPS_REGISTRY_HPP
