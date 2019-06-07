#include <iostream>

#include "module_util/module_util.h"

int main()
{
    using nginx::module_util::common::time::DateTime;
    ngx_time_init();
    DateTime dateTime = DateTime::get();
    std::cout << dateTime.year() << "-" << dateTime.month() << "-" << dateTime.dayOfMonth()
              << " " << dateTime.dayOfWeek()
              << " " << dateTime.dayOfYear(true)
              << std::endl;
    std::cout << dateTime.hours() << ":" << dateTime.minutes() << ":" << dateTime.seconds() << "." << dateTime.millisecond() << std::endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}