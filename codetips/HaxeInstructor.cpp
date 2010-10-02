#include <ftl/ProcessFactory.hpp>
#include <ftl/Path.hpp>
#include <ftl/Format.hpp>
#include <ftl/PrintDebug.hpp> // debug
#include "Supervisor.hpp"
#include "HaxeMessageSyntax.hpp"
#include "HaxeInstructor.hpp"

namespace codetips
{

CODETIPS_REGISTRATION_IMPL(HaxeInstructor)

HaxeInstructor::HaxeInstructor()
	: processFactory_(new ProcessFactory),
	  messageSyntax_(new HaxeMessageSyntax)
{
	Path haxe = Path::lookup(Process::env("PATH").split(":"), "haxe", File::Exists|File::Execute);
	processFactory_->setExecPath(haxe);
	processFactory_->setIoPolicy(Process::ForwardOutput|Process::ErrorToOutput);
	knowhow()->insert("HAXE", haxe);
	// debug("HaxeInstructor::HaxeInstructor(): processFactory_->execPath() = \"%%\"\n", processFactory_->execPath());
}

String HaxeInstructor::language() const { return "haxe"; }
String HaxeInstructor::description() const { return "Show code tips directly from the haXe compiler."; }

Ref<Instance, Owner> HaxeInstructor::assist(Ref<Context> context, int modifiers, uchar_t key)
{
	Ref<Instance, Owner> assistance;
	
	if (processFactory_->execPath() == "") return assistance;
	
	String className = Path(context->path()).fileNameSansExtension();
	String projectFile = "";
	String searchPath = context->path();
	
	while (searchPath != "/") {
		searchPath = Path(searchPath).reduce();
		String candidate = Format("%%/compile.hxml") << searchPath;
		if (File(candidate).exists()) {
			projectFile = candidate;
			break;
		}
	}
	
	if (projectFile == "") return 0;
	
	String options = Format("%% %% --display %%@%%") << projectFile << className << context->path() << context->cursorByte();
	
	// debug("HaxeInstructor::assist(): options = \"%%\"\n", options);
	
	processFactory_->setOptions(options.split(" "));
	
	Ref<Process, Owner> process = processFactory_->produce();
	
	String message = process->rawOutput()->readAll();
	
	// debug("HaxeInstructor::assist(): message = \"%%\"\n", message);
	
	return messageSyntax_->parse(message);
}

} // namespace codetips
