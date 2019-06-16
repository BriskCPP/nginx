//
// Created by ubuntu on 19-6-9.
//

#ifndef NGINX__MODULE_UTIL__MODULE_ENTRY__NGINX_MODULE_H
#define NGINX__MODULE_UTIL__MODULE_ENTRY__NGINX_MODULE_H


#include <memory>
#include <vector>
#include <functional>

#include "../../nginx_src/nginx_src.h"
#include "../common/commons.h"

namespace nginx
{
    namespace module_util
    {
        namespace module_entry
        {
            template<unsigned int numberOfCommands, unsigned int uniqueSignature>
            class NginxCommandsHolder
            {
            public:
                static ngx_command_t commands[];

                static inline ngx_command_t *set(
                        const std::vector<ngx_command_t> &commandsVector
                )
                {
                    if (commandsVector.size() <= numberOfCommands)
                    {
                        //输入的列表长度小于等于可以保存的长度，那么就照样保存，一个不差
                        //然后把最后一个设为空元素，是规定结尾
                        for (std::vector<ngx_command_t>::size_type currentCommandIndex = 0;
                             currentCommandIndex < commandsVector.size(); ++currentCommandIndex)
                        {
                            commands[currentCommandIndex] = commandsVector[currentCommandIndex];
                        }
                        commands[commandsVector.size()] = ngx_null_command;//以一个空对象结尾
                    }
                    else
                    {
                        //输入的列表长度大于可以保存的长度，不会自动扩容，对输入的列表进行截断
                        for (std::vector<ngx_command_t>::size_type currentCommandIndex = 0;
                             currentCommandIndex < numberOfCommands; ++currentCommandIndex)
                        {
                            commands[currentCommandIndex] = commandsVector[currentCommandIndex];
                        }
                        commands[numberOfCommands] = ngx_null_command;//以一个空对象结尾
                    }
                    return commands;
                }
            };

            //在类外定义静态变量
            template<unsigned int numberOfCommands, unsigned int uid>
            ngx_command_t NginxCommandsHolder<numberOfCommands, uid>::commands[numberOfCommands + 1];


            template<typename CTX>
            class NginxModule final
            {
            private:
                const CTX ctx;

                const ngx_command_t *const commands;
                //ngx_command_t组成的数组，用一个空对象作为结尾
                //所以我们可以转变为一个…………vector
                //嗯…………说明我们还要想办法把一个vector转变成C语言的数组…………一片连续的内存区域
                const ngx_uint_t moduleTypeFlag;

                ngx_int_t (*const init_module)(ngx_cycle_t *cycle);

                ngx_int_t (*const init_process)(ngx_cycle_t *cycle);

                void (*const exit_process)(ngx_cycle_t *cycle);

                void (*const exit_master)(ngx_cycle_t *cycle);

            public:
                NginxModule(
                        const CTX &ctx,
                        const ngx_command_t *const commands,
                        const ngx_uint_t moduleTypeFlag,
                        ngx_int_t (*init_module)(ngx_cycle_t *cycle) = nullptr,
                        ngx_int_t (*init_process)(ngx_cycle_t *cycle) = nullptr,
                        void (*exit_process)(ngx_cycle_t *cycle) = nullptr,
                        void (*exit_master)(ngx_cycle_t *cycle) = nullptr
                ) :
                        ctx(ctx), commands(commands),
                        moduleTypeFlag(moduleTypeFlag),
                        init_module(init_module), init_process(init_process),
                        exit_process(exit_process), exit_master(exit_master)
                {
                }


                inline const ngx_module_t getInternalModuleDescription() const
                {
                    ngx_module_t result = {
                            NGX_MODULE_V1,
                            reinterpret_cast<void *>(const_cast<CTX *>(&ctx)),
                            const_cast<ngx_command_t *>(this->commands),
                            moduleTypeFlag,
                            nullptr,/**UNUSED HOOK:ngx_int_t    (*init_master)(ngx_log_t *log)*/
                            this->init_module,
                            this->init_process,
                            nullptr,/**UNUSED HOOK:ngx_int_t    (*init_thread)(ngx_cycle_t *cycle)*/
                            nullptr,/**UNUSED HOOK:void         (*exit_thread)(ngx_cycle_t *cycle)*/
                            this->exit_process,
                            this->exit_master,
                            NGX_MODULE_V1_PADDING
                    };
                    return result;
                }


