
#include <xio/utils/notification.h>

#include <map>


namespace xio::utils
{


notification::stack   notification::_stack;
static notification NullA;

std::map<notification::type, std::string> Types = {
    {notification::type::unset,        "Unset"     },
    {notification::type::status,       "Status"    },
    {notification::type::error,        "Error"     },
    {notification::type::notice,       "Notice"    },
    {notification::type::warning,      "Warning"   },
    {notification::type::fatal,        "Fatal"     },
    {notification::type::exception,    "Exception" },
    {notification::type::information,  "Information"},
    {notification::type::internal,     "Internal"   }
};


std::map<notification::code, std::string> Codes = {
    {notification::code::unset,      "Unset"},
    {notification::code::success,     "Success"},
    {notification::code::ok,         "ok"},
    {notification::code::accepted,   "Accepted"},
    {notification::code::rejected,   "Rejected"},
    {notification::code::unexpected, "Unexpected" },
    {notification::code::empty,      "Empty"},
    {notification::code::invalid,    "Invalid"},
    {notification::code::illegal,    "Illegal"},
    {notification::code::eof,        "Eof"},
    {notification::code::null,       "Null"},
    {notification::code::unmatched,    "Unmatched"},
    {notification::code::syntax,     "Syntax"},
    {notification::code::exists,     "Exists"},
    {notification::code::implement,  "Implement" },
    {notification::code::unterminated_litteral,  "Unterminated litteral string" },
    {notification::code::notfound,   "Not found"}
    
};

notification::notification(const notification& A)
{
    _text = A._text;
    _type = A._type;
    _code = A._code;
    
}

notification::notification(notification&& A) noexcept
{
    _text = A._text;
    _type = A._type;
    _code = A._code;
    std::swap(_text, A._text);
}


notification::~notification()
{
    _text.clear();
}

notification& notification::Null()
{
    return NullA;
}

notification& notification::operator=(const notification& A)
{
    _text = A._text;
    _type = A._type;
    _code = A._code;
    
    return *this;
}

notification& notification::operator=(notification&& A) noexcept
{
    _text = A._text;
    _type = A._type;
    _code = A._code;
    std::swap(_text, A._text);
    return *this;
}

void notification::clear(std::function<void(notification&)> LambdaFn)
{
    if (notification::_stack.empty()) return;
    while (!notification::_stack.empty()) {
        if (LambdaFn)
            LambdaFn(notification::_stack.top());
        notification::_stack.pop();
    }
    
}

notification& notification::push(notification::type aType)
{
    notification A;
    A._type = aType; // notification::type::Notice by default;
    notification::_stack.push(A);
    return notification::_stack.top();
}

std::string notification::operator()()
{
    _text = "";
    for (auto S : _components) _text += S;
    _components.clear();
    return _text();
}

std::string notification::to_s(notification::code C)
{
    return Codes[C];
}

std::string notification::to_s(notification::type T)
{
    return Types[T];
}




}