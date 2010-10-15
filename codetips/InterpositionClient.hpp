#ifndef CODETIPS_INTERPOSITIONCLIENT_HPP
#define CODETIPS_INTERPOSITIONCLIENT_HPP

#include <unistd.h> // __MACH__?
#include <ftl/String.hpp>

namespace codetips
{

using namespace ftl;

class InterpositionClient
{
public:
	static String redirectOpen(String path, int flags);
	static int open(const char* path, int flags);
};

} // namespace codetips

extern "C" {
#ifndef __MACH__
	int open(const char* path, int flags, ...);
#else
	int codetips_open(const char* path, int flags, mode_t mode);
#endif
}

#endif // CODETIPS_INTERPOSITIONCLIENT_HPP
