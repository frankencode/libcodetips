/*
 * CxxAutotext.cpp -- C++ autotext generator
 *
 * Copyright (c) 2010, Frank Mertens
 *
 * See ../COPYING for the license.
 */

#include <ftl/PrintDebug.hpp>
#include "AssistantManager.hpp"
#include "CxxAutotext.hpp"

namespace codetips
{

CODETIPS_REGISTRATION_IMPL(CxxAutotext)

CxxAutotext::CxxAutotext()
{
	printTo(log(), "All fine.\n");
}

String CxxAutotext::language() const { return "cxx"; }
String CxxAutotext::name() const { return "autotext"; }
String CxxAutotext::displayName() const { return "C++ Autotext"; }

String CxxAutotext::description() const
{
	return "Automatically inserts frequently used mandatory redundancies.";
}

Ref<Tip, Owner> CxxAutotext::assist(Ref<Context> context, int modifiers, uchar_t key)
{
	if (key == '{') {
		bool appendSemicolon = false;
		if (context->line() > 0) {
			String currLine = context->copyLine(context->line());
			String prevLine = context->copyLine(context->line() - 1);
			appendSemicolon =
				currLine.contains("class") || currLine.contains("struct") ||
				prevLine.contains("class") || prevLine.contains("struct");
		}
		if (appendSemicolon)
			context->insert("{};");
		else
			context->insert("{}");
		context->move(1);
	}
	/*else if (key == '(') {
		context->insert("()");
		context->move(1);
	}
	else if (key == '[') {
		context->insert("[]");
		context->move(1);
	}*/
	else if (key == '\n') {
		String currLine = context->copyLine(context->line());
		if ((currLine.length() > 0) && (context->linePos() > 0)) {
			uchar_t ch = currLine.get(currLine.first() + context->linePos() - 1);
			bool autoIndent = false;
			if (ch == '{') {
				String prevLine = context->copyLine(context->line() - 1);
				autoIndent = !(
					currLine.contains("class") || currLine.contains("struct") ||
					prevLine.contains("class") || prevLine.contains("struct")
				);
				context->insert("\n");
				if (autoIndent) {
					context->insert(context->indent());
					context->insert("\n");
					context->move(context->indent().length() + 1);
				}
				else {
					context->insert("\n");
					context->move(1);
				}
			}
			else if (ch == ':') {
				context->insert(context->indent());
				context->insert("\n");
				context->move(context->indent().length() + 1);
			}
		}
	}
	return 0;
}

} // namespace codetips
