#include <ftl/PrintDebug.hpp> // debug
#include "codetips.hpp"
#include "HaxeMessageSyntax.hpp"

namespace codetips
{

HaxeMessageSyntax::HaxeMessageSyntax()
{
	DEFINE_VOID("SpaceChar", RANGE(" \n\r\t"));
	DEFINE_VOID("Whitespace", REPEAT(INLINE("SpaceChar")));
	
	specialChar_ =
		DEFINE("SpecialChar",
			GLUE(
				CHAR('&'),
				KEYWORD("gt lt"),
				CHAR(';')
			)
		);
	gt_ = keywordByName("gt");
	lt_ = keywordByName("lt");
	
	value_ =
		DEFINE("Value",
			REPEAT(1,
				CHOICE(
					REF("SpecialChar"),
					GLUE(
						NOT(
							GLUE(
								INLINE("Whitespace"),
								CHAR('<')
							)
						),
						ANY()
					)
				)
			)
		);
	
	typeTip_ =
		DEFINE("TypeTip",
			GLUE(
				STRING("<type>"),
				INLINE("Whitespace"),
				REF("Value"),
				INLINE("Whitespace"),
				STRING("</type>")
			)
		);
	
	memberName_ = DEFINE("MemberName", REPEAT(1, OTHER('\"')));
	memberType_ = DEFINE("MemberType", REPEAT(0, 1, INLINE("Value")));
	memberDescription_ = DEFINE("MemberDescription", REPEAT(0, 1, INLINE("Value")));
	
	memberTip_ =
		DEFINE("MemberTip",
			GLUE(
				STRING("<i n=\""),
				REF("MemberName"),
				STRING("\"><t>"),
				REF("MemberType"),
				STRING("</t><d>"),
				REF("MemberDescription"),
				STRING("</d></i>")
			)
		);
	
	classTip_ =
		DEFINE("ClassTip",
			GLUE(
				STRING("<list>"),
				REPEAT(1,
					GLUE(
						INLINE("Whitespace"),
						REF("MemberTip")
					)
				),
				INLINE("Whitespace"),
				STRING("</list>")
			)
		);
	
	haxeTip_ =
		DEFINE("HaxeTip",
			GLUE(
				INLINE("Whitespace"),
				CHOICE(
					REF("TypeTip"),
					REF("ClassTip")
				),
				INLINE("Whitespace")
			)
		);
	
	ENTRY("HaxeTip");
}

String HaxeMessageSyntax::readValue(String message, Ref<Token> token)
{
	if (token->firstChild()) {
		StringList list;
		Ref<Token> child = token->firstChild();
		int i = token->i0();
		while (child) {
			if (i < child->i0())
				list.append(message->copy(i, child->i0()));
			String s;
			if (child->keyword() == gt_) s = ">";
			else if (child->keyword() == lt_) s = "<";
			else s = message->copy(child->i0(), child->i1());
			list.append(s);
			i = child->i1();
			child = child->nextSibling();
		}
		if (i < token->i1())
			list.append(message->copy(i, token->i1()));
		return list.join();
	}
	else
		return message->copy(token->i0(), token->i1());
}

Ref<Instance, Owner> HaxeMessageSyntax::parse(String message)
{
	Ref<Instance, Owner> assistance;
	Ref<Token, Owner> rootToken = match(message);
	if (rootToken) {
		Ref<Token> token = rootToken->firstChild();
		if (token->rule() == typeTip_) {
			// debug("type tip\n");
			token = token->firstChild();
			if (token->rule() == value_) {
				String type = readValue(message, token);
				// debug("type = \"%%\"\n", type);
				assistance = new TypeTip(type);
			}
		}
		else if (token->rule() == classTip_) {
			// debug("class tip\n");
			Ref<MemberTipList, Owner> memberTipList = new MemberTipList;
			token = token->firstChild();
			while (token) {
				if (token->rule() == memberTip_) {
					String name, type, description;
					Ref<Token> child = token->firstChild();
					while (child) {
						if (child->rule() == memberName_)
							name = message->copy(child->i0(), child->i1());
						else if (child->rule() == memberType_)
							type = readValue(message, child);
						else if (child->rule() == memberDescription_)
							description = message->copy(child->i0(), child->i1());;
						child = child->nextSibling();
					}
					// debug("\"%%\",\"%%\",\"%%\"\n", name, type, description);
					memberTipList->append(new MemberTip(name, type, description));
				}
				token = token->nextSibling();
			}
			assistance = new ClassTip(memberTipList);
		}
	}
	
	return assistance;
}

} // namespace codetips