                template<unsigned int numberOfCommands, unsigned int uniqueSignature>
                class Builder
                {
                private:
                    Builder() : hooks({}), commands(nullptr), moduleTypeFlag(0)
                    {}

                    const CTX hooks;
                    const ngx_command_t *const commands;
                    const ngx_uint_t moduleTypeFlag;

                    ngx_int_t (*init_module)(ngx_cycle_t *cycle) = nullptr;

                    ngx_int_t (*init_process)(ngx_cycle_t *cycle) = nullptr;

                    void (*exit_process)(ngx_cycle_t *cycle) = nullptr;

                    void (*exit_master)(ngx_cycle_t *cycle) = nullptr;

                public:
                    NginxModule<CTX> build()
                    {
                        return NginxModule<CTX>(
                                hooks, this->commands, this->moduleTypeFlag,
                                this->init_module, this->init_process,
                                this->exit_process, this->exit_master
                        );
                    }

                    inline Builder &onInitModule(
                            std::shared_ptr<std::function<ngx_int_t(ngx_cycle_t *)>> &&onInitModule
                    )
                    {
                        using nginx::module_util::callable::CallableObjectHolder;
                        this->init_module = CallableObjectHolder<
                                std::function<ngx_int_t(ngx_cycle_t *)>, (uniqueSignature + __LINE__)
                        >::template getPointer<ngx_int_t, ngx_cycle_t *>(
                                std::forward<
                                        std::shared_ptr<std::function<ngx_int_t(ngx_cycle_t *)>>
                                >(onInitModule)
                        );
                        return *this;
                    }

                    inline Builder &onInitModule(ngx_int_t (*init_module)(ngx_cycle_t *))
                    {
                        this->init_module = init_module;
                        return *this;
                    }

                    inline Builder &onInitProcess(
                            std::shared_ptr<std::function<ngx_int_t(ngx_cycle_t *)>> &&onInitProcess
                    )
                    {
                        using nginx::module_util::callable::CallableObjectHolder;
                        this->init_process = CallableObjectHolder<
                                std::function<ngx_int_t(ngx_cycle_t *)>, (uniqueSignature + __LINE__)
                        >::template getPointer<ngx_int_t, ngx_cycle_t *>(
                                std::forward<
                                        std::shared_ptr<std::function<ngx_int_t(ngx_cycle_t *)>>
                                >(onInitProcess)
                        );
                        return *this;
                    }

                    inline Builder &onInitProcess(ngx_int_t (*init_process)(ngx_cycle_t *))
                    {
                        this->init_process = init_process;
                        return *this;
                    }

                    inline Builder &onExitProcess(
                            std::shared_ptr<std::function<void(ngx_cycle_t *)>> &&onExitProcess
                    )
                    {
                        using nginx::module_util::callable::CallableObjectHolder;
                        this->exit_process = CallableObjectHolder<
                                std::function<void(ngx_cycle_t *)>, (uniqueSignature + __LINE__)
                        >::template getPointer<void, ngx_cycle_t *>(
                                std::forward<
                                        std::shared_ptr<std::function<void(ngx_cycle_t *)>>
                                >(onExitProcess)
                        );
                        return *this;
                    }

                    inline Builder &onExitProcess(void (*exit_process)(ngx_cycle_t *))
                    {
                        this->exit_process = exit_process;
                        return *this;
                    }

                    inline Builder &onExitMaster(
                            std::shared_ptr<std::function<void(ngx_cycle_t *)>> &&onExitMaster
                    )
                    {
                        using nginx::module_util::callable::CallableObjectHolder;
                        this->exit_master = CallableObjectHolder<
                                std::function<void(ngx_cycle_t *)>, (uniqueSignature + __LINE__)
                        >::template getPointer<void, ngx_cycle_t *>(
                                std::forward<
                                        std::shared_ptr<std::function<void(ngx_cycle_t *)>>
                                >(onExitMaster)
                        );
                        return *this;
                    }

