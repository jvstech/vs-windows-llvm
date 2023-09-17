#if !defined(JVS_VS_WINDOWS_LLVM_PLUGIN_REGISTRATION_H_)
#define JVS_VS_WINDOWS_LLVM_PLUGIN_REGISTRATION_H_

#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>

namespace jvs
{

void RegisterPluginPasses(llvm::PassBuilder& passBuilder);

}  // namespace jvs


extern "C" LLVM_ATTRIBUTE_WEAK auto llvmGetPassPluginInfo() -> ::llvm::PassPluginLibraryInfo;

#endif  // !JVS_VS_WINDOWS_LLVM_PLUGIN_REGISTRATION_H_
