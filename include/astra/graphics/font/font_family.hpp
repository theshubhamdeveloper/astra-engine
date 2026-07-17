#pragma once

#include <vector>

#include <astra/core/types.hpp>
#include <astra/graphics/font/font_style.hpp>
#include <astra/graphics/font/font_face.hpp>

namespace astra::core {
    class ResourceManager;
}

namespace astra::graphics {
    class FontFamily {
        std::vector<core::Scope<FontFace> > m_faces;
        core::ResourceManager *resourceManager;
        FT_Library library;

        std::string m_name;

    public:
        struct Desc {
            core::ResourceManager *resourceManager;
            FT_Library library;
        };

        FontFamily() : resourceManager(), library() {
        }

        explicit FontFamily(const Desc &desc) : resourceManager(desc.resourceManager), library(desc.library) {
        }

        void addFace(const std::string_view &filePath);

        [[nodiscard]] FontFace &resolve(const FontStyle &style) const;

        const std::string &name() const;
    };
}
