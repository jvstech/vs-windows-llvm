#if !defined(JVS_VS_WINDOWS_LLVM_STRING_REVERSER_H_)
#define JVS_VS_WINDOWS_LLVM_STRING_REVERSER_H_

#include <string>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Passes/PassBuilder.h>

#include <vs-windows-llvm/plugin-registration.h>


namespace jvs
{

class StringReverser final : public llvm::PassInfoMixin<StringReverser>
{
public:
  llvm::PreservedAnalyses run(llvm::Module& m, llvm::ModuleAnalysisManager& am);
  bool run(llvm::SmallVectorImpl<std::pair<llvm::GlobalVariable*, std::string>>&& globalStrings);
  static inline constexpr llvm::StringRef name() { return "string-reverser"; }

private:
  friend class llvm::PassInfoMixin<StringReverser>;
  friend void RegisterPluginPasses(llvm::PassBuilder& passBuilder);

  static inline llvm::AnalysisKey Key;

  static bool registerPipelinePass(llvm::StringRef name, llvm::ModulePassManager& mpm,
    llvm::ArrayRef<llvm::PassBuilder::PipelineElement> /*ignored*/);
  static bool registerEPPass(llvm::ModulePassManager& mpm, llvm::OptimizationLevel opt);
};

}  // namespace jvs


#endif  // !JVS_VS_WINDOWS_LLVM_STRING_REVERSER_H_
