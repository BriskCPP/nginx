//
// Created by ubuntu on 19-6-8.
//

#ifndef NGINX__MODULE_UTIL__COMMON__VALUE__UNSET_VALUE_H
#define NGINX__MODULE_UTIL__COMMON__VALUE__UNSET_VALUE_H
namespace nginx
{
    namespace module_util
    {
        namespace common
        {
            namespace value
            {
                class UnsetValue final
                {
                private:
                    UnsetValue() = default;

                public:
                    static const UnsetValue &getConst()
                    {
                        static const UnsetValue unsetValue;
                        return unsetValue;
                    }

                    static UnsetValue &get()
                    {
                        static UnsetValue unsetValue;
                        return unsetValue;
                    }

                    template<typename T>
                    inline explicit operator T()
                    {
                        return static_cast<T>(-1);
                    }

                    template<typename T>
                    inline explicit operator T *()
                    {
                        return reinterpret_cast<T *>(-1);
                    }
                };


                template<typename T>
                bool isUnset(const T &value)
                {
                    return value == static_cast<T>(UnsetValue::getConst());
                }

                template<typename ValueType, typename CandidateValueType>
                void setIfUnset(ValueType &value, const CandidateValueType &candidateValue)
                {
                    if (isUnset<ValueType>(value)) value = candidateValue;
                }
            }
        }
    }
}
#endif //NGINX__MODULE_UTIL__COMMON__VALUE__UNSET_VALUE_H
