#include "chips/level.hpp"
#include "chips/error.hpp"
#include <fstream>

namespace chips
{
    level read_level(std::string const & fname)
    {
        std::ifstream in(fname, std::ios::in | std::ios::binary);
        if (!in.is_open()) throw chips_error{"Failed to open level file: " + fname};
        
        level dest;
        in.read(reinterpret_cast<char*>(&dest), sizeof(level));
        if (!in) throw chips_error{"Failed to read level file: " + fname};
        in.close();
        return dest;
    }
    
    void write_level(std::string const & fname, level const & l)
    {
        std::ofstream out(fname, std::ios::out | std::ios::binary);
        if (!out.is_open()) throw chips_error{"Failed to open level file: " + fname};
        out.write(reinterpret_cast<const char*>(&l), sizeof(level));
        if (!out) throw chips_error{"Failed to write level to file: " + fname};
        out.close();
    }
}                                                           // namespace chips