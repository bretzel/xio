#pragma once
#include <xio/utils/rtloader.h>



namespace teacc
{

class teacc : public utils::rtlx
{


public:
    utils::notification::code execute() override;
};

}