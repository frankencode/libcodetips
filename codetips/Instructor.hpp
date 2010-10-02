#ifndef CODETIPS_INSTRUCTOR_HPP
#define CODETIPS_INSTRUCTOR_HPP

#include <ftl/Map.hpp>
#include <ftl/Variant.hpp>
#include <ftl/LineSink.hpp>
#include "codetips.hpp"

namespace codetips
{

class Instructor: public Instance
{
public:
	virtual ~Instructor() {}
	
	virtual String language() const = 0;
	virtual String description() const = 0;
	
	enum Modifiers { Shift = 1, Alt = 2, Control = 4, Meta = 8 };
	virtual Ref<Instance, Owner> assist(Ref<Context> context, int modifiers, uchar_t key) = 0;
	
	typedef Map<String, Variant> Knowhow;
	typedef LineSink Log;
	Ref<Knowhow> knowhow();
	Ref<Log> log();
	virtual void update();
	
private:
	Ref<Knowhow, Owner> knowhow_;
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
