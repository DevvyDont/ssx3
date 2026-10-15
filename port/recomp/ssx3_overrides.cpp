// SPDX-License-Identifier: GPL-3.0-or-later
// SSX 3 (SLUS_207.72) overrides for the PS2Recomp runtime.
#include "game_overrides.h"
#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace ps2_syscalls
{
    int32_t findResidentSifModule(const std::string &name);
}

namespace ps2_stubs
{
    void sceSifSendCmd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace
{
    constexpr int32_t kUnknownModule = -202; // KE_UNKNOWN_MODULE

    // sceSifSearchModuleByName(const char *name). The libsdk version asks the IOP
    // LOADFILE RPC server, which the runtime does not run (it loads modules
    // itself), so the game spun forever trying to bind it.
    void ssx3SifSearchModuleByName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        (void)runtime;
        ctx->pc = getRegU32(ctx, 31);
        std::string name;
        for (uint32_t addr = getRegU32(ctx, 4); addr != 0u && name.size() < 64u; ++addr)
        {
            const uint8_t *p = getConstMemPtr(rdram, addr);
            if (!p || *p == 0)
                break;
            name.push_back(static_cast<char>(*p));
        }
        const int32_t id = name.empty() ? -1 : ps2_syscalls::findResidentSifModule(name);
        setReturnS32(ctx, id >= 0 ? id : kUnknownModule);
    }

    // func_00319718(loader, const char *irxName, ...) loads an IRX from DATA\MODULES.
    // The PC port has no network adapter: fail the Ethernet and modem drivers the way
    // a console without one does (they cannot link against a non-resident DEV9), so
    // initOnline() sees no network and skips the online stack.
    PS2Runtime::RecompiledFunction g_loadIrxOriginal = nullptr;

    void ssx3LoadIrx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        std::string name;
        for (uint32_t addr = getRegU32(ctx, 5); addr != 0u && name.size() < 64u; ++addr)
        {
            const uint8_t *p = getConstMemPtr(rdram, addr);
            if (!p || *p == 0)
                break;
            name.push_back(static_cast<char>(std::toupper(*p)));
        }
        if (name == "SMAP.IRX" || name == "SPDUART.IRX")
        {
            ctx->pc = getRegU32(ctx, 31);
            setReturnS32(ctx, -200); // KE_LINKERR
            return;
        }
        g_loadIrxOriginal(rdram, ctx, runtime);
    }

    // func_003F61D0 starts the USB keyboard manager (sceUsbKbInit on USBKB.IRX).
    // The runtime has no USB host controller, so the RPC never answers. Report no
    // keyboard support; the game then leaves keyboard text entry disabled. The PC
    // port feeds keyboard input through its own pad layer instead.
    void ssx3UsbKbStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        (void)rdram;
        (void)runtime;
        ctx->pc = getRegU32(ctx, 31);
        setReturnS32(ctx, -1);
    }

    // isceSifSendCmd (0x004261F0) is the interrupt-context twin of sceSifSendCmd.
    // The analyzer stubs only the latter, so the recompiled copy poked SIF DMA
    // registers that reach nothing and IOP modules (the sound driver's bank
    // upload) never saw the command.
    void ssx3iSifSendCmd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = getRegU32(ctx, 31);
        ps2_stubs::sceSifSendCmd(rdram, ctx, runtime);
    }

    // PS2X_FILE_LOG=1 logs file lookups (EA FILESYS and BXFILE_exists) with results.
    std::string readGuestString(uint8_t *rdram, uint32_t addr)
    {
        std::string out;
        for (; addr != 0u && out.size() < 128u; ++addr)
        {
            const uint8_t *p = getConstMemPtr(rdram, addr);
            if (!p || *p == 0)
                break;
            out.push_back(static_cast<char>(*p));
        }
        return out;
    }

    PS2Runtime::RecompiledFunction g_fileInfoOriginal = nullptr;
    PS2Runtime::RecompiledFunction g_bxExistsOriginal = nullptr;

    void ssx3LogFileInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const std::string name = readGuestString(rdram, getRegU32(ctx, 4));
        g_fileInfoOriginal(rdram, ctx, runtime);
        std::fprintf(stderr, "[file] fileinfo \"%s\" -> %d\n", name.c_str(), static_cast<int32_t>(getRegU32(ctx, 2)));
    }

    void ssx3LogBxExists(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const std::string name = readGuestString(rdram, getRegU32(ctx, 4));
        g_bxExistsOriginal(rdram, ctx, runtime);
        std::fprintf(stderr, "[file] exists \"%s\" -> %d\n", name.c_str(), static_cast<int32_t>(getRegU32(ctx, 2)));
    }

    void applySsx3(PS2Runtime &runtime)
    {
        if (std::getenv("PS2X_FILE_LOG"))
        {
            g_fileInfoOriginal = runtime.lookupFunction(0x003DE4D0u);
            if (g_fileInfoOriginal)
                runtime.replaceFunction(0x003DE4D0u, ssx3LogFileInfo);
            g_bxExistsOriginal = runtime.lookupFunction(0x00317F38u);
            if (g_bxExistsOriginal)
                runtime.replaceFunction(0x00317F38u, ssx3LogBxExists);
        }
        runtime.replaceFunction(0x004261F0u, ssx3iSifSendCmd);
        runtime.replaceFunction(0x003F61D0u, ssx3UsbKbStart);
        runtime.replaceFunction(0x0042B6D0u, ssx3SifSearchModuleByName);
        g_loadIrxOriginal = runtime.lookupFunction(0x00319718u);
        if (g_loadIrxOriginal)
            runtime.replaceFunction(0x00319718u, ssx3LoadIrx);
    }
}

PS2_REGISTER_GAME_OVERRIDE("SSX 3 (USA)", "SLUS_207.72", 0x00100008u, 0u, applySsx3)
