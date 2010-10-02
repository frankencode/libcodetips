#include <ftl/File.hpp>
#include "Instructor.hpp"

namespace codetips
{

Ref<Instructor::Knowhow> Instructor::knowhow() {
	return (knowhow_) ? knowhow_ : knowhow_ = new Knowhow;
}

Ref<Instructor::Log> Instructor::log() {
	return (log_) ? log_ : log_ = new Log(File::temp());
}

void Instructor::update()
{}

} // namespace codetips
