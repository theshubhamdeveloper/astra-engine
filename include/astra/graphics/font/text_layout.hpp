#pragma once

#include <vector>

#include <astra/graphics/font/glyph.hpp>

#include <astra/graphics/font/text_style.hpp>

namespace astra::graphics {
    struct GlyphRun {
        Glyph glyph;
        math::vec2 position;
    };

    enum class TextAlign {
        Left = 0,
        Center,
        Right,
        Justify
    };

    struct TextRange {
        size_t begin;
        size_t end; // excluded
    };

    struct TextRun {
        TextRange range;
        TextStyle style;
        std::vector<GlyphRun> glyphRuns;
    };

    struct ParagraphStyle {
        TextAlign align;
    };

    struct TextSpan {
        TextRange range;
        TextStyle style;
    };

    struct TextBlock {
        std::string text;
        TextStyle textStyle;
        std::vector<TextSpan> spans;
        ParagraphStyle paragraphStyle;
        math::vec2 constraints;
    };

    class TextLayout {
        std::vector<TextRun> textRuns;

    public:
        TextLayout() {
        }

        void layout(const TextBlock &textBlock) {
            // Runs
            size_t lastRangeEnd = 0;
            for (const auto &span: textBlock.spans) {
                if (lastRangeEnd < span.range.begin) {
                    textRuns.emplace_back(TextRange{lastRangeEnd, span.range.begin}, textBlock.textStyle);
                }

                textRuns.emplace_back(span.range, span.style);
                lastRangeEnd = span.range.end;
            }
            if (lastRangeEnd < textBlock.text.size()) {
                textRuns.emplace_back(TextRange{lastRangeEnd, textBlock.text.size()}, textBlock.textStyle);
            }

            // GlyphRuns

            for (const auto &textRun: textRuns) {
                textRun.glyphRuns.;
            }
        }

        const math::vec2 &bounding() const;

        const math::vec2 &lineBounding(size_t line) const;

        float caretPosition(size_t index) const;

        size_t hitTest(float x) const;


        const core::Rect &selection(const TextRange &range) const;
    };
}
