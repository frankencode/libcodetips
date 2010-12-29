/*
 * AssistantManager.cpp -- singleton which manages the assistants
 *
 * Copyright (c) 2010, Frank Mertens
 *
 * See ../COPYING for the license.
 */

#include "AssistantManager.hpp"

namespace codetips
{

AssistantManager::AssistantManager()
	: assistantListByLanguage_(new AssistantListByLanguage)
{}

void AssistantManager::registerAssistant(Ref<Assistant> assistant)
{
	Ref<AssistantList, Owner> list;
	if (!assistantListByLanguage_->lookup(assistant->language(), &list)) {
		list = new AssistantList;
		assistantListByLanguage_->insert(assistant->language(), list);
	}
	list->append(assistant);
}

Ref<AssistantListByLanguage> AssistantManager::assistantListByLanguage() const
{
	return assistantListByLanguage_;
}

Ref<Instance, Owner> AssistantManager::assist(Ref<Context> context, int modifiers, uchar_t key) const
{
	Ref<Instance, Owner> tip = 0;
	Ref<AssistantList, Owner> list;
	assistantListByLanguage_->lookup(context->language(), &list);
	if (list) {
		AssistantList::Iterator it = list->iterator();
		while ((it.hasNext()) && (!tip)) {
			Ref<Assistant> assistant = it.next();
			tip = assistant->assist(context, modifiers, key);
		}
	}
	return tip;
}

} // namespace codetips
