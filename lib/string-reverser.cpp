#include <vs-windows-llvm/string-reverser.h>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/STLExtras.h>

#include <vs-windows-llvm/string-analysis.h>
#include <vs-windows-llvm/util.h>

#define DEBUG_TYPE "string-reverser"

namespace
{

using GVStringRefPair = jvs::StringAnalyzer::Result::value_type;
using GVStdStringPair = std::pair<llvm::GlobalVariable*, std::string>;

GVStdStringPair toStdString(GVStringRefPair gvStringRefPair)
{
  return { gvStringRefPair.first, gvStringRefPair.second.str() };
}

}  // namespace


llvm::PreservedAnalyses jvs::StringReverser::run(llvm::Module& m, llvm::ModuleAnalysisManager& am)
{
  auto results = llvm::PreservedAnalyses::all();
  auto globalStrings = llvm::to_vector(llvm::map_range(am.getResult<StringAnalyzer>(m), toStdString));
  bool modified = !globalStrings.empty();
  while (!globalStrings.empty())
  {
    auto [gv, stringVal] = globalStrings.pop_back_val();
    std::reverse(stringVal.begin(), stringVal.end());
    gv->setInitializer(llvm::ConstantDataArray::getString(m.getContext(), stringVal));
    PASS_DEBUG("Replaced " 
      << gv->getNameOrAsOperand() 
      << " with \"" 
      << GetEscapedString(stringVal) 
      << "\"\n");
  }

  if (modified)
  {
    results.intersect(llvm::PreservedAnalyses::allInSet<llvm::CFGAnalyses>());
  }

  return results;
}

bool jvs::StringReverser::registerPass(llvm::StringRef name, llvm::ModulePassManager& mpm,
  llvm::ArrayRef<llvm::PassBuilder::PipelineElement> /*unused*/)
{
  if (name == DEBUG_TYPE)
  {
    mpm.addPass(StringReverser{});
    return true;
  }

  return false;
}
