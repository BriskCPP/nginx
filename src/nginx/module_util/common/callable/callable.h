//
// Created by ubuntu on 19-6-17.
//

#ifndef NGINX__MODULE_UTIL__COMMON__CALLABLE__CALLABLE_H
#define NGINX__MODULE_UTIL__COMMON__CALLABLE__CALLABLE_H

#include<memory>

namespace nginx
{
    namespace module_util
    {
        namespace callable
        {
            template<typename Callable, unsigned int uniqueSignature>
            class CallableObjectHolder
            {
            private:
                static std::shared_ptr<Callable> callableObject;
            public:
                template<typename ReturnType, typename ... Arguments>
                static inline ReturnType function(Arguments... args)
                {
                    return (*callableObject)(std::forward<Arguments>(args)...);
                }

                template<typename ReturnType, typename ... Arguments>
                using c_style_function_pointer = ReturnType(*)(Arguments ... args);

                template<typename ReturnType, typename ... Arguments>
                static inline c_style_function_pointer<ReturnType, Arguments...> getPointer(
                        std::shared_ptr<Callable> &&callable
                )
                {
                    callableObject = std::shared_ptr<Callable>(callable);
                    return &function<ReturnType, Arguments...>;
                }

                template<typename ReturnType, typename ... Arguments>
                static inline c_style_function_pointer<ReturnType, Arguments...> getPointer(
                        const std::shared_ptr<Callable> &callable
                )
                {
                    callableObject = std::shared_ptr<Callable>(callable);
                    return &function<ReturnType, Arguments...>;
                }
            };

            //define static member out of the class
            template<typename Callable, unsigned int uniqueSignature>
            std::shared_ptr<Callable>
                    CallableObjectHolder<Callable, uniqueSignature>::callableObject;
        }
    }
}
#endif //NGINX__MODULE_UTIL__COMMON__CALLABLE__CALLABLE_H
