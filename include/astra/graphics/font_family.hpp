#pragma once
#include <vector>

#include <astra/graphics/font_face.hpp>

namespace astra::graphics {
    class FontFamily {
        std::vector<FontFace> m_faces;
        core::ResourceManager *resourceManager;
        FT_Library library;

        std::string name;

    public:
        struct Desc {
            core::ResourceManager *resourceManager;
            FT_Library library;
        };

        FontFamily() : resourceManager(), library() {
        }

        explicit FontFamily(const Desc &desc) : resourceManager(desc.resourceManager), library(desc.library) {
        }

        void addFace(const std::string_view &filePath) {
            m_faces.emplace_back(resourceManager, library, filePath);
        }

        FontFace &resolve(const FontStyle &style) {
            int bestFace = -1;
            uint32_t bestScore = UINT32_MAX;

            for (int i = 0; i < m_faces.size(); ++i) {
                const uint32_t score = m_faces[i].score(style);
                if (score < bestScore) {
                    bestScore = score;
                    bestFace = i;
                    if (bestScore == 0) break;
                }
            }

            ASSERT(bestFace >= 0);

            return m_faces[bestFace];
        }
    };
}
