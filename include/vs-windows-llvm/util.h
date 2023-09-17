#if !defined(JVS_VS_WINDOWS_LLVM_UTIL_H_)
#define JVS_VS_WINDOWS_LLVM_UTIL_H_

#include <string>

#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Debug.h>

#define PASS_DEBUG(x) LLVM_DEBUG(llvm::dbgs() << DEBUG_TYPE ": " << x)

namespace jvs
{

std::string GetEscapedString(llvm::StringRef str);
std::string ToString(const llvm::Module& m);
std::string ToString(const llvm::Type* t);
std::string ToString(const llvm::Value* v);

}  // namespace jvs


#endif  // !JVS_VS_WINDOWS_LLVM_UTIL_H_
