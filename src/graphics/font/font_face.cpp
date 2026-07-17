#include <freetype/freetype.h>
#include <freetype/ftmm.h>
#include <freetype/tttables.h>

#include <astra/graphics/font/font_face.hpp>
#include <astra/graphics/font/font_instance.hpp>

namespace astra::graphics {
    FontFace::FontFace(core::ResourceManager *resourceManager,
                       const FT_Library library,
                       const std::string_view &fontPath) : resourceManager(resourceManager), m_face(), m_italic(false) {
        const auto status = FT_New_Face(library, fontPath.data(), 0, &m_face);
        ASSERT(status == FT_Err_Ok);

        const auto os2 =
                static_cast<TT_OS2 *>(FT_Get_Sfnt_Table(m_face, FT_SFNT_OS2));

        ASSERT(os2 != nullptr);

        m_weight = os2->usWeightClass;
        m_width = os2->usWidthClass;
        if (m_face->style_flags & FT_STYLE_FLAG_ITALIC)
            m_italic = true;

        FT_MM_Var *mmVar = nullptr;
        FT_Get_MM_Var(m_face, &mmVar);
        if (mmVar == nullptr) return;

        ASSERT(mmVar->num_axis <= MAX_AXES)
        m_axes.defaultCoords.count = mmVar->num_axis;

        for (int i = 0; i < mmVar->num_axis; ++i) {
            const FT_Var_Axis &axis = mmVar->axis[i];
            switch (axis.tag) {
                case font_tags::weight:
                    m_axes.weight = i;
                    break;

                case font_tags::width:
                    m_axes.width = i;
                    break;

                case font_tags::italic:
                    m_axes.italic = i;
                    break;

                default:
                    m_axes.customAxisIndices.push_back(i);
                    break;
            }

            m_axes.defaultCoords.values[i] = axis.def;
            m_axes.axes.emplace_back(static_cast<uint32_t>(axis.tag),
                                     i,
                                     axis.minimum >> 16,
                                     axis.maximum >> 16,
                                     axis.def >> 16);
        }
        FT_Done_MM_Var(library, mmVar);
    }

    FontFace::~FontFace() {
        FT_Done_Face(m_face);
    }

    uint32_t FontFace::score(const FontStyle &style) const {
        uint32_t score = 0;

        int weight = m_weight;
        int width = m_width;
        bool italic = m_italic;

        if (!m_axes.axes.empty()) {
            if (m_axes.weight != -1) {
                weight = std::clamp(static_cast<int>(style.weight),
                                    m_axes.axes[m_axes.weight].minimum,
                                    m_axes.axes[m_axes.weight].maximum);
            }

            if (m_axes.width != -1) {
                width = std::clamp(static_cast<int>(style.width),
                                   m_axes.axes[m_axes.width].minimum,
                                   m_axes.axes[m_axes.width].maximum);
            }

            if (m_axes.italic != -1) {
                italic = std::clamp(style.italic,
                                    static_cast<bool>(m_axes.axes[m_axes.italic].minimum),
                                    static_cast<bool>(m_axes.axes[m_axes.italic].maximum));
            }
        };

        score += std::abs(weight - static_cast<int>(style.weight)) * WEIGHT_PENALITY_MULTIPLIER;
        score += std::abs(width - static_cast<int>(style.width)) * WIDTH_PENALITY_MULTIPLIER;

        if (italic != style.italic) {
            score += ITALIC_PENALTY;
        }

        for (const auto &[tag, value]: style.axes()) {
            if (!hasAxis(tag)) {
                score += CUSTOM_AXIS_PENALITY;
            }
        }
        return score;
    }

    FontInstance &FontFace::getInstance(const FontStyle &style) {
        auto axisCoords = m_axes.defaultCoords;

        if (m_axes.weight != -1)
            axisCoords.values[m_axes.weight] = style.weight << 16;

        if (m_axes.width != -1)
            axisCoords.values[m_axes.width] = style.width << 16;

        if (m_axes.italic != -1)
            axisCoords.values[m_axes.italic] = style.italic << 16;

        for (const auto [tag, value]: style.axes()) {
            for (const auto axisIndex: m_axes.customAxisIndices) {
                if (m_axes.axes[axisIndex].tag == tag) {
                    axisCoords.values[axisIndex] = value << 16;
                }
            }
        }

        for (auto &instance: m_instances) {
            if (instance.size() == style.size && instance.coordValues() == axisCoords) {
                return instance;
            }
        }

        m_instances.emplace_back(resourceManager, m_face, style.size, axisCoords);
        return m_instances.back();
    }

    bool FontFace::hasAxis(const uint32_t tag) const {
        for (const auto axisIndex: m_axes.customAxisIndices) {
            if (m_axes.axes[axisIndex].tag == tag) {
                return true;
            }
        }

        return false;
    }
}
