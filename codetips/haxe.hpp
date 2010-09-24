#ifndef CODETIPS_HAXE_HPP
#define CODETIPS_HAXE_HPP

#include "generics.hpp"
#include "Registry.hpp"

namespace ftl {
	class ProcessFactory;
}

namespace codetips
{

class HaxeFactory: public Factory {
public:
	HaxeFactory();
	
	String name() const;
	Ref<Instance> produce(String path, off_t offset);
	
private:
	Ref<ProcessFactory, Owner> processFactory_;
};

class HaxeInitializer {
public:
	HaxeInitializer() {
		Registry::instance()->add(new HaxeFactory);
	}
};

namespace { HaxeInitializer initializer_; }

} // namespace codetips

#endif // CODETIPS_HAXE_HPP
