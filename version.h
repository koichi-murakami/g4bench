/*============================================================================
  Copyright 2017-2019 Koichi Murakami

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file LICENSE for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifndef VERSION_H_
#define VERSION_H_

#define G4BENCH_VERSION_MAJOR "1"
#define G4BENCH_VERSION_MINOR "5.3"
#define G4BENCH_VERSION_BUILD 0x755af12

namespace {
  const int build_head = (G4BENCH_VERSION_BUILD & 0xffff000) >> 12;
  const int build_tail = G4BENCH_VERSION_BUILD & 0xfff;
} // namespace

#endif
