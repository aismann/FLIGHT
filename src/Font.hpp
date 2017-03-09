#pragma once

//#include <ft2build.h>
//#include FT_FREETYPE_H
#include <memory>
#include <stdexcept>

namespace FLIGHT {
class FontFace {
    friend class Game;
    static void Init();

public:
    static std::shared_ptr<FontFace> New(const std::string & path, size_t size);
};
}
