#ifndef CODETIPS_HAXEMESSAGESYNTAX_HPP
#define CODETIPS_HAXEMESSAGESYNTAX_HPP

#include <ftl/Syntax.hpp>
#include <ftl/Array.hpp>

namespace codetips
{

using namespace ftl;

class HaxeMessageSyntax: public Syntax<ByteArray>::Definition
{
public:
	HaxeMessageSyntax();
	
	Ref<Instance, Owner> parse(String message);
	
private:
	String readValue(String message, Ref<Token> token);
	
	int gt_, lt_;
	int specialChar_;
	int value_;
	int typeTip_;
	int memberName_;
	int memberType_;
	int memberDescription_;
	int memberTip_;
	int classTip_;
	int haxeTip_;
};

} // namespace codetips

#endif // CODETIPS_HAXEMESSAGESYNTAX_HPP
