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
	return (log_) ? log_ : log_ = new Log(File::temp());
}

void Instructor::update()
{}

Ref<ResourceMap> Instructor::resourceMap() { return resourceMap_; }
Ref<ResourceList> Instructor::resourceList() { return resourceList_; }

} // namespace codetips
