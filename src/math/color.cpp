#include "math/color.hpp"

namespace astra::math {
Color Color::alphaBlend(const Color& dst, const Color& src) {
    if (src.a == 1)
        return src;
    else if (src.a == 0)
        return dst;

    Color final;
    final.r = (src.r * src.a + dst.r * (255 - src.a)) / 255;
    final.g = (src.g * src.a + dst.g * (255 - src.a)) / 255;
    final.b = (src.b * src.a + dst.b * (255 - src.a)) / 255;

    return final;
}
}
