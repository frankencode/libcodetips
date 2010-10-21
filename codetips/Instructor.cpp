/*
 * Instructor.cpp -- language-specific codetips generator
 *
 * Copyright (c) 2010, Frank Mertens
 *
 * See ../COPYING for the license.
 */

#include <ftl/File.hpp>
#include "Instructor.hpp"

namespace codetips
{

Instructor::Instructor()
	: resourceMap_(new ResourceMap),
	  resourceList_(new ResourceList)
{}

Variant Instructor::resource(String key) const {
	return resourceMap_->get(key);
}

void Instructor::setResource(String key, Variant value) {
	if (resourceMap_->insert(key, value)) {
		resourceList_->append(ResourceList::Item(key, value));
	}
	else {
		resourceMap_->remove(key);
		resourceMap_->insert(key, value);
	}
}

Ref<Log> Instructor::log() {
	if (!log_) {
		Ref<File, Owner> tmp = File::temp();
		tmp->unlinkWhenDone();
		log_ = new Log(tmp);
		
	}
	return log_;
}

void Instructor::update()
{}

Ref<ResourceMap> Instructor::resourceMap() { return resourceMap_; }
Ref<ResourceList> Instructor::resourceList() { return resourceList_; }

} // namespace codetips
