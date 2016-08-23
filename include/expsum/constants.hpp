// -*- mode: c++; fill-column: 80; indent-tabs-mode: nil; -*-

#ifndef EXPSUM_CONSTANTS_HPP
#define EXPSUM_CONSTANTS_HPP

namespace expsum
{
template <typename T>
struct constant
{
    //! circle ratio $$\pi$$
    constexpr static const T pi = T(
        3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679);
    //! $$ pi/2 $$
    constexpr static const T pi_half = pi / 2;
    //! $$ \sqrt{pi} $$
    constexpr static const T sqrt_pi = T(
        1.7724538509055160272981674833411451827975494561223871282138077898529112845910321813749506567385446654);
    //! Base of natural logarithm
    constexpr static const T e = T(
        2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274);
    //! Euler constant
    constexpr static const T euler = T(
        0.5772156649015328606065120900824024310421593359399235988057672348848677267776646709369470632917467495);

    //! sqrt(2)
    constexpr static const T sqrt2 = T(
        1.4142135623730950488016887242096980785696718753769480731766797379907324784621070388503875343276415727);
};

} // namespace: expsum

#endif /* EXPSUM_CONSTANTS_HPP */