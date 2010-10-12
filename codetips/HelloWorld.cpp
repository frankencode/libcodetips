#include <ftl/PrintDebug.hpp>
#include "Supervisor.hpp"
#include "HelloWorld.hpp"

namespace codetips
{

CODETIPS_REGISTRATION_IMPL(HelloWorld)

HelloWorld::HelloWorld()
{
	printTo(log(), "All fine.\n");
}

String HelloWorld::language() const { return "plain"; }
String HelloWorld::name() const { return "test"; }
String HelloWorld::displayName() const { return "Test Instructor"; }
String HelloWorld::description() const { return "Testing module. Says 'Hello' when pressing the magic key (SHIFT-Tab)."; }

Ref<Tip, Owner> HelloWorld::assist(Ref<Context> context, int modifiers, uchar_t key)
{
	Ref<Tip, Owner> tip;
	if ((modifiers == Shift) && (key == '\t'))
		tip = new TypeTip(new Type(String("Hello!")));
	return tip;
}

} // namespace codetips
