#include <vector>
#include <stack>
#include <mutex>
#include <iostream>

#include "context.hh"
namespace mamba {
  std::mutex callStackMutex, exceptionHandlerMutex;

  std::stack<Traceback> callStack;
  std::vector<std::stack<const MicrocodeSection*>> exceptionHandlers;

  size_t integerToStringConversionLimit = 4000;
  size_t getIntegerToStringConversionLimit() {
    return integerToStringConversionLimit;
  }

  void setIntegerToStringConversionLimit(const size_t limit) {
    integerToStringConversionLimit = limit;
  }

  void pushCallStack(Traceback traceback) {
    std::scoped_lock<std::mutex> guard(callStackMutex);
    callStack.push(traceback);
  }

  void popCallStack() {
    std::scoped_lock<std::mutex> guard(callStackMutex);
    callStack.pop();
  }

  void raise(Signal error, ExceptionReason reason) {

  }

  void raise(size_t error, const std::string_view& message) {

  }
}