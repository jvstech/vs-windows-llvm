#include <vs-windows-llvm/plugin-registration.h>

#include <vs-windows-llvm/string-analysis.h>
#include <vs-windows-llvm/string-reverser.h>

void jvs::RegisterPluginPasses(llvm::PassBuilder& passBuilder)
{
  // opt tool registration
  passBuilder.registerAnalysisRegistrationCallback(jvs::StringAnalyzer::registerAnalysis);
  passBuilder.registerPipelineParsingCallback(jvs::StringAnalyzerPrinter::registerPipelinePass);
  passBuilder.registerPipelineParsingCallback(jvs::StringReverser::registerPipelinePass);
  // Default optimization level registration
  passBuilder.registerOptimizerEarlyEPCallback(jvs::StringAnalyzerPrinter::registerEPPass);
  passBuilder.registerOptimizerEarlyEPCallback(jvs::StringReverser::registerEPPass);
  passBuilder.registerOptimizerEarlyEPCallback(jvs::StringAnalyzerPrinter::registerEPPass);
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