                    inline Builder &onExitMaster(void (*exit_master)(ngx_cycle_t *))
                    {
                        this->exit_master = exit_master;
                        return *this;
                    }

                    Builder(
                            const CTX &context,
                            const ngx_command_t *const commands,
                            const ngx_uint_t moduleTypeFlag
                    ) : hooks(context), commands(commands), moduleTypeFlag(moduleTypeFlag)
                    {
                        this->init_module = nullptr;
                        this->init_process = nullptr;
                        this->exit_process = nullptr;
                        this->exit_master = nullptr;
                    }

                    Builder(
                            const CTX &context,
                            const std::vector<ngx_command_t> &commands,
                            const ngx_uint_t moduleTypeFlag
                    ) : hooks(context),
                        commands(
                                NginxCommandsHolder<
                                        numberOfCommands, (uniqueSignature + __LINE__)
                                >::set(commands)
                        ),
                        moduleTypeFlag(moduleTypeFlag)
                    {
                        this->init_module = nullptr;
                        this->init_process = nullptr;
                        this->exit_process = nullptr;
                        this->exit_master = nullptr;
                    }
                };
            };


            template<unsigned int uniqueSignature>
            class HttpModuleHooks
            {
            public:
                inline HttpModuleHooks &onPreConfiguration(
                        std::shared_ptr<std::function<ngx_int_t(ngx_conf_t *)>> &&pre_configuration
                )
                {
                    using nginx::module_util::callable::CallableObjectHolder;
                    this->preConfiguration = CallableObjectHolder<
                            std::function<ngx_int_t(ngx_conf_t *)>, (uniqueSignature + __LINE__)
                    >::template getPointer<ngx_int_t, ngx_conf_t *>(
                            std::forward<
                                    std::shared_ptr<std::function<ngx_int_t(ngx_conf_t *)>>
                            >(pre_configuration)
                    );
                    return *this;
                }

                inline HttpModuleHooks &onPostConfiguration(
                        std::shared_ptr<std::function<ngx_int_t(ngx_conf_t *)>> &&post_configuration
                )
                {
                    using nginx::module_util::callable::CallableObjectHolder;
                    this->postConfiguration = CallableObjectHolder<
                            std::function<ngx_int_t(ngx_conf_t *)>, (uniqueSignature + __LINE__)
                    >::template getPointer<ngx_int_t, ngx_conf_t *>(
                            std::forward<
                                    std::shared_ptr<std::function<ngx_int_t(ngx_conf_t *)>>
                            >(post_configuration)
                    );
                    return *this;
                }

                inline HttpModuleHooks &onCreateMainConfiguration(
                        std::shared_ptr<std::function<void *(ngx_conf_t *)>> &&createMainConfiguration
                )
                {
                    using nginx::module_util::callable::CallableObjectHolder;
                    this->create_main_conf = CallableObjectHolder<
                            std::function<void *(ngx_conf_t *)>, uniqueSignature + __LINE__
                    >::template getPointer<void *, ngx_conf_t *>(
                            std::forward<
                                    std::shared_ptr<std::function<void *(ngx_conf_t *)>>
                            >(createMainConfiguration)
                    );
                    return *this;
                }

                inline HttpModuleHooks &onInitMainConfiguration(
                        std::shared_ptr<std::function<char *(ngx_conf_t *, void *)>> &&initMainConfiguration
                )
                {
                    using nginx::module_util::callable::CallableObjectHolder;
                    this->init_main_conf = CallableObjectHolder<
                            std::function<char *(ngx_conf_t *, void *)>, uniqueSignature + __LINE__
                    >::template getPointer<char *, ngx_conf_t *, void *>(
                            std::forward<
                                    std::shared_ptr<std::function<char *(ngx_conf_t *, void *)>>
                            >(initMainConfiguration)
                    );
                    return *this;
                }

                inline HttpModuleHooks &onCreateServerConfiguration(
                        std::shared_ptr<std::function<void *(ngx_conf_t *)>> &&createServerConfiguration
                )
                {
                    using nginx::module_util::callable::CallableObjectHolder;
                    this->create_srv_conf = CallableObjectHolder<
                            std::function<void *(ngx_conf_t *)>, uniqueSignature + __LINE__
                    >::template getPointer<void *, ngx_conf_t *>(
                            std::forward<
                                    std::shared_ptr<std::function<void *(ngx_conf_t *)>>
                            >(createServerConfiguration));
                    return *this;
                }

