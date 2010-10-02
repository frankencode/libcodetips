#ifndef CODETIPS_HAXEINSTRUCTOR_HPP
#define CODETIPS_HAXEINSTRUCTOR_HPP

#include "codetips.hpp"

namespace ftl {
	class ProcessFactory;
}

namespace codetips
{

CODETIPS_REGISTRATION_HEADER(HaxeInstructor)

class HaxeMessageSyntax;

class HaxeInstructor: public Instructor {
public:
	HaxeInstructor();
	
	virtual String language() const;
	virtual String description() const;
	virtual Ref<Instance, Owner> assist(Ref<Context> context, int modifiers, uchar_t key);
	
private:
	Ref<ProcessFactory, Owner> processFactory_;
	Ref<HaxeMessageSyntax, Owner> messageSyntax_;
};

} // namespace codetips

#endif // CODETIPS_HAXEINSTRUCTOR_HPP
