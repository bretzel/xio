#pragma once

#include <xio++/interpreter/module.hpp>


class Calulator : public xio::xio_module
{
    xio::token_t::list_t m_tokens;
    const xio::rule_t* aebtree;
public:

    Calulator();
    virtual ~Calulator();


    xio_t::result    build() override;
    xio_t::result    parse() override;

};

