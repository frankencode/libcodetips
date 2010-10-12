#ifndef CODETIPS_INSTRUCTOR_HPP
#define CODETIPS_INSTRUCTOR_HPP

#include <ftl/Map.hpp>
#include <ftl/Variant.hpp>
#include <ftl/LineSink.hpp>
#include "codetips.hpp"

namespace codetips
{

typedef Map<String, Variant> ResourceMap;
typedef List<ResourceMap::Item> ResourceList;
typedef LineSink Log;

class Instructor: public Instance
{
public:
	Instructor();
	virtual ~Instructor() {}
	
	virtual String language() const = 0;
	virtual String name() const = 0;
	virtual String displayName() const = 0;
	virtual String description() const = 0;
	
	enum Modifiers { Shift = 1, Alt = 2, Control = 4, Meta = 8 };
	virtual Ref<Tip, Owner> assist(Ref<Context> context, int modifiers, uchar_t key) = 0;
	
	Variant resource(String key) const;
	void setResource(String key, Variant value);
	
	Ref<ResourceMap> resourceMap();
	Ref<ResourceList> resourceList();
	
	Ref<Log> log();
	virtual void update();
	
private:
	Ref<ResourceMap, Owner> resourceMap_;
	Ref<ResourceList, Owner> resourceList_;
	Ref<Log, Owner> log_;
};

#define CODETIPS_REGISTRATION_HEADER(FieldInstructor) \
class FieldInstructor; \
class FieldInstructor ## Registration { \
public: \
	FieldInstructor ## Registration(); \
}; \
\
namespace { FieldInstructor ## Registration registration_; }

#define CODETIPS_REGISTRATION_IMPL(FieldInstructor) \
FieldInstructor ## Registration::FieldInstructor ## Registration() { \
	Supervisor::instance()->registerInstructor(new FieldInstructor); \
}

} // namespace codetips

#endif // CODETIPS_INSTRUCTOR_HPP
