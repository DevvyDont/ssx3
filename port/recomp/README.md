# SSX 3 on the PS2Recomp runtime

This builds SSX 3 for 32-bit Linux by translating the retail `SLUS_207.72` to C++ with
[PS2Recomp](https://github.com/ran-j/PS2Recomp) and linking it against PS2Recomp's runtime, with our fixes
applied. The translated code is generated in the build directory and is never committed. Neither is the ELF
or any disc file.

## What's here

- `CMakeLists.txt` fetches PS2Recomp at a fixed commit, applies `ps2recomp.patch`, finds function boundaries
  with spimdisasm, translates the ELF, then builds the runtime 32-bit.
- `ps2recomp.patch` holds our runtime changes. None of them are SSX 3 specific, so they could go upstream:
  - a 32-bit build, with EE RAM identity-mapped at its PS2 addresses;
  - EE→IOP SIF commands;
  - `sceCdCallback`;
  - semaphores with a max count of 0;
  - callback stacks kept clear of the main thread's stack;
  - GS CSR FIFO status;
  - a resident-module list for `sceSifSearchModuleByName`;
  - debug switches.
- `ssx3_overrides.cpp` holds the game-specific replacements: it skips the network and USB keyboard drivers,
  answers the module search, and routes `isceSifSendCmd`.
- `ssx3.toml.in` is the translator config (the analyzer's stub and entry lists). `gen_funcs.py` and
  `function_splits.txt` produce its function map.

## Building

You need a 32-bit toolchain and X/GL libraries, plus the decomp's Python venv, which provides spimdisasm.
On Debian/Ubuntu:

```bash
sudo dpkg --add-architecture i386 && sudo apt update
sudo apt install g++-multilib cmake ninja-build libgl-dev:i386 libx11-dev:i386 libxrandr-dev:i386 \
    libxinerama-dev:i386 libxcursor-dev:i386 libxi-dev:i386
cmake -S port/recomp -B port/recomp/build -G Ninja -DSSX3PC_ELF=/path/to/SLUS_207.72 -DSSX3PC_RUNTIME_JOBS=4
ninja -C port/recomp/build
```

The translator turns about 14,700 functions into the same number of C++ files, so the runtime build is long.
Each compile job needs up to about 3 GB of memory, and that's what `SSX3PC_RUNTIME_JOBS` is for.

## Running

Run it from a folder holding the extracted disc, and point `PS2X_CD_IMAGE` at an ISO of the disc:

```bash
cd /path/to/extracted-disc
PS2X_CD_IMAGE=/path/to/ssx3.iso /path/to/port/recomp/build/runtime/ps2xRuntime/ps2EntryRunner "$PWD/SLUS_207.72"
```

These debug switches print to stderr:

| Variable | Prints |
| --- | --- |
| `PS2X_DUMP_THREADS=1` | the EE thread table every 5 s, with return addresses from each stack |
| `PS2X_SIF_LOG=1` | SIF commands in both directions |
| `PS2X_CD_LOG=1` | CD reads |
| `PS2X_FILE_LOG=1` | the game's file lookups and their results |

## Status

The game loads its IOP modules, sets up the app manager, loads its sound banks and enters the front end. It
then waits on files that the test disc image didn't have: `MUSIC.BIG`, `MUSIC2.BIG`, `UI/MAPGFX.BIG` and
`DATA/MOVIES`. Getting past that needs the full disc.
