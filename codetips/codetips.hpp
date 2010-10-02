#ifndef CODETIPS_CODETIPS_HPP
#define CODETIPS_CODETIPS_HPP

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

typedef List< Ref<MemberTip, Owner> > MemberTipList;

class ClassTip: public Instance
{
public:
	ClassTip(Ref<MemberTipList> list)
		: memberTipList_(list)
	{}
	
	Ref<MemberTipList, Owner> memberTipList_;
};

class Context: public Instance
{
public:
	virtual String path() const = 0;
	virtual String language() const = 0;
	
	virtual String text() const = 0;
	virtual String lineText() const = 0;
	
	virtual off_t cursorByte() const = 0;
	virtual off_t cursorChar() const = 0;
	virtual int cursorLine() const = 0;
	virtual int cursorColumn() const = 0;
	
	virtual void moveCursor(int delta) = 0;
	virtual void select(int index0, int index1) = 0;
	virtual String copy(int index0, int index1) = 0;
	virtual void cut(int index0, int index1) = 0;
	virtual void paste(String text) = 0;
};

} // namespace codetips

#endif // CODETIPS_CODETIPS_HPP
