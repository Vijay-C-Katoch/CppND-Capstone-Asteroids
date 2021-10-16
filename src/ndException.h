#ifndef ND_EXCEPTION_H_
#define ND_EXCEPTION_H_

#include <stdexcept>
#include <sstream>
#include <type_traits>
#include <utility>

namespace nd
{
    template<typename T>
    class Exception : public T
    {
        static_assert(std::is_base_of_v<std::exception, T> == true, "nd::Exception<T>: T must be derived from std::exception");
    public:
        template<typename ...Args>
        Exception(const char* fileName, int lineNumber, const char* functionName, const Args &...messages);
    };


    // definition

    template<typename T>
    template<typename ...Args>
    Exception<T>::Exception(const char* fileName, int lineNumber, const char* functionName, const Args &...messages) :
        T(
            [&] () 
            {
                std::ostringstream oss;
                oss << fileName << ':' << lineNumber <<':' << functionName << "()";
                if constexpr (sizeof...(messages) != 0) {
                    ((oss << " : ") << ... << messages);     // binary left fold
                }
                else {
                    oss << ' ';
                }
                return oss.str();
            }() // execute immidiately
        )
    {}


    // Standard exception alias
    using LogicException = Exception<std::logic_error>;
    using RuntimeException = Exception<std::runtime_error>;


    /////////////////////////////////////////////////////////////////////////////

    template<typename T, typename E>
    class ClassException : public E
    {
        static_assert(std::is_base_of_v<std::exception, T> == true, "nd::ClassException<T, E>: E must be derived from std::exception");
    public:
        template<typename ...Args>
        ClassException(Args &&...args); // Template forward reference
    };

    // definition
    template<typename T, typename E>
    template<typename ...Args>
    inline ClassException<T, E>::ClassException(Args &&...args) : E(std::forward<Args>(args)...)
    {}


}

#endif /* ND_EXCEPTION_H_ */
