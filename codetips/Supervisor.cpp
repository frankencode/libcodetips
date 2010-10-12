#include "Supervisor.hpp"

namespace codetips
{

Supervisor::Supervisor()
	: instructorListByLanguage_(new InstructorListByLanguage)
{}

void Supervisor::registerInstructor(Ref<Instructor> instructor)
{
	Ref<InstructorList, Owner> list;
	if (!instructorListByLanguage_->lookup(instructor->language(), &list)) {
		list = new InstructorList;
		instructorListByLanguage_->insert(instructor->language(), list);
	}
	list->append(instructor);
}

Ref<InstructorListByLanguage> Supervisor::instructorListByLanguage() const
{
	return instructorListByLanguage_;
}

Ref<Instance, Owner> Supervisor::assist(Ref<Context> context, int modifiers, uchar_t key) const
{
	Ref<Instance, Owner> tip = 0;
	Ref<InstructorList, Owner> list;
	instructorListByLanguage_->lookup(context->language(), &list);
	if (list) {
		InstructorList::Iterator it = list->iterator();
		while ((it.hasNext()) && (!tip)) {
			Ref<Instructor> instructor = it.next();
			tip = instructor->assist(context, modifiers, key);
		}
	}
	return tip;
}

} // namespace codetips
