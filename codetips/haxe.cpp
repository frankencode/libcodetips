#include <ftl/ProcessFactory.hpp>
#include <ftl/Path.hpp>
#include <ftl/Format.hpp>
#include <ftl/PrintDebug.hpp> // debug
#include "haxe.hpp"

namespace codetips
{

HaxeFactory::HaxeFactory()
	: processFactory_(new ProcessFactory)
{
	processFactory_->setExecPath("haxe");
}

String HaxeFactory::name() const { return "haxe"; }

Ref<Instance> HaxeFactory::produce(String path, off_t offset)
{
	String className = Path(path).fileNameSansExtension();
	String projectFile = "";
	String searchPath = path;
	
	while (searchPath != "/") {
		searchPath = Path(searchPath).reduce();
		String candidate = Format("%%/compile.hxml") << searchPath;
		if (File(candidate).exists()) {
			projectFile = candidate;
			break;
		}
	}
	
	if (projectFile == "") return 0; // "Can't find project file."
	
	String options = Format("%% %% --display %%@%%") << projectFile << className << path << offset;
	
	debug("options = \"%%\"\n", options);
	
	processFactory_->setOptions(options.split("."));
	
	return 0;
}

} // namespace codetips
