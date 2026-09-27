#include <astra/graphics/font/font_family.hpp>
#include <astra/core/types.hpp>
#include <astra/core/assert.hpp>
#include <astra/graphics/font/font_face.hpp>
#include <astra/core/resource_manager.hpp>

namespace astra::graphics {
    void FontFamily::addFace(const std::string_view &filePath) {
        m_faces.emplace_back(core::CreateScope<FontFace>(resourceManager, library, filePath));
    }

    FontFace &FontFamily::resolve(const FontStyle &style) const {
        int bestFace = -1;
        uint32_t bestScore = UINT32_MAX;

        for (int i = 0; i < m_faces.size(); ++i) {
            const uint32_t score = m_faces[i]->score(style);
            if (score < bestScore) {
                bestScore = score;
                bestFace = i;
                if (bestScore == 0) break;
            }
        }

        ASSERT(bestFace >= 0);

        return *m_faces[bestFace];
    }

    const std::string &FontFamily::name() const {
        return m_name;
    }
}
