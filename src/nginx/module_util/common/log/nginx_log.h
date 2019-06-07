//
// Created by ubuntu on 19-6-8.
//

#ifndef NGINX__MODULE_UTIL__COMMON__LOG__LOG_H
#define NGINX__MODULE_UTIL__COMMON__LOG__LOG_H

#include "../../../nginx_src/nginx_src.h"

namespace nginx
{
    namespace module_util
    {
        namespace common
        {
            namespace log
            {
                class NginxLog
                {
                private:
                    NginxLog() = default;

                public:
                    //工厂模式
                    static NginxLog get()
                    {
                        return NginxLog();
                    }

                    template<typename ... Args>
                    inline void stdError(ngx_log_t *log, const char *format, const Args &...args)
                    {
                        ngx_log_error_core(
                                NGX_LOG_STDERR, log,
                                static_cast<ngx_err_t>(NGX_LOG_STDERR),
                                format, args...
                        );
                    }

                    template<typename ... Args>
                    inline void emergent(ngx_log_t *log, const char *format, const Args &...args)
                    {
                        ngx_log_error_core(
                                NGX_LOG_EMERG, log,
                                static_cast<ngx_err_t>(NGX_LOG_EMERG),
                                format, args...
                        );
                    }

                    template<typename ... Args>
                    inline void alert(ngx_log_t *log, const char *format, const Args &...args)
                    {
                        ngx_log_error_core(
                                NGX_LOG_ALERT, log,
                                static_cast<ngx_err_t>(NGX_LOG_ALERT),
                                format, args...
                        );
                    }

                    template<typename ... Args>
                    inline void critical(ngx_log_t *log, const char *format, const Args &...args)
                    {
                        ngx_log_error_core(
                                NGX_LOG_CRIT, log,
                                static_cast<ngx_err_t>(NGX_LOG_CRIT),
                                format, args...
                        );
                    }

                    template<typename ... Args>
                    inline void error(ngx_log_t *log, const char *format, const Args &...args)
                    {
                        ngx_log_error_core(
                                NGX_LOG_ERR, log,
                                static_cast<ngx_err_t>(NGX_LOG_ERR),
                                format, args...
                        );
                    }

                    template<typename ... Args>
                    inline void warn(ngx_log_t *log, const char *format, const Args &...args)
                    {
                        ngx_log_error_core(
                                NGX_LOG_WARN, log,
                                static_cast<ngx_err_t>(NGX_LOG_WARN),
                                format, args...
                        );
                    }

                    template<typename ... Args>
                    inline void notice(ngx_log_t *log, const char *format, const Args &...args)
                    {
                        ngx_log_error_core(
                                NGX_LOG_NOTICE, log,
                                static_cast<ngx_err_t>(NGX_LOG_NOTICE),
                                format, args...
                        );
                    }

                    template<typename ... Args>
                    inline void info(ngx_log_t *log, const char *format, const Args &...args)
                    {
                        ngx_log_error_core(
                                NGX_LOG_INFO, log,
                                static_cast<ngx_err_t>(NGX_LOG_INFO),
                                format, args...
                        );
                    }

                    template<typename ... Args>
                    inline void debug(ngx_log_t *log, const char *format, const Args &...args)
                    {
                        ngx_log_error_core(
                                NGX_LOG_DEBUG, log,
                                static_cast<ngx_err_t>(NGX_LOG_DEBUG),
                                format, args...
                        );
                    }
                };
            }
        }
    }
}
#endif //NGINX__MODULE_UTIL__COMMON__LOG__LOG_H
