/*
 * CxxAutotext.hpp -- C++ autotext generator
 *
 * Copyright (c) 2010, Frank Mertens
 *
 * See ../COPYING for the license.
 */
#ifndef CODETIPS_CXXAUTOTEXT_HPP
#define CODETIPS_CXXAUTOTEXT_HPP

#include "codetips.hpp"

namespace codetips
{

CODETIPS_REGISTRATION_HEADER(CxxAutotext)

class CxxAutotext: public Assistant {
public:
	CxxAutotext();
	
	virtual String language() const;
	virtual String name() const;
	virtual String displayName() const;
	virtual String description() const;
	virtual Ref<Tip, Owner> assist(Ref<Context> context, int modifiers, uchar_t key);
};

} // namespace codetips

#endif // CODETIPS_CXXAUTOTEXT_HPP
