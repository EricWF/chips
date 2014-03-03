#ifndef CHIPS_TEXTURE_INFO_HPP
#define CHIPS_TEXTURE_INFO_HPP

# include <string>

namespace chips
{
    struct texture_info
    {
        std::string id;
        int x, y, width, height;
        int frame;
    };
}                                                           // namespace chips
#endif /* CHIPS_TEXTURE_INFO_HPP */