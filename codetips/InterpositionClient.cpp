#include <unistd.h> // __MACH__?
#include <ftl/PrintDebug.hpp> // DEBUG
#ifndef __MACH__
#include <dlfcn.h>
#include <stdarg.h>
#include <ftl/LocalStatic.hpp>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "InterpositionClient.hpp"

namespace codetips
{

String InterpositionClient::redirectOpen(String path, int flags)
{
	print("InterpositionClient::redirectOpen(): path = \"%%\"\n", path);
	return path;
}

} // namespace codetips

#ifndef __MACH__
class OpenFunction {
public:
	typedef int (*Pointer)(const char* path, int flags, ...);
	OpenFunction()
		: ptr_((Pointer)dlsym(RTLD_NEXT, "open"))
	{}
	Pointer ptr_;
};

int open(const char* path, int flags, ...)
{
	using namespace codetips;
	String redirPath = InterpositionClient::redirectOpen(path, flags);
	OpenFunction& openFunction = localStatic<OpenFunction, InterpositionClient>();
	int ret = -1;
	if (flags & O_CREAT) {
		va_list ap;
		va_start(ap, flags);
		mode_t mode = va_arg(ap, mode_t);
		va_end(ap);
		ret = openFunction.ptr_(redirPath, flags, mode);
	}
	else {
		ret = openFunction.ptr_(redirPath, flags);
	}
	return ret;
}
#else
typedef struct interpose_s {
    void *new_func;
    void *orig_func;
} interpose_t;

int codetips_open(const char* path, int flags, mode_t mode);

static const interpose_t interposers[] \
	__attribute__ ((section("__DATA, __interpose"))) = {
		{ (void *)codetips_open,  (void *)open  }
	};

int codetips_open(const char* path, int flags, mode_t mode)
{
    String redirPath = InterpositionClient::redirectOpen(path, flags);
    return open(redirPath, flags, mode);
}
#endif
