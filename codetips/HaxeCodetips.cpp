#include <stdlib.h> // setenv, unsetenv
#include <ftl/ProcessFactory.hpp>
#include <ftl/Dir.hpp>
#include <ftl/Path.hpp>
#include <ftl/Format.hpp>
#include <ftl/PrintDebug.hpp> // debug
#include "Supervisor.hpp"
#include "HaxeMessageSyntax.hpp"
#include "InterpositionServer.hpp"
#include "HaxeCodetips.hpp"

namespace codetips
{

CODETIPS_REGISTRATION_IMPL(HaxeCodetips)

HaxeCodetips::HaxeCodetips()
	: processFactory_(new ProcessFactory),
	  messageSyntax_(new HaxeMessageSyntax)
{
	Path haxe = Path::lookup(Process::env("PATH").split(":"), "haxe", File::Exists|File::Execute);
	
	processFactory_->setExecPath(haxe);
	processFactory_->setIoPolicy(Process::ForwardOutput|Process::ErrorToOutput);
	InterpositionServer::injectClient(processFactory_->envMap());
	
	setResource("haxe", haxe);
	
	update();
}

void HaxeCodetips::update()
{
	Path haxe = resource("haxe");
	printTo(log(), "HaxeCodetips::update(): haxe = %%\n", haxe);
	if (haxe != "") {
		if (File(haxe).access(File::Read|File::Execute))
			printTo(log(), "Found 'haxe' binary at \"%%\".\n", haxe);
		else
			printTo(log(), "Insufficiant permissions to 'haxe' binary.\n");
	}
	else {
		printTo(log(), "Can't find the 'haxe' binary.\n");
		printTo(log(), "Please provide the path to the 'haxe' binary.\n");
	}
}

String HaxeCodetips::language() const { return "haxe"; }
String HaxeCodetips::name() const { return "codetips"; }
String HaxeCodetips::displayName() const { return "haXe Code Tips"; }

String HaxeCodetips::description() const { return "Provides code tips for haxe, when pressing the magic key (SHIFT-Tab)."; }

Ref<Tip, Owner> HaxeCodetips::assist(Ref<Context> context, int modifiers, uchar_t key)
{
	if ( (key != '\t') ||
	     ((modifiers != Shift) && (modifiers != 0)) ||
	     (processFactory_->execPath() == "") )
	     return 0;
	
	Ref<Instance, Owner> tip;
	
	if (modifiers == 0) {
		String line = context->line();
		if (context->cursorColumn() == 0) return 0;
		char ch = line.get(line.first() + context->cursorColumn() - 1);
		if ((ch != '.') && (ch != '(')) return 0;
	}
	
	String className = Path(context->path()).fileNameSansExtension();
	String projectFile = "";
	String searchPath = context->path();
	
	while (searchPath != "/") {
		searchPath = Path(searchPath).reduce();
		Dir dir(searchPath);
		while (dir.hasNext()) {
			Ref<DirEntry, Owner> entry = dir.next();
			Ref<StringList, Owner> parts = entry->name().split(".");
			if (parts->at(parts->last()) == "hxml") {
				projectFile = Format("%%/%%") << searchPath << entry->name();
				break;
			}
		}
	}
	
	if (projectFile == "") return tip;
	
	processFactory_->setWorkingDirectory(Path(projectFile).reduce());
	// debug("HaxeCodetips::assist(): processFactory_->execPath() = \"%%\"\n", processFactory_->execPath());
	
	String options = Format("%% %% --display %%@%%") << projectFile << className << context->path() << context->cursorByte();
	
	// debug("HaxeCodetips::assist(): options = \"%%\"\n", options);
	
	processFactory_->setOptions(options.split(" "));
	
	Ref<Process, Owner> process = processFactory_->produce();
	String message = process->rawOutput()->readAll();
	
	// debug("HaxeCodetips::assist(): message = \"%%\"\n", message);
	
	return messageSyntax_->parse(message);
}

} // namespace codetips
