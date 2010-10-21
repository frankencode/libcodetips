/*
 * codetips.hpp -- generic types
 *
 * Copyright (c) 2010, Frank Mertens
 *
 * See ../COPYING for the license.
 */
#ifndef CODETIPS_CODETIPS_HPP
#define CODETIPS_CODETIPS_HPP

#include <ftl/atoms>
#include <ftl/String.hpp>
#include <ftl/List.hpp>

namespace codetips
{

using namespace ftl;

class Argument: public Instance
{
public:
	Argument(String name, String type)
		: name_(name),
		  type_(type)
	{}
	inline String name() const { return name_; }
	inline String type() const { return type_; }
private:
	String name_;
	String type_;
};

typedef Array< Ref<Argument, Owner> > Arguments;

class Type: public Instance
{
public:
	Type(String displayString, String type = "", Ref<Arguments> arguments = 0)
		: displayString_(displayString),
		  type_(type),
		  arguments_(arguments)
	{}
	inline String displayString() const { return displayString_; }
	inline String type() const { return type_; }
	inline Ref<Arguments> arguments() const { return arguments_; }
private:
	String displayString_;
	String type_;
	Ref<Arguments, Owner> arguments_;
};

class Member: public Instance
{
public:
	Member(String name, Ref<Type> type, String description = "")
		: name_(name),
		  type_(type),
		  description_(description)
	{}
	inline String name() const { return name_; }
	inline Ref<Type> type() const { return type_; }
	inline String description() const { return description_; }
private:
	String name_;
	Ref<Type, Owner> type_;
	String description_;
};

typedef Array< Ref<Member, Owner> > Members;

class Tip: public Instance
{
public:
	Tip(int typeId): typeId_(typeId) {}
	inline int typeId() const { return typeId_; }
private:
	int typeId_;
};

class TypeTip: public Tip
{
public:
	enum { Id = 0 };
	TypeTip(Ref<Type> type)
		: Tip(Id),
		  type_(type)
	{}
	inline Ref<Type> type() const { return type_; }
private:
	Ref<Type, Owner> type_;
};

class MembersTip: public Tip
{
public:
	enum { Id = 1 };
	MembersTip(Ref<Members> members)
		: Tip(Id),
		  members_(members)
	{}
	inline Ref<Members> members() const { return members_; }
private:
	Ref<Members, Owner> members_;
};

class Context: public Instance
{
public:
	virtual String path() const = 0;
	virtual String language() const = 0;
	
	virtual String text() const = 0;
	virtual String line(int yr = 0) const = 0;
	
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
