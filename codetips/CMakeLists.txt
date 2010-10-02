SET(CODETIPS_HEADER
	codetips.hpp
	Instructor.hpp
	Supervisor.hpp
	HaxeInstructor.hpp
	HaxeMessageSyntax.hpp
)

SET(CODETIPS_SOURCE
	Instructor.cpp
	Supervisor.cpp
	HaxeInstructor.cpp
	HaxeMessageSyntax.cpp
)

ADD_LIBRARY(codetips
	${CODETIPS_HEADER}
	${CODETIPS_SOURCE}
)

SET_TARGET_PROPERTIES(codetips PROPERTIES VERSION 0.0.1 SOVERSION 0)

INSTALL(TARGETS codetips ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
