#include <vs-windows-llvm/string-analysis.h>

#include <string>

#include <llvm/ADT/STLExtras.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/Support/Debug.h>

#include <vs-windows-llvm/util.h>

#define DEBUG_TYPE "string-analyzer"

namespace
{

STATISTIC(StringCount, "number of strings");

/// Returns true if the given GlobalVariable points to a string literal.
bool isCString(const llvm::GlobalVariable& gv) noexcept
{
  if (!gv.isConstant() || !gv.hasInitializer())
  {
    PASS_DEBUG(gv.getNameOrAsOperand() << " is either mutable or does not have an initializer.\n");
    return false;
  }

  auto* cda = llvm::dyn_cast<llvm::ConstantDataArray>(gv.getInitializer());
  if (!cda)
  {
    PASS_DEBUG(gv.getNameOrAsOperand() << " is not an array.\n");
    return false;
  }
  
  if (!cda->isCString())
  {
    PASS_DEBUG(gv.getNameOrAsOperand() << " is not a string.\n");
    return false;
  }

  return true;
}

}  // namespace


jvs::StringAnalyzer::Result jvs::StringAnalyzer::run(llvm::Module& m)
{
  StringCount = 0;
  Result results;
  for (llvm::GlobalVariable& gv : llvm::make_filter_range(m.globals(), isCString))
  {
    ++StringCount;
    auto stringVal = llvm::cast<llvm::ConstantDataArray>(gv.getInitializer())->getAsCString();
    PASS_DEBUG("Found string "
      << gv.getNameOrAsOperand() 
      << ": \"" 
      << GetEscapedString(stringVal) 
      << "\"\n");
    results.insert({ &gv, stringVal });
  }

  return results;
}

jvs::StringAnalyzer::Result jvs::StringAnalyzer::run(
  llvm::Module& m, llvm::ModuleAnalysisManager& /*unused*/)
{
  return run(m);
}

void jvs::StringAnalyzer::registerAnalysis(llvm::ModuleAnalysisManager& am)
{
  am.registerPass([&] { return llvm::PassInstrumentationAnalysis{}; });
  am.registerPass([&] { return StringAnalyzer{}; });
}

jvs::StringAnalyzerPrinter::StringAnalyzerPrinter(llvm::raw_ostream& os)
  : os_(os)
{
}

llvm::PreservedAnalyses jvs::StringAnalyzerPrinter::run(
  llvm::Module& m, llvm::ModuleAnalysisManager& am)
{
  for (auto [gv, stringVal] : am.getResult<StringAnalyzer>(m))
  {
    os_ << gv->getNameOrAsOperand() << " = \"" << GetEscapedString(stringVal) << "\"\n";
  }

  return llvm::PreservedAnalyses::all();
}

bool jvs::StringAnalyzerPrinter::registerPass(llvm::StringRef name, llvm::ModulePassManager& mpm,
  llvm::ArrayRef<llvm::PassBuilder::PipelineElement> /*unused*/)
{
  if (name.consume_front("print") &&
    (name.consume_front("<" DEBUG_TYPE ">") || name.consume_front("[" DEBUG_TYPE "]")))
  {
    mpm.addPass(StringAnalyzerPrinter(llvm::outs()));
    return true;
  }
  
  return false;
}
