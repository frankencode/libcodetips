/*
 * XmlAutotext.hpp -- XML autotext generator
 *
 * Copyright (c) 2010, Frank Mertens
 *
 * See ../COPYING for the license.
 */
#ifndef CODETIPS_XMLAUTOTEXT_HPP
#define CODETIPS_XMLAUTOTEXT_HPP

namespace codetips
{

CODETIPS_REGISTRATION_HEADER(XmlAutotext)

class XmlOpenTagSyntax;

class XmlAutotext: public Assistant
{
public:
	XmlAutotext();
	
	String language() const;
	String name() const;
	String displayName() const;
	String description() const;
	Ref<Tip, Owner> assist(Ref<Context> context, int modifiers, uchar_t key);

private:
	Ref<XmlOpenTagSyntax, Owner> openTagSyntax_;
};

} // namespace codetips

#endif // CODETIPS_XMLAUTOTEXT_HPP
