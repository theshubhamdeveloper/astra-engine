#include <iostream>
#include <vector>
#include <algorithm>
#include <astra/core/atlas_builder.hpp>
#include "astra/core/assert.hpp"

namespace astra::core {
    AtlasBuilder::AtlasBuilder(const math::uvec2 &size, const int colorChannels) {
        atlas = assets::Image{static_cast<int>(size.x), static_cast<int>(size.y), colorChannels};
        atlas.pixels.resize(size.x * size.y * 4, 0);
        freeRects.push_back({{0, 0}, size});
    }

    AtlasRegion AtlasBuilder::add(const assets::Image &image, const uint32_t padding) {
        const auto imageSize = math::uvec2{
            image.width + padding * 2, image.height + padding * 2
        };

        const int freeIndex = bestFreeIndex(imageSize);
        if (freeIndex == -1) {
            return {};
        }

        auto [pos, size] = freeRects[freeIndex];

        copyImage(pos, image, padding);
        spilt(Rect{pos, imageSize});

        const math::vec2 atlasSize = {static_cast<float>(atlas.width), static_cast<float>(atlas.height)};
        return {
            pos.x / atlasSize.x,
            pos.y / atlasSize.y,
            (pos.x + imageSize.x) / atlasSize.x,
            (pos.y + imageSize.y) / atlasSize.y,
        };
    }

    int AtlasBuilder::bestFreeIndex(const math::uvec2 &size) const {
        float bestShortFit = MAXFLOAT;
        float bestLongFit = MAXFLOAT;
        int freeIndex = -1;

        for (size_t i = 0; i < freeRects.size(); ++i) {
            if (size.x > freeRects[i].size.x || size.y > freeRects[i].size.y) continue;

            const math::uvec2 leftover = freeRects[i].size - size;
            const float shortFit = std::min(leftover.x, leftover.y);
            const float longFit = std::max(leftover.x, leftover.y);

            if (shortFit < bestShortFit || (shortFit == bestShortFit && longFit < bestLongFit)) {
                freeIndex = static_cast<int>(i);
                bestShortFit = shortFit;
                bestLongFit = longFit;
            }
        }
        return freeIndex;
    }

    void AtlasBuilder::spilt(const Rect &placedRect) {
        std::vector<Rect> nextFreeRects;
        nextFreeRects.reserve(freeRects.size() * 2);

        for (const auto &freeRect: freeRects) {
            if (!freeRect.intersects(placedRect)) {
                nextFreeRects.push_back(freeRect);
                continue;
            }

            // Top
            if (placedRect.pos.y > freeRect.pos.y) {
                nextFreeRects.push_back({freeRect.pos, {freeRect.size.x, placedRect.pos.y - freeRect.pos.y}});
            }

            // Bottom
            if (placedRect.bottom() < freeRect.bottom()) {
                nextFreeRects.push_back({
                    {freeRect.pos.x, placedRect.bottom()},
                    {freeRect.size.x, freeRect.bottom() - placedRect.bottom()}
                });
            }

            // Left
            if (placedRect.pos.x > freeRect.pos.x) {
                nextFreeRects.push_back({freeRect.pos, {placedRect.pos.x - freeRect.pos.x, freeRect.size.y}});
            }

            // Right
            if (placedRect.pos.x + placedRect.size.y < freeRect.pos.x + freeRect.size.x) {
                nextFreeRects.push_back({
                    {placedRect.right(), freeRect.pos.y},
                    {freeRect.right() - placedRect.right(), freeRect.size.y}
                });
            }
        }

        for (size_t i = 0; i < nextFreeRects.size(); ++i) {
            for (size_t j = 0; j < nextFreeRects.size(); ++j) {
                if (i == j) continue;
                if (nextFreeRects[j].contains(nextFreeRects[i])) {
                    nextFreeRects[i].size = {0, 0};
                    break;
                }
            }
        }

        nextFreeRects.erase(
            std::ranges::remove_if(nextFreeRects, [](const Rect &r) {
                return r.size.x <= 0 || r.size.y <= 0;
            }).begin(),
            nextFreeRects.end()
        );

        freeRects = std::move(nextFreeRects);
    }

    void AtlasBuilder::copyImage(const math::uvec2 &position, const assets::Image &image, const uint32_t padding) {
        ASSERT(atlas.channels == image.channels);

        const math::uvec2 start = position + math::uvec2(padding);
        const size_t rowStride = image.width * atlas.channels;

        for (int row = 0; row < image.height; ++row) {
            const auto srcStart = image.pixels.begin() + (row * rowStride);
            const auto destStart = atlas.pixels.begin() + (((start.y + row) * atlas.width + start.x) * atlas.channels);

            std::copy_n(srcStart, rowStride, destStart);
        }
    }
}
