#include <unistd.h> // __MACH__?
#include <ftl/Format.hpp>
#include <ftl/Process.hpp>
#include <ftl/LinkInfo.hpp>
#include "InterpositionServer.hpp"

int codetips_hook;

namespace codetips
{

InterpositionServer::InterpositionServer()
	: path_(Format("/tmp/codetips_%%.socket") << Process::currentId())
{
	start();
}

InterpositionServer::~InterpositionServer()
{
	done_.release();
	try { kill(); } catch (...) {}
}

void InterpositionServer::setup(Ref<EnvMap> map)
{
	map->insert("CODETIPS_SOCKET", path_);
	String libPath = LinkInfo((void*)&codetips_hook).libraryPath();
	#ifdef __MACH__
	map->insert("DYLD_INSERT_LIBRARIES", libPath);
	#else
	map->insert("LD_PRELOAD", libPath);
	#endif
}

void InterpositionServer::run()
{
	try { File(path_).unlink(); } catch(...) {}
	socket_ = new StreamSocket(new SocketAddress(AF_LOCAL, path_));
	socket_->bind();
	socket_->listen();
	while (!done_.tryAcquire()) {
		try {
			Ref<StreamSocket, Owner> stream = socket_->accept();
			Ref<StringList, Owner> parts = stream->readAll().split(",");
			if (parts->length() == 2) {
				String path = parts->at(0);
				int flags = parts->at(1).toInt();
				stream->write(redirectOpen(path, flags));
			}
			/* NB: May be signalled in two different areas:
			 *     [A] in kernel (accept(2), read(2) or write(2))
			 *     [B] during userlevel code execution (between the calls above)
			 * In case [A] direct termination is guaranteed (throwing/catching StreamException).
			 * In case [B] at most another connection will be accepted and handled completely.
			 */
		}
		catch (StreamException&) {
			break;
		}
	}
}

} // namespace codetips
