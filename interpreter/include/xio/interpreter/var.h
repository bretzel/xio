


#pragma once

#include <xio/interpreter/xio.h>



namespace teacc
{

class INTERPRETERAPI var : public xio
{
    using shared = std::shared_ptr<var>;
public:
    var() = default;
    var(xio::shared a_parent, lexer::type::token_t* a_token, alu::shared a_acc);
    ~var() override = default;

    static var::shared make(xio::shared a_parent, lexer::type::token_t* a_token, alu::shared a_acc);

protected:
    utils::result_code instanciate_from(xio::shared bcd) override
    {
        return utils::notification::code::implement;
    }
};

}