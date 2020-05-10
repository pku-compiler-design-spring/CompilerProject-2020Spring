#include "../../include/mytest.h"
Boost::Internal::Ref<const Boost::Internal::IRNode> MyIntBuilder(int val)
{
    return Boost::Internal::Expr(val);
};
Boost::Internal::Ref<const Boost::Internal::IRNode> MyDoubleBuilder(double val)
{
    return Boost::Internal::Expr(val);
};
Boost::Internal::Ref<const Boost::Internal::IRNode> MyIdBuilder(const char *val)
{
    std::string str(val);
    return Boost::Internal::StringImm::make(Boost::Internal::Type::int_scalar(32), str);
};