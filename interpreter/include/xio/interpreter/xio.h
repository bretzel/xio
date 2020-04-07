//
// Created by slussier on 19-08-25.
//

#pragma once

#include <xio/interpreter/interpreterdll.h>
#include <xio/utils/expect>
#include <xio/interpreter/alu.h>
#include <xio/lexer/token_t.h>



#include <functional>
#include <utility>
#include <stack>
#include <map>

namespace teacc{

/*!
    @brief xio : eXecutable Instruction Object.
    Where the beach is its grain of sand, the Interpreter is its grain of xio. (FUCK! LOL!!!!)

    @author &copy;2018, Serge Lussier (also known as: dynalog; bretzel; bretzelus; lordvorace), lussier.serge@gmail.com
*/

class INTERPRETERAPI  xio {
public:
    using shared     = std::shared_ptr<xio>;
    using collection = std::vector<xio::shared>;

    ///< Children Xio's (instructions/[statically stored]bloc/stack instances) shared across blocs/stacks.
    using shared_collection = std::shared_ptr<xio::collection>;

private:

    xio::shared op     = nullptr; ///< Parent Operator
    xio::shared lhs    = nullptr; ///< Left Hand Side Operand.
    xio::shared rhs    = nullptr; ///< Right Hand Size Operand.

    
    lexer::type::token_t* t0 = nullptr; ///< Token informations.
    alu::shared acc; ///< Arithmetic Logical Unit, or Accumulator, or whatever!


    struct storage_attr {
        uint8_t lvalue : 1; // 0 = rvalue non-assignable (such as const,leaf). 1= left value; assignable.
        uint8_t own    : 1; // 1 = this xio owns its acc storage - so will delete it in the destructor.
        uint8_t leave  : 1; // 1 = leave flag because this xio is a return statement.
        uint8_t sstatic: 1; // 1 = static storage.
        // ...
    }mem = { 0,0,0,0 }; ///< Storage types of this xio.
    friend class var;
    friend class bloc;
    // friend class compiler;
    // friend class interpreter_t;


public:

    xio() = default;


    xio(xio::shared a_parent);
    xio(xio::shared a_parent, lexer::type::token_t* a_token, alu::shared a_alu = nullptr);

    virtual ~xio() = default;

    xio& operator =(const xio&) = delete;
    xio& operator =(xio&&) = delete;

    using expect = utils::expect<xio::shared>;
    using maker = std::function<xio::shared(lexer::type::token_t *)>;

    alu::shared unit() { return acc; }

    xio::expect tree_input(lexer::type::token_t* a_token, xio::maker a_allocator=nullptr);

    virtual alu jsr(); // "Jump Subroutine" :)


    virtual std::string type_name();

    xio::shared oper() { return op; }
    lexer::type::token_t* token() { return t0;}
    xio::expect tree_close();
    xio::expect tree_root();

    static xio::shared make(xio::shared a_parent, lexer::type::token_t* a_token, alu::shared a_acc);
    
    template<class T> T* to() {return dynamic_cast<T*>(this);}


private:

#pragma region Assoc
    // First : declare associative pair:
    using associative_type = std::pair<lexer::type::T, lexer::type::T>;
    using inptr_fn_t    = xio::expect(xio::*)(xio::shared);        ///< callable xio tree input function ptr
    using associated_method = std::pair < xio::associative_type, xio::inptr_fn_t>;

    using input_table_t = std::vector<xio::associated_method>;
    static xio::input_table_t tree_input_assoc_table;

#pragma endregion Assoc

protected:

    using xio_opfn_t = alu(xio::*)();
    using xio_opfn_table_t = std::map<lexer::lexem::mnemonic, xio_opfn_t>;


    xio_opfn_t xio_fn = nullptr;

    static xio::shared get_global_root();
    virtual utils::result_code instanciate_from(xio::shared bcd);

private:

    static xio::shared _global_root_bloc;
    static std::stack<xio::shared> pars;
    static std::stack<xio::shared> indexes;
    static std::stack<xio::shared> curls;
    static void push_lpar(xio::shared lpar);
    static xio::shared pop_lpar();

/*  
    static void push_index(xio::shared index);
    static xio::shared pop_index();
    static void push_curly(xio::shared curly);
    static xio::shared pop_curly();
*/

// -------- Arithmetic binary tree INPUT: -----------------------------------
#pragma region INPUT
    xio::expect tree_input_binary(xio::shared x);
    xio::expect tree_input_leaf(xio::shared x);
    xio::expect tree_set_left(xio::shared x);
    xio::expect tree_set_right(xio::shared x);
    xio::expect tree_set_right_to_op(xio::shared x);

    xio::inptr_fn_t associate(xio::shared a_lhs, xio::shared a_rhs);

    xio::expect tree_lpar_input_binary(xio::shared x);

    xio::expect tree_input_rpar(xio::shared);
    xio::expect tree_input_lpar(xio::shared);
    xio::expect tree_close_par  (xio::shared);
    xio::expect tree_rpar_input_postfix (xio::shared);
    xio::expect tree_rpar_input_leaf(xio::shared);
    xio::expect tree_rpar_rpar    (xio::shared);

#pragma endregion INPUT
// -------- Arithmetic binary tree: -----------------------------------
    
    
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

#pragma region TRIGRAPH
public:
    static void trace_tree_start(utils::xstr& a_out, const utils::xstr& Title);
    static void trace_node(xio::shared A, utils::xstr& a_out);
    static void trace_null_node(xio::shared, int, utils::xstr&);
    static void trace_tree(xio::shared a_root, utils::xstr& a_out);
    static void trace_tree_close(utils::xstr& a_out);
    void tree_attr(utils::xstr& a_out);
    std::string attribute();
#pragma endregion TRIGRAPH


protected:

    collection _children;
    xio::shared _parent;

    xio::shared query_child(xio* cThis);

};

template<class T> typename T::shared make(xio::shared a_parent, lexer::type::token_t* a_token, alu::shared a_acc)
{
    return std::make_shared<T>(a_parent, a_token, a_acc);
}


}
