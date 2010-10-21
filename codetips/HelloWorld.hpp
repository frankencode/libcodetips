/*
 * HelloWorld.hpp -- dummy instructor for testing
 *
 * Copyright (c) 2010, Frank Mertens
 *
 * See ../COPYING for the license.
 */
#ifndef CODETIPS_HELLOINSTRUCTOR_HPP
#define CODETIPS_HELLOINSTRUCTOR_HPP

#include "codetips.hpp"

namespace codetips
{

CODETIPS_REGISTRATION_HEADER(HelloWorld)

class HelloWorld: public Instructor {
public:
	HelloWorld();
	
	virtual String language() const;
	virtual String name() const;
	virtual String displayName() const;
	virtual String description() const;
	virtual Ref<Tip, Owner> assist(Ref<Context> context, int modifiers, uchar_t key);
};

} // namespace codetips

#endif // CODETIPS_HELLOINSTRUCTOR_HPP
