#if !defined(JVS_VS_WINDOWS_LLVM_STRING_ANALYSIS_H_)
#define JVS_VS_WINDOWS_LLVM_STRING_ANALYSIS_H_

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/MapVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Pass.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>

#include <vs-windows-llvm/plugin-registration.h>

namespace jvs
{

class StringAnalyzer final : public llvm::AnalysisInfoMixin<StringAnalyzer>
{
public:
  using Result = llvm::MapVector<llvm::GlobalVariable*, llvm::StringRef>;

  Result run(llvm::Module& m, llvm::ModuleAnalysisManager& /*unused*/);
  Result run(llvm::Module& m);

  static inline constexpr llvm::StringRef name() { return "string-analyzer"; }
  
private:
  friend class llvm::AnalysisInfoMixin<StringAnalyzer>;
  friend void RegisterPluginPasses(llvm::PassBuilder& passBuilder);

  static inline llvm::AnalysisKey Key;

  static void registerAnalysis(llvm::ModuleAnalysisManager& am);
};

/// Printing pass for StringAnalyzer
class StringAnalyzerPrinter final : public llvm::PassInfoMixin<StringAnalyzerPrinter>
{
public:
  explicit StringAnalyzerPrinter(llvm::raw_ostream& os);

  llvm::PreservedAnalyses run(llvm::Module& m, llvm::ModuleAnalysisManager& am);

  static inline constexpr llvm::StringRef name() { return "print<string-analyzer>"; }

private:
  friend void RegisterPluginPasses(llvm::PassBuilder& passBuilder);

  llvm::raw_ostream& os_;

  static bool registerPipelinePass(llvm::StringRef name, llvm::ModulePassManager& mpm,
    llvm::ArrayRef<llvm::PassBuilder::PipelineElement> /*ignored*/);
  static bool registerEPPass(llvm::ModulePassManager& mpm, llvm::OptimizationLevel opt);
};

}  // namespace jvs


#endif  // !JVS_VS_WINDOWS_LLVM_STRING_ANALYSIS_H_
