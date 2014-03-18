#ifndef CHIPS_CHIPS_MAIN_HPP
#define CHIPS_CHIPS_MAIN_HPP

namespace chips
{
    /// NOTE: indirection makes the build system much easier to handle
    int chips_main(int, char**, char**);
	void level_test(const unsigned int);
	void menu_test();
}                                                           // namespace chips
#endif /* CHIPS_CHIPS_MAIN_HPP */
