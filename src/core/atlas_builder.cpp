#include <iostream>
#include <astra/core/atlas_builder.hpp>

namespace astra::core {
    AtlasBuilder::AtlasBuilder(const math::Vec2 &size) {
        atlas = assets::Image{static_cast<int>(size.x), static_cast<int>(size.y), 4};
        atlas.pixels.resize(size.x * size.y * 4, 129);

        sizes.emplace_back(size);
        positions.emplace_back(0, 0);
    }

    void AtlasBuilder::add(const assets::Image &image, const uint32_t padding) {
        uint32_t bestShortFit = UINT32_MAX;
        uint32_t bestLongFit = UINT32_MAX;
        int bestFreeIndex = -1;

        const math::Vec2 imageSize = {
            static_cast<float>(image.width + padding * 2),
            static_cast<float>(image.height + padding * 2)
        };

        for (int i = 0; i < sizes.size(); ++i) {
            if (imageSize.x > sizes[i].x || imageSize.y > sizes[i].y) {
                continue;
            }

            const math::Vec2 leftover = sizes[i] - imageSize;
            const uint32_t shortFit = std::min(leftover.x, leftover.y);
            const uint32_t longFit = std::max(leftover.x, leftover.y);

            if (shortFit < bestShortFit || (shortFit == bestShortFit && longFit < bestLongFit)) {
                bestFreeIndex = i;
                bestShortFit = shortFit;
                bestLongFit = longFit;
            }
        }

        if (bestFreeIndex == -1) {
            std::cout << "No space\n";
            return;
        }

        math::Vec2 freeSize = sizes[bestFreeIndex];
        math::Vec2 freePos = positions[bestFreeIndex];

        int channels = 4;
        int startX = static_cast<int>(freePos.x + padding);
        int startY = static_cast<int>(freePos.y + padding);

        for (int row = 0; row < image.height; ++row) {
            auto srcStart = image.pixels.begin() + (row * image.width * channels);
            auto srcEnd = srcStart + (image.width * channels);

            int atlasRow = startY + row;
            auto destStart = atlas.pixels.begin() + ((atlasRow * atlas.width + startX) * channels);

            std::copy(srcStart, srcEnd, destStart);
        }

        positions.erase(positions.begin() + bestFreeIndex);
        sizes.erase(sizes.begin() + bestFreeIndex);

        if (freeSize.x > imageSize.x) {
            positions.emplace_back(freePos.x + imageSize.x, freePos.y);
            sizes.emplace_back(freeSize.x - imageSize.x, imageSize.y);
        }

        if (freeSize.y > imageSize.y) {
            positions.emplace_back(freePos.x, freePos.y + imageSize.y);
            sizes.emplace_back(freeSize.x, freeSize.y - imageSize.y);
        }
    }
}
