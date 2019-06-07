//
// Created by ubuntu on 19-6-8.
//

#ifndef NGINX__NGINX_SRC__NGINX_SRC_H
#define NGINX__NGINX_SRC__NGINX_SRC_H

extern "C"
{
#include "./1.16.0/src/core/nginx.h"//含有版本号
#include "./1.16.0/src/core/ngx_config.h"//含有整数相关定义
#include "./1.16.0/src/core/ngx_core.h"//含有错误类型码
#include "./1.16.0/src/core/ngx_cycle.h"
#include "./1.16.0/src/core/ngx_module.h"
#include "./1.16.0/src/core/ngx_times.h"//含有时间相关的
#include "./1.16.0/src/os/unix/ngx_time.h"
#include "./1.16.0/src/core/ngx_parse_time.h"
};
#endif //NGINX__NGINX_SRC__NGINX_SRC_H
