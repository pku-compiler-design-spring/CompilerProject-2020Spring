#ifndef MYTEST
#define MYTEST
#include "IR.h"

Boost::Internal::Ref<const Boost::Internal::IRNode> MyIntBuilder(int val);
Boost::Internal::Ref<const Boost::Internal::IRNode> MyDoubleBuilder(double val);
Boost::Internal::Ref<const Boost::Internal::IRNode> MyIdBuilder(const char *val);
#endif