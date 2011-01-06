/*
 * WordPredictor.cpp -- word indexer and auto-completer
 *
 * Copyright (c) 2010, Frank Mertens
 *
 * See ../COPYING for the license.
 */

#include <ftl/PrintDebug.hpp>
#include "AssistantManager.hpp"
#include "WordPredictor.hpp"

namespace codetips
{

class WordPredictorRegistration {
public:
	WordPredictorRegistration();
};

WordPredictorRegistration::WordPredictorRegistration()
{
	AssistantManager::instance()->registerAssistant(WordPredictor::instance());
}

namespace { WordPredictorRegistration wordAutoCompleterRegistration; }

String WordPredictor::language() const { return "any";}
String WordPredictor::name() const { return "predictor"; }
String WordPredictor::displayName() const { return "Genric Word Predictor"; }
String WordPredictor::description() const { return "Word based index and auto-completion service, invoked by TAB key."; }

Ref<Tip, Owner> WordPredictor::assist(Ref<Context> context, int modifiers, uchar_t key)
{
	Ref<Tip, Owner> tip;
	
	bool tabKey = (key == '\t') &&  ((modifiers == Shift) || (modifiers == 0));
	if (!tabKey) return tip;
	
	Ref<Words, Owner> words;
	if (wordsByPath_->lookup(context->path(), &words)) {
		String text = context->copyLine(context->line());
		String::Index i1 = text.first() + context->linePos();
		String::Index i0 = i1;
		while (text.def(i0 - 1)) {
			uchar_t ch = text.get(i0 - 1);
			if (
				(('a' <= ch) && (ch <= 'z')) ||
				(('A' <= ch) && (ch <= 'Z')) ||
				(('0' <= ch) && (ch <= '9')) ||
				(ch == '_')
			)
				--i0;
			else 
				break;
		}
		if (i0 != i1) {
			String prefix = text.copy(i0, i1);
			Words::Index first, last, common;
			// print("WordPredictor::assist(): prefix = \"%%\"\n", prefix);
			if (words->predict(prefix, &first, &last, &common)) {
				String commonPrefix = words->key(common);
				// print("WordPredictor::assist(): commonPrefix = \"%%\"\n", commonPrefix);
				if (commonPrefix.length() > prefix.length()) {
					int nc = commonPrefix.length() - prefix.length();
					context->insert(commonPrefix.tail(nc));
					context->move(nc);
					tip = new Tip;
				}
				else {
					/*int n = 0;
					{
						Words::Index i = first;
						while (true) {
							++n;
							print("WordPredictor::assist(): n, words->value(i) = %%, \"%%\"\n", n, words->value(i));
							if (i == last) break;
							else ++i;
						}
					}*/
					/*Ref<Members, Owner> members = new Members(n);
					{
						Words::Index i = first;
						int j = 0;
						while (true) {
							members->set(j, new Member(words->value(i)));
							++j;
							if (i == last) break;
							else ++i;
						}
					}
					tip = new MembersTip(members);*/
				}
			}
		}
	}
	
	return tip;
}

WordPredictor::WordPredictor()
	: wordsByPath_(new WordsByPath)
{}

void WordPredictor::extractWords(String path, String text)
{
	disbandWords(path);
	Ref<Words, Owner> words = new Words;
	const char* p = text;
	const char* p0 = p;
	char ch;
	while ((ch = *p) != 0) {
		bool wordChar =
			(('a' <= ch) && (ch <= 'z')) ||
			(('A' <= ch) && (ch <= 'Z')) ||
			(('0' <= ch) && (ch <= '9')) ||
			(ch == '_') ||
			((p0 < p) && (ch == '!')) /* Ruby insitu method */ ||
			((p0 == p) && (ch == '#')) /* C++ preprocessing stmt.*/;
		if (wordChar) {
			++p;
		}
		else {
			if (p0 < p) {
				String s(p0, p - p0);
				words->insert(s, s);
			}
			++p;
			p0 = p;
		}
	}
	/*{
		// DEBUG
		print("========================================\n");
		print("file: %%\n", path);
		int n = 0;
		for (Words::Index i = words->first(); words->def(i); ++i, ++n)
			print("[%%]\n", words->value(i));
		print("n = %%\n", n);
	}*/
	wordsByPath_->insert(path, words);
}

void WordPredictor::disbandWords(String path)
{
	wordsByPath_->remove(path);
}

} // namespace codetips
