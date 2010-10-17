#include <unistd.h> // __MACH__?
#include <ftl/PrintDebug.hpp> // DEBUG
#include <ftl/Format.hpp>
#include <ftl/LinkInfo.hpp>
#include "InterpositionServer.hpp"

int codetips_hook;

namespace codetips
{

InterpositionServer::InterpositionServer()
{
	debug("InterpositionServer::InterpositionServer()\n");
	start();
}

InterpositionServer::~InterpositionServer()
{
	debug("InterpositionServer::~InterpositionServer(): finalize...\n");
	done_.release();
	debug("InterpositionServer::~InterpositionServer(): sa1\n");
	try { kill(); } catch (...) {}
	debug("InterpositionServer::~InterpositionServer(): wait...\n");
	wait();
	debug("InterpositionServer::~InterpositionServer(): done.\n");
}

String InterpositionServer::socketPath()
{
	return Format("/tmp/codetips_%%.socket") << Process::currentId();
}

void InterpositionServer::injectClient(Ref<EnvMap> map)
{
	map->insert("CODETIPS_SOCKET", socketPath());
	String libPath = LinkInfo((void*)&codetips_hook).libraryPath().replace("codetips.", "codetipsclient.");
	print("InterpositionServer::injectClient(): libPath = \"%%\"\n", libPath);
	#ifdef __MACH__
	print("setting DYLD_FORCE_FLAT_NAMESPACE, DYLD_INSERT_LIBRARIES\n");
	map->insert("DYLD_FORCE_FLAT_NAMESPACE", "");
	map->insert("DYLD_INSERT_LIBRARIES", libPath);
	#else
	map->insert("LD_PRELOAD", libPath);
	#endif
}

void InterpositionServer::run()
{
	debug("InterpositionServer::run()...\n");
	try { File(socketPath()).unlink(); } catch(...) {}
	socket_ = new StreamSocket(new SocketAddress(AF_LOCAL, socketPath()));
	socket_->bind();
	debug("  bound: socket_->localAddress()->toString() = \"%%\"\n", socket_->localAddress()->toString());
	socket_->listen();
	debug("  waiting for connection...\n");
	while (!done_.tryAcquire()) {
		try {
			Ref<StreamSocket, Owner> stream = socket_->accept();
			debug("  ooh...\n");
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
	debug("InterpositionServer::run(): done.\n");
}

} // namespace codetips
