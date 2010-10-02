#ifndef CODETIPS_SUPERVISOR_HPP
#define CODETIPS_SUPERVISOR_HPP

#include <ftl/PrefixTree.hpp>
#include <ftl/Singleton.hpp>
#include "codetips.hpp"
#include "Instructor.hpp"

namespace codetips
{

typedef List< Ref<Instructor, Owner> > InstructorList;
typedef Map<String, Ref<InstructorList, Owner> > InstructorListByLanguage;

class Supervisor: public Instance, public Singleton<Supervisor>
{
public:
	void registerInstructor(Ref<Instructor> instructor);
	Ref<InstructorListByLanguage> instructorListByLanguage() const;
	
	Ref<Instance> assist(Ref<Context> context, int modifiers, uchar_t key) const;
	
private:
	friend class Singleton<Supervisor>;
	Supervisor();
	
	Ref<InstructorListByLanguage, Owner> instructorListByLanguage_;
};

} // namespace codetips

#endif // CODETIPS_SUPERVISOR_HPP
