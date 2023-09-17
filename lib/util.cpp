#include <vs-windows-llvm/util.h>

#include <llvm/ADT/StringExtras.h>

#include <llvm/Support/raw_ostream.h>

namespace
{

template <typename StreamWriterT>
std::string writeToStream(const StreamWriterT& writer)
{
  std::string result;
  llvm::raw_string_ostream os(result);
  writer(os);
  return os.str();
}

}  // namespace 


std::string jvs::GetEscapedString(llvm::StringRef str)
{
  return writeToStream([=](llvm::raw_ostream& os)
    {
      llvm::printEscapedString(str, os);
    });
}

std::string jvs::ToString(const llvm::Module& m)
{
  return writeToStream([&](llvm::raw_ostream& os)
    {
      m.print(os, /*AAW*/ nullptr);
    });
}

std::string jvs::ToString(const llvm::Type* t)
{
  return writeToStream([=](llvm::raw_ostream& os)
    {
      if (t)
      {
        os << *t;
      }
    });
}

std::string jvs::ToString(const llvm::Value* v)
{
  return writeToStream([=](llvm::raw_ostream& os)
    {
      if (v)
      {
        os << *v;
      }
    });
}