                inline HttpModuleHooks &onMergeServerConfiguration(
                        std::shared_ptr<std::function<char *(ngx_conf_t *, void *, void *)>> &&mergeServerConfiguration
                )
                {
                    using nginx::module_util::callable::CallableObjectHolder;
                    this->merge_srv_conf = CallableObjectHolder<
                            std::function<char *(ngx_conf_t *, void *, void *)>, uniqueSignature + __LINE__
                    >::template getPointer<char *, ngx_conf_t *, void *, void *>(
                            std::forward<
                                    std::shared_ptr<std::function<char *(ngx_conf_t *, void *, void *)>>
                            >(mergeServerConfiguration)
                    );
                    return *this;
                }

                inline HttpModuleHooks &onCreateLocationConfiguration(
                        std::shared_ptr<std::function<void *(ngx_conf_t *)>> &&createLocationConfiguration
                )
                {
                    using nginx::module_util::callable::CallableObjectHolder;
                    this->create_loc_conf = CallableObjectHolder<
                            std::function<void *(ngx_conf_t *)>, uniqueSignature + __LINE__
                    >::template getPointer<void *, ngx_conf_t *>(
                            std::forward<
                                    std::shared_ptr<std::function<void *(ngx_conf_t *)>>
                            >(createLocationConfiguration));
                    return *this;
                }

                inline HttpModuleHooks &onMergeLocationConfiguration(
                        std::shared_ptr<std::function<char *(ngx_conf_t *, void *, void *)>> &&mergeLocationConfiguration
                )
                {
                    using nginx::module_util::callable::CallableObjectHolder;
                    this->merge_loc_conf = CallableObjectHolder<
                            std::function<char *(ngx_conf_t *, void *, void *)>, uniqueSignature + __LINE__
                    >::template getPointer<char *, ngx_conf_t *, void *, void *>(
                            std::forward<
                                    std::shared_ptr<std::function<char *(ngx_conf_t *, void *, void *)>>
                            >(mergeLocationConfiguration)
                    );
                    return *this;
                }

                HttpModuleHooks()
                {
                    preConfiguration = nullptr;
                    postConfiguration = nullptr;
                    create_main_conf = nullptr;
                    init_main_conf = nullptr;
                    create_srv_conf = nullptr;
                    merge_srv_conf = nullptr;
                    create_loc_conf = nullptr;
                    merge_loc_conf = nullptr;
                }

                /**pre-configuration 解析配置文件前被调用*/
                ngx_int_t (*preConfiguration)(ngx_conf_t *cf);

                /**post-configuration 解析配置文件后被调用*/
                ngx_int_t (*postConfiguration)(ngx_conf_t *cf);

                /**create_main_conf 创建http main域的配置结构*/
                void *(*create_main_conf)(ngx_conf_t *cf);

                /**init_main_conf 初始化http main域的配置结构*/
                char *(*init_main_conf)(ngx_conf_t *cf, void *conf);

                /**create_srv_conf 创建server域的配置结构*/
                void *(*create_srv_conf)(ngx_conf_t *cf);

                /**merge_srv_conf 合并server域的配置结构*/
                char *(*merge_srv_conf)(ngx_conf_t *cf, void *prev, void *conf);

                /**create_loc_conf 创建location域的配置结构*/
                void *(*create_loc_conf)(ngx_conf_t *cf);

                /**merge_loc_conf 合并location域的配置结构*/
                char *(*merge_loc_conf)(ngx_conf_t *cf, void *prev, void *conf);

                explicit operator ngx_http_module_t()
                {
                    return {
                            preConfiguration, postConfiguration,
                            create_main_conf, init_main_conf,
                            create_srv_conf, merge_srv_conf,
                            create_loc_conf, merge_loc_conf
                    };
                }
            };

            /*
            class HttpModuleBuilder final
            {
            };

            class NginxModuleFactory
            {
            };
            */
        }
    }
}
#endif //NGINX__MODULE_UTIL__MODULE_ENTRY__NGINX_MODULE_H
