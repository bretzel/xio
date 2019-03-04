#pragma once

#include <xio++/object.hpp>
#include <xio++/xtypes.hpp>
#include <xio++/interpreter/kernel/alu.hpp>
#include <xio++/expect.hpp>
#include <map>

#include <functional>
#include <utility>
#include <stack>

namespace xio {


/*!
    @brief xio : eXecutable Instruction Object.
    Where the beach is its grain of sand, the Interpreter is its grain of xio. (FUCK! LOL!!!!)

    @author &copy;2018, Serge Lussier (also known as: dynalog; bretzel; bretzelus; lordvorace), lussier.serge@gmail.com
*/
class xio_api xio_t : public object
{

    xio_t* op     = nullptr; ///< Parent Operator
    xio_t* lhs    = nullptr; ///< Left Hand Side Operand.
    xio_t* rhs    = nullptr; ///< Right Hand Size Operand.

    token_t* t0 = nullptr; ///< Token informations.
    alu* acc    = nullptr; ///< Arithmetic Logical Unit, or Accumulator, or whatever!

    struct storage_attr {
        uint8_t lvalue : 1; // 0 = rvalue non-assignable (such as const,leaf). 1= left value; assignable.
        uint8_t own    : 1; // 1 = this xio owns its acc storage - so will delete it in the destructor.
        uint8_t leave  : 1; // 1 = leave flag because this xio is a return statement.
        uint8_t sstatic: 1; // 1 = static storage.
        // ...
    }mem = { 0,0,0 }; ///< Storage types of this xio.
    friend class xio_variable;
    friend class xio_stack; 
    friend class interpreter_t;


public:
    using list_t = std::vector<xio_t*>;
    xio_t();

    xio_t(object* a_parent);
    xio_t(object* a_parent, token_t* a_token, alu* a_alu= nullptr);

    virtual ~xio_t();

    xio_t& operator =(const xio_t&) = delete;
    xio_t& operator =(xio_t&&) = delete;

    using result = expect<xio_t*>;
    using allocator_t = std::function<xio_t*(token_t *)>;

    alu* unit() { return acc; }
    
    xio_t::result tree_input(token_t* a_token, xio_t::allocator_t a_allocator=nullptr);

    virtual alu jsr(); // "Jump Subroutine" :)
    
    
    virtual std::string type_name();
    
    xio_t* oper() { return op; }
    token_t* token() { return t0;}
    xio_t::result tree_close();
    xio_t::result tree_root();

    void discard();

private:

#pragma region Assoc
    // First : declare associative pair:
    using associative_type_t = std::pair<type_t::T, type_t::T>;
    using inptr_fn_t    = xio_t::result(xio_t::*)(xio_t*);        ///< callable xio tree input function ptr
    using associated_method = std::pair < xio_t::associative_type_t, xio_t::inptr_fn_t>;

    using input_table_t = std::vector<xio_t::associated_method>;
    static xio_t::input_table_t tree_input_assoc_table;

#pragma endregion Assoc

protected:

    using xio_opfn_t = alu(xio_t::*)();
    using xio_opfn_table_t = std::map<e_code, xio_opfn_t>;
    

    xio_opfn_t xio_fn = nullptr;
private:

    static std::stack<xio_t*> pars;
    static std::stack<xio_t*> indexes;
    static std::stack<xio_t*> curls;
    static xio_t::list_t xio_gc;
    static void push_lpar(xio_t* lpar);
    static xio_t* pop_lpar();
    
/*  
    static void push_index(xio* index);
    static xio* pop_index();
    static void push_curly(xio* curly);
    static xio* pop_curly();
*/

#pragma region INPUT
    xio_t::result tree_input_binary(xio_t* x);
    xio_t::result tree_input_leaf(xio_t* x);
    xio_t::result tree_set_left(xio_t* x);
    xio_t::result tree_set_right(xio_t* x);
    xio_t::result tree_set_right_to_op(xio_t* x);

    xio_t::inptr_fn_t associate(xio_t* a_lhs, xio_t* a_rhs);

    xio_t::result tree_lpar_input_binary(xio_t* x);

    xio_t::result tree_input_rpar(xio_t*);
    xio_t::result tree_input_lpar(xio_t*);
    xio_t::result tree_close_par  (xio_t*);
    xio_t::result tree_rpar_input_postfix (xio_t*);
    xio_t::result tree_rpar_input_leaf(xio_t*);
    xio_t::result tree_rpar_rpar    (xio_t*);

#pragma endregion INPUT

    static xio_opfn_table_t xio_operators_table;
    // ----------- Primary operators ----------------------------
#pragma region ARITMETIC
    virtual alu  lshift();
    virtual alu  radical();
    virtual alu  exponent();
    virtual alu  rshift();
    virtual alu  decr();
    virtual alu  incr();
    virtual alu  assignadd();
    virtual alu  assignsub();
    virtual alu  assignmul();
    virtual alu  assigndiv();
    virtual alu  assignmod();
    virtual alu  assignand();
    virtual alu  assignor();
    virtual alu  assignxor();
    virtual alu  assignx1();
    virtual alu  assignlshift();
    virtual alu  assignrshift();
    virtual alu  leq();
    virtual alu  geq();
    virtual alu  eq();
    virtual alu  neq();  
    virtual alu  add();
    virtual alu  sub();
    virtual alu  mul();
    virtual alu  modulo();
    virtual alu  lt();
    virtual alu  gt();
    virtual alu  assign();
    virtual alu  binand();
    virtual alu  binor();
    virtual alu  bitxor();
    virtual alu  x1();
    virtual alu  x2();
    virtual alu  bitnot();
    virtual alu  booland();
    virtual alu  boolor();
    virtual alu  division();
    virtual alu  factorial();
    virtual alu  positive();
    virtual alu  negative();
    virtual alu  kpi();
    virtual alu  kcos();
    virtual alu  kacos();
    virtual alu  ktan();
    virtual alu  katan();
    virtual alu  ksin();
    virtual alu  kasin();
    virtual alu  knumber();
    virtual alu  ku8    ();
    virtual alu  ku16   ();
    virtual alu  ku32   ();
    virtual alu  ku64   ();
    virtual alu  ki8    ();
    virtual alu  ki16   ();
    virtual alu  ki32   ();
    virtual alu  ki64   ();
    virtual alu  kreal  ();
    virtual alu  kstring();

#pragma endregion ARITHMETICS
};

}

