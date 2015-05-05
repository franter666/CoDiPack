/**
 * CoDiPack, a Code Differentiation Package
 *
 * Copyright (C) 2015 Chair for Scientific Computing, TU Kaiserslautern
 *
 * This file is part of CoDiPack.
 *
 * CoDiPack is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 *
 * CoDiPack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU
 * General Public License along with CoDiPack.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: TODO
 */
#pragma once

#include "chunk.hpp"

namespace codi {
  template<typename ChunkData>
  class ChunkVector {
  public:

    struct Position {
      size_t chunkIndex;
      size_t chunkPosition;
    };

  private:
    std::vector<ChunkData* > chunks;

    ChunkData* curChunk;

    size_t chunkSize;

    Position curPosition;

  public:
    ChunkVector(const size_t& chunkSize) : chunkSize(chunkSize), curPosition(0, 0) {
      chunks.push_back(new Chunk<Data>(chunkSize));
      curChunk = chunks[0];
    }

    void nextChunk() {
      curChunk.setUsedSize(curPosition.chunkPosition);
      curChunk->store();

      curPosition.chunkIndex += 1;
      if(chunks.size() == curPosition.chunkIndex) {
        curChunk = new ChunkData(chunkSize);
        chunks.push_back(curChunk);
      } else {
        curChunk = chunks[curPosition.chunkIndex];
        curChunk->reset();
      }
    }

    void reset(const Position& pos) {
      assert(pos.chunkIndex < chunks.size());
      assert(pos.chunkPosition < chunkSize);

      curPosition = pos;
      curChunk = chunks[curPosition.chunkIndex];
      curChunk->load();
    }

    void reset() {
      reset(Position(0, 0));
    }

    void reserveItems(const size_t& items) {
      assert(items < chunkSize);

      if(chunkSize < curPosition.chunkPosition + items) {
        nextChunk();
      }
    }

    void useItems(const size_t& items) {
      // This method shuld only be called if 'reserveItems' has been called
      assert(chunkSize < curPosition.chunkPosition + items);

      curPosition.chunkPosition += items;
    }
  };
}
