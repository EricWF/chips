#ifndef CHIPS_CORE_LOG_HPP
#define CHIPS_CORE_LOG_HPP

# include <elib/log.hpp>

namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                               LOG
////////////////////////////////////////////////////////////////////////////////
    
    namespace detail { struct chips_log_tag {}; }
    
    /// the cutoff logging level. It's defined as
    /// enum class level_e
    /// { debug, step, info, warn, err }
    /// usage: level_e::debug, level_e::err...
    using elib::log::level_e;
    
    /// a log class with static logging methods.
    /// usage:
    /// log::debug(const char *msg, ...)
    /// log::step(const char *msg, ...)
    /// NOTE: they have the same signature as printf
    using log = elib::log::static_log<detail::chips_log_tag>;
}                                                           // namespace chips
#endif /* CHIPS_CORE_LOG_HPP */