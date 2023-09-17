#include <vs-windows-llvm/plugin-registration.h>

#include <vs-windows-llvm/string-analysis.h>
#include <vs-windows-llvm/string-reverser.h>

void jvs::RegisterPluginPasses(llvm::PassBuilder& passBuilder)
{
  passBuilder.registerAnalysisRegistrationCallback(jvs::StringAnalyzer::registerAnalysis);
  passBuilder.registerPipelineParsingCallback(jvs::StringAnalyzerPrinter::registerPass);
  passBuilder.registerPipelineParsingCallback(jvs::StringReverser::registerPass);
}

auto llvmGetPassPluginInfo()
  -> llvm::PassPluginLibraryInfo
{
  return {
    LLVM_PLUGIN_API_VERSION,  // APIVersion
    "vs-windows-llvm",        // PluginName
    LLVM_VERSION_STRING,      // PluginVersion
    &jvs::RegisterPluginPasses
  };
}
