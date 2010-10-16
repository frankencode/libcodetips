#include <unistd.h> // __MACH__?
#include <dlfcn.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ftl/PrintDebug.hpp> // DEBUG
#include <ftl/LocalStatic.hpp>
#include "InterpositionClient.hpp"

namespace codetips
{

String InterpositionClient::redirectOpen(String path, int flags)
{
	print("InterpositionClient::redirectOpen(): path = \"%%\"\n", path);
	return path;
}

class OpenFunction {
public:
	typedef int (*Pointer)(const char* path, int flags, ...);
	
	OpenFunction()
		: ptr_((Pointer)dlsym(RTLD_NEXT, "open"))
	{}
	Pointer ptr_;
};

} // namespace codetips

int open(const char* path, int flags, ...)
{
	using namespace codetips;
	String redirPath = InterpositionClient::redirectOpen(path, flags);
	OpenFunction& openFunction = localStatic<OpenFunction, InterpositionClient>();
	int ret = -1;
	if (flags & O_CREAT) {
		va_list ap;
		va_start(ap, flags);
		mode_t mode = va_arg(ap, int);
		va_end(ap);
		ret = openFunction.ptr_(redirPath, flags, mode);
	}
	else {
		ret = openFunction.ptr_(redirPath, flags);
	}
	return ret;
}

#ifdef __MACH__
char** environ; // link HACK
#endif
