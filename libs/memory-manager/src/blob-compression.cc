/*
 * Scroom - Generic viewer for 2D data
 * Copyright (C) 2009-2012 Kees-Jan Dijkzeul
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License, version 2, as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "blob-compression.hh"

namespace Scroom
{
  namespace MemoryBlobs
  {
    namespace Detail
    {
      PageList compressBlob(const uint8_t* in, size_t size, PageProvider::Ptr provider)
      {
        return PageList();
      }
      
      void decompressBlob(uint8_t* out, size_t size, PageList list)
      {
      }
    }
  }
}

