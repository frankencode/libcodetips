SET(CODETIPS_HEADER
	codetips
	codetips.hpp
	Instructor.hpp
	Supervisor.hpp
	HelloWorld.hpp
	HaxeCodetips.hpp
	HaxeMessageSyntax.hpp
	InterpositionServer.hpp
)

SET(CODETIPS_SOURCE
	Instructor.cpp
	Supervisor.cpp
	HelloWorld.cpp
	HaxeCodetips.cpp
	HaxeMessageSyntax.cpp
	InterpositionServer.cpp
)

ADD_LIBRARY(codetips
	${CODETIPS_HEADER}
	${CODETIPS_SOURCE}
)

ADD_LIBRARY(codetipsclient
	InterpositionClient.hpp
	InterpositionClient.cpp
)

ADD_LIBRARY(interposer
	interposer.c
)

SET_TARGET_PROPERTIES(codetips PROPERTIES VERSION 0.0.1 SOVERSION 0)
SET_TARGET_PROPERTIES(codetipsclient PROPERTIES VERSION 0.0.1 SOVERSION 0)


INSTALL(TARGETS codetips ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
INSTALL(TARGETS codetipsclient ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
