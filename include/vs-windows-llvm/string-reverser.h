#if !defined(JVS_VS_WINDOWS_LLVM_STRING_REVERSER_H_)
#define JVS_VS_WINDOWS_LLVM_STRING_REVERSER_H_

#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>

#include <vs-windows-llvm/plugin-registration.h>


namespace jvs
{

class StringReverser final : public llvm::PassInfoMixin<StringReverser>
{
public:
  llvm::PreservedAnalyses run(llvm::Module& m, llvm::ModuleAnalysisManager& am);
  bool run(llvm::ArrayRef<std::pair<llvm::GlobalVariable*, llvm::ConstantDataArray>>);
  static inline constexpr llvm::StringRef name() { return "string-reverser"; }

private:
  friend class llvm::PassInfoMixin<StringReverser>;
  friend void RegisterPluginPasses(llvm::PassBuilder& passBuilder);

  static inline llvm::AnalysisKey Key;

  static bool registerPass(llvm::StringRef name, llvm::ModulePassManager& mpm,
    llvm::ArrayRef<llvm::PassBuilder::PipelineElement> /*ignored*/);
};

}  // namespace jvs


#endif  // !JVS_VS_WINDOWS_LLVM_STRING_REVERSER_H_
