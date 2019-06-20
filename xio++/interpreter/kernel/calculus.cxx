//
// Created by bretzelus on 19-06-19.
//

#include "calculus.hpp"

namespace xio{
calculus::calculus():bloc_t()
{

}

calculus::~calculus()
{


}

xio_t::result calculus::operator[](const std::string & a_expr) {


    // ...
    return build();
//    return {
//        (message::push(message::xclass::internal), message::code::implement)
//    };
}

xio_t::result calculus::build() {
    return {
        (message::push(message::xclass::internal), message::code::implement)
    };
}


}
