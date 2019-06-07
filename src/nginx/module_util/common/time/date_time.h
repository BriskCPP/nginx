//
// Created by ubuntu on 19-6-8.
//

#ifndef NGINX__MODULE_UTIL__COMMON__TIME__DATE_TIME_H
#define NGINX__MODULE_UTIL__COMMON__TIME__DATE_TIME_H

#include "../../../nginx_src/nginx_src.h"

namespace nginx
{
    namespace module_util
    {
        namespace common
        {
            namespace time
            {
                class DateTime final
                {
                private:
                    ngx_time_t ngxTime;

                    //DateTime() = default;

                    explicit DateTime(
                            const ngx_time_t &ngxTime = {
                                    ngx_cached_time->sec,
                                    ngx_cached_time->msec,
                                    ngx_cached_time->gmtoff
                            }
                    ) : ngxTime{ngxTime.sec, ngxTime.msec, ngxTime.gmtoff}
                    {
                    }

                public:
                    enum Zone
                    {
                        Local,
                        GMT
                    };

                    static const DateTime get(bool updateNow = false)
                    {
                        if (updateNow) ngx_time_update();
                        return DateTime();
                    }


                    //下面是具体的函数
                    const int millisecond() const
                    {
                        return static_cast<int>(ngxTime.msec);
                    }

                    const int seconds(Zone zone = Local) const
                    {
                        ngx_tm_t temp;
                        switch (zone)
                        {

                            case GMT:
                                ngx_gmtime(this->ngxTime.sec, &temp);
                                return temp.tm_sec;
                            case Local:
                            default:
                                ngx_localtime(this->ngxTime.sec, &temp);
                                return temp.tm_sec;
                        }
                    }

                    const int minutes(Zone zone = Local) const
                    {
                        ngx_tm_t temp;
                        switch (zone)
                        {

                            case GMT:
                                ngx_gmtime(this->ngxTime.sec, &temp);
                                return temp.tm_min;
                            case Local:
                            default:
                                ngx_localtime(this->ngxTime.sec, &temp);
                                return temp.tm_min;
                        }
                    }

                    const int hours(Zone zone = Local) const
                    {
                        ngx_tm_t temp;
                        switch (zone)
                        {

                            case GMT:
                                ngx_gmtime(this->ngxTime.sec, &temp);
                                return temp.tm_hour;
                            case Local:
                            default:
                                ngx_localtime(this->ngxTime.sec, &temp);
                                return temp.tm_hour;
                        }
                    }


                    const int month(Zone zone = Local) const
                    {
                        ngx_tm_t temp;
                        switch (zone)
                        {

                            case GMT:
                                ngx_gmtime(this->ngxTime.sec, &temp);
                                return temp.tm_mon;
                            case Local:
                            default:
                                ngx_localtime(this->ngxTime.sec, &temp);
                                return temp.tm_mon;
                        }
                    }

                    const int year(Zone zone = Local) const
                    {
                        ngx_tm_t temp;
                        switch (zone)
                        {

                            case GMT:
                                ngx_gmtime(this->ngxTime.sec, &temp);
                                return temp.tm_year;
                            case Local:
                            default:
                                ngx_localtime(this->ngxTime.sec, &temp);
                                return temp.tm_year;
                        }
                    }

                    const int dayOfMonth(Zone zone = Local) const
                    {
                        ngx_tm_t temp;
                        switch (zone)
                        {

                            case GMT:
                                ngx_gmtime(this->ngxTime.sec, &temp);
                                return temp.tm_mday;
                            case Local:
                            default:
                                ngx_localtime(this->ngxTime.sec, &temp);
                                return temp.tm_mday;
                        }
                    }

                    const int dayOfWeek(Zone zone = Local) const
                    {
                        ngx_tm_t temp;
                        switch (zone)
                        {

                            case GMT:
                                ngx_gmtime(this->ngxTime.sec, &temp);
                                return temp.tm_wday;
                            case Local:
                            default:
                                ngx_localtime(this->ngxTime.sec, &temp);
                                return temp.tm_wday;
                        }
                    }

                    const int dayOfYear(bool firstDayAs1 = true, Zone zone = Local) const
                    {
                        ngx_tm_t temp;
                        switch (zone)
                        {

                            case GMT:
                                ngx_gmtime(this->ngxTime.sec, &temp);
                                if (firstDayAs1) return (temp.tm_yday + 1);
                                else return temp.tm_yday;
                            case Local:
                            default:
                                ngx_localtime(this->ngxTime.sec, &temp);
                                if (firstDayAs1) return (temp.tm_yday + 1);
                                else return temp.tm_yday;
                        }
                    }
                };
            }
        }
    }
}
#endif //NGINX__MODULE_UTIL__COMMON__TIME__DATE_TIME_H
