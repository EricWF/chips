#ifndef CHIPS_LOG_HPP
#define CHIPS_LOG_HPP

# include <elib/log.hpp>

namespace chips
{
    struct chips_log_tag {};
    
    using elib::log::level_e;
    using log = elib::log::static_log<chips_log_tag>;
}                                                           // namespace chips
#endif /* CHIPS_LOG_HPP */