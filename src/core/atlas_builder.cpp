#include <iostream>
#include <vector>
#include <algorithm>
#include <astra/core/atlas_builder.hpp>
#include "astra/core/assert.hpp"

namespace astra::core {
    AtlasBuilder::AtlasBuilder(const math::Vec2 &size) {
        atlas = assets::Image{static_cast<int>(size.x), static_cast<int>(size.y), 4};
        atlas.pixels.resize(size.x * size.y * 4, 0);
        freeRects.push_back({{0, 0}, size});
    }

    void AtlasBuilder::add(const assets::Image &image, const uint32_t padding) {
        const auto imageSize = math::Vec2{
            static_cast<float>(image.width + padding * 2), static_cast<float>(image.height + padding * 2)
        };

        const int freeIndex = bestFreeIndex(imageSize);
        if (freeIndex == -1) {
            std::cout << "No space\n";
            return;
        }

        auto [pos, size] = freeRects[freeIndex];

        copyImage(pos, image, padding);
        spilt(Rect{pos, imageSize});
    }

    int AtlasBuilder::bestFreeIndex(const math::Vec2 &size) const {
        float bestShortFit = MAXFLOAT;
        float bestLongFit = MAXFLOAT;
        int freeIndex = -1;

        for (size_t i = 0; i < freeRects.size(); ++i) {
            if (size.x > freeRects[i].size.x || size.y > freeRects[i].size.y) continue;

            const math::Vec2 leftover = freeRects[i].size - size;
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
            if (placedRect.pos.y + placedRect.size.y < freeRect.pos.y + freeRect.size.y) {
                nextFreeRects.push_back({
                    {freeRect.pos.x, placedRect.pos.y + placedRect.size.y},
                    {freeRect.size.x, (freeRect.pos.y + freeRect.size.y) - (placedRect.pos.y + placedRect.size.y)}
                });
            }

            // Left
            if (placedRect.pos.x > freeRect.pos.x) {
                nextFreeRects.push_back({freeRect.pos, {placedRect.pos.x - freeRect.pos.x, freeRect.size.y}});
            }

            // Right
            if (placedRect.pos.x + placedRect.size.y < freeRect.pos.x + freeRect.size.x) {
                nextFreeRects.push_back({
                    {placedRect.pos.x + placedRect.size.x, freeRect.pos.y},
                    {(freeRect.pos.x + freeRect.size.x) - (placedRect.pos.x + placedRect.size.x), freeRect.size.y}
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

        // Removing waste
        nextFreeRects.erase(
            std::ranges::remove_if(nextFreeRects, [](const Rect &r) {
                return r.size.x <= 0 || r.size.y <= 0;
            }).begin(),
            nextFreeRects.end()
        );

        freeRects = std::move(nextFreeRects);
    }

    void AtlasBuilder::copyImage(const math::Vec2 &position, const assets::Image &image, const uint32_t padding) {
        ASSERT(atlas.channels == image.channels);

        const int startX = static_cast<int>(position.x + padding);
        const int startY = static_cast<int>(position.y + padding);
        const size_t rowStride = image.width * atlas.channels;

        for (int row = 0; row < image.height; ++row) {
            auto srcStart = image.pixels.begin() + (row * rowStride);
            auto destStart = atlas.pixels.begin() + (((startY + row) * atlas.width + startX) * atlas.channels);

            std::copy(srcStart, srcStart + rowStride, destStart);
        }
    }
}
