#pragma once

#include <xio++/interpreter/module.hpp>


class Calulator : public xio::xio_module
{

    const xio::rule_t* aebtree;
public:

    Calulator();
    virtual ~Calulator();


    xio_t::result    build() override;

};

