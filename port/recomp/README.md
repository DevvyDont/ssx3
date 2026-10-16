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
  - MPEG movies that pull their data through the game's Nodata callback, and end when the stream does;
  - a way for a game to move the runtime's scratch allocations out of its own heap;
  - the debug panel hidden until F1 is pressed (or `PS2X_DEBUG_UI=1`);
  - VIF `UNPACK V4-5` expanding 5:5:5:1 colours to 8 bits per channel, as the hardware does;
  - a guest call that yields before its callee starts no longer counts as finished when the callee is the
    function the call came from (EA's file system recurses this way while a race loads);
  - `BLTZ`/`BGEZ`/`BLEZ`/`BGTZ` (and their likely/link forms) testing the whole 64-bit register, like `SLT` and
    `BEQ` already do; EA's file table compares hashes with `LWU`+`DSUBU`+`BLTZ`, and a 32-bit test sorted it
    wrongly, so some riders' texture archives were never found;
  - debug switches.
- `ssx3_overrides.cpp` holds the game-specific replacements: it skips the network and USB keyboard drivers,
  answers the module search, routes `isceSifSendCmd`, and keeps the runtime's scratch buffers in the
  0x80000-0x100000 gap below the ELF, since the game's own allocator owns everything above it.
- `ssx3.toml.in` is the translator config (the analyzer's stub and entry lists). `gen_funcs.py` and
  `function_splits.txt` produce its function map.

## Building

You need a 32-bit toolchain and X/GL libraries, plus the decomp's Python venv, which provides spimdisasm.
On Debian/Ubuntu:

```bash
sudo dpkg --add-architecture i386 && sudo apt update
sudo apt install g++-multilib cmake ninja-build libgl-dev:i386 libx11-dev:i386 libxrandr-dev:i386 \
    libxinerama-dev:i386 libxcursor-dev:i386 libxi-dev:i386 \
    libavcodec-dev:i386 libavformat-dev:i386 libavutil-dev:i386 libswresample-dev:i386 libswscale-dev:i386
cmake -S port/recomp -B port/recomp/build -G Ninja -DSSX3PC_ELF=/path/to/SLUS_207.72 -DSSX3PC_RUNTIME_JOBS=4
ninja -C port/recomp/build
```

The translator turns about 14,700 functions into the same number of C++ files, so the runtime build is long.
Each compile job needs up to about 3 GB of memory, and that's what `SSX3PC_RUNTIME_JOBS` is for.

FFmpeg plays the game's movies. Pass `-DSSX3PC_FFMPEG=OFF` to build without it (the game then waits forever on
the first movie), or `-DSSX3PC_PKG_CONFIG_PATH=...` if your 32-bit FFmpeg lives elsewhere.

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
| `PS2X_CD_LOG=1` | CD reads and `sceCdCallback` calls |
| `PS2X_FILE_LOG=1` | the game's file lookups and their results, each file command it queues, and each one that fails |
| `PS2X_MPEG_LOG=1` | MPEG callbacks, data fed to the decoder, and the end of each movie |
| `PS2X_HEAP_LOG=1` | the runtime's scratch allocations |
| `PS2X_DUMP_RDRAM=file` with `PS2X_DUMP_RDRAM_TICK=n` | EE RAM, written to `file` at frame `n` |
| `PS2X_DUMP_RDRAM_ON_FAULT=file` | EE RAM, written to `file` the first time guest code jumps to an unknown address |
| `PS2X_VU1_LOG=1` | how often each VU1 microprogram runs, and VIF1 codes the interpreter skips |
| `PS2X_GS_SAMPLE=flagfile` | once `flagfile` exists, 3000 non-sprite primitives drawn to the main framebuffer |

Keyboard: Enter is START, X is Cross, Z is Square; F1 shows the debug panel.

## Status

With the full disc, the game plays the EA SPORTS BIG logo movie, shows the title screen, and takes keyboard
input through the front end to a race. In the race, the HUD, sky, distant mountains and the slope draw; the
riders and close scenery do not yet. Sound has not been tested yet.

When a guest address with no function is reached, the scheduler prints its last 64 dispatches (`[sched-trace]`).
