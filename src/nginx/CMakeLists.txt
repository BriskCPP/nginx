cmake_minimum_required(VERSION 3.12)
project(NginxWorkbench)

set(CMAKE_CXX_STANDARD 11)



INCLUDE_DIRECTORIES(
        ./nginx_src/1.16.0/objs
        ./nginx_src/1.16.0/src/core
        ./nginx_src/1.16.0/src/event
        ./nginx_src/1.16.0/src/http
        ./nginx_src/1.16.0/src/http/modules
        ./nginx_src/1.16.0/src/http/v2
        ./nginx_src/1.16.0/src/os/unix
)

aux_source_directory(nginx_src/1.16.0/src/core NGXSRC_SRC_CORE)
aux_source_directory(nginx_src/1.16.0/src/event NGXSRC_SRC_EVENT)
aux_source_directory(nginx_src/1.16.0/src/event/modules NGXSRC_SRC_EVENT_MODULES)
list(FILTER NGXSRC_SRC_EVENT_MODULES EXCLUDE REGEX src/event/modules/ngx_kqueue_module.c$)
list(FILTER NGXSRC_SRC_EVENT_MODULES EXCLUDE REGEX src/event/modules/ngx_win32_poll_module.c$)
list(FILTER NGXSRC_SRC_EVENT_MODULES EXCLUDE REGEX src/event/modules/ngx_win32_select_module.c$)

aux_source_directory(nginx_src/1.16.0/src/http NGXSRC_SRC_HTTP)
aux_source_directory(nginx_src/1.16.0/src/http/modules NGXSRC_SRC_HTTP_MODULES)
aux_source_directory(nginx_src/1.16.0/src/http/v2 NGXSRC_SRC_HTTP_V2)
list(FILTER NGXSRC_SRC_HTTP_MODULES EXCLUDE REGEX modules/ngx_http_geoip_module.c$)
list(FILTER NGXSRC_SRC_HTTP_MODULES EXCLUDE REGEX modules/ngx_http_image_filter_module.c$)
list(FILTER NGXSRC_SRC_HTTP_MODULES EXCLUDE REGEX modules/ngx_http_stub_status_module.c$)
list(FILTER NGXSRC_SRC_HTTP_MODULES EXCLUDE REGEX modules/ngx_http_xslt_filter_module.c$)


#aux_source_directory(src/toolset/nginx/nginx_src/src/os/unix NGXSRC_SRC_OS_UNIX)
FILE(GLOB NGXSRC_SRC_OS_UNIX nginx_src/1.16.0/src/os/unix/*.c)

list(FILTER NGXSRC_SRC_OS_UNIX EXCLUDE REGEX darwin.*.c$)
list(FILTER NGXSRC_SRC_OS_UNIX EXCLUDE REGEX freebsd.*.c$)
list(FILTER NGXSRC_SRC_OS_UNIX EXCLUDE REGEX solaris.*.c$)

list(FILTER NGXSRC_SRC_OS_UNIX EXCLUDE REGEX /os/unix/ngx_file_aio_read.c$)
list(FILTER NGXSRC_SRC_OS_UNIX EXCLUDE REGEX /os/unix/ngx_linux_aio_read.c$)


aux_source_directory(nginx_src/1.16.0/objs NGXSRC_OBJS)


project(NginxModule_testmodule)
project(NginxWorkbench_executable)
#这个可执行项目一定要最后再定义  因为CLion自动识别的项目名称是根目录下最后一个Project的名称

ADD_DEFINITIONS(-DNGINX_MODULE_WORKBENCH) #这个宏定义是为了避免编译Nginx源代码中的main函数

#add_library(NginxModule_test
#            ${NGXSRC_SRC_CORE}
#            ${NGXSRC_SRC_EVENT}
#            ${NGXSRC_SRC_EVENT_MODULES}
#            ${NGXSRC_SRC_HTTP}
#            ${NGXSRC_SRC_HTTP_MODULES}
#            ${NGXSRC_SRC_HTTP_V2}
#            ${NGXSRC_SRC_OS_UNIX}
#            ${NGXSRC_OBJS}
#            src/toolset/nginx/module/ngx_test_module.cpp)

add_executable(NginxWorkbench_executable
               ${NGXSRC_SRC_CORE}
               ${NGXSRC_SRC_EVENT}
               ${NGXSRC_SRC_EVENT_MODULES}
               ${NGXSRC_SRC_HTTP}
               ${NGXSRC_SRC_HTTP_MODULES}
               ${NGXSRC_SRC_HTTP_V2}
               ${NGXSRC_SRC_OS_UNIX}
               ${NGXSRC_OBJS}
               test.cpp nginx_src/nginx_src.h)


# TARGET_LINK_LIBRARIES(NginxWorkbench_executable NginxModule_test)


TARGET_LINK_LIBRARIES(NginxWorkbench_executable dl)
TARGET_LINK_LIBRARIES(NginxWorkbench_executable m)
TARGET_LINK_LIBRARIES(NginxWorkbench_executable pcre)
TARGET_LINK_LIBRARIES(NginxWorkbench_executable pthread)
TARGET_LINK_LIBRARIES(NginxWorkbench_executable crypt)
TARGET_LINK_LIBRARIES(NginxWorkbench_executable crypto)
TARGET_LINK_LIBRARIES(NginxWorkbench_executable ssl)
TARGET_LINK_LIBRARIES(NginxWorkbench_executable z)