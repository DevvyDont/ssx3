#include "common.h"

// Sony libkernl syscall stubs (credit: peak3project's fork, github.com/peak3project/ssx3). Each is
// `li $v1, N; syscall; jr $ra; nop`: the kernel takes the call number in $v1 ($3), the arguments are already
// in $a0-$a3 and the result comes back in $v0, so a stub is only the trap. Sony wrote them as an assembler
// macro; here the number is bound to $3 because that register is the kernel ABI (tools/gen/phaseb_import.py
// ABI_OK). The C prototypes are left as void(void): the callers declare the real signatures.
// PORT: PS2 kernel syscall stubs; a PC port replaces this whole unit.
#define SYSCALL_STUB(name, n) \
    void name(void) { register int v __asm__("$3") = (n); __asm__ volatile("syscall" : : "r"(v)); }

//100%
INCLUDE_ASM("seg/seg_3249A0", SetGsCrt);
#ifdef SKIP_ASM
SYSCALL_STUB(SetGsCrt, 0x2)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_004239B0);
#ifdef SKIP_ASM
SYSCALL_STUB(func_004239B0, 0x3)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", KExit);
#ifdef SKIP_ASM
SYSCALL_STUB(KExit, 0x4)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_004239D0);
#ifdef SKIP_ASM
SYSCALL_STUB(func_004239D0, 0x5)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _LoadExecPS2);
#ifdef SKIP_ASM
SYSCALL_STUB(_LoadExecPS2, 0x6)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _ExecPS2);
#ifdef SKIP_ASM
SYSCALL_STUB(_ExecPS2, 0x7)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423A00);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423A00, 0x8)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423A10);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423A10, 0x9)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", AddSbusIntcHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(AddSbusIntcHandler, 0xA)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", RemoveSbusIntcHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(RemoveSbusIntcHandler, 0xB)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", Interrupt2Iop);
#ifdef SKIP_ASM
SYSCALL_STUB(Interrupt2Iop, 0xC)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetVTLBRefillHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(SetVTLBRefillHandler, 0xD)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetVCommonHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(SetVCommonHandler, 0xE)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetVInterruptHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(SetVInterruptHandler, 0xF)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423A80);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423A80, 0x10)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423A90);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423A90, 0x10)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", RemoveIntcHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(RemoveIntcHandler, 0x11)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423AB0);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423AB0, 0x12)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423AC0);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423AC0, 0x12)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", RemoveDmacHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(RemoveDmacHandler, 0x13)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _EnableIntc);
#ifdef SKIP_ASM
SYSCALL_STUB(_EnableIntc, 0x14)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _DisableIntc);
#ifdef SKIP_ASM
SYSCALL_STUB(_DisableIntc, 0x15)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _EnableDmac);
#ifdef SKIP_ASM
SYSCALL_STUB(_EnableDmac, 0x16)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _DisableDmac);
#ifdef SKIP_ASM
SYSCALL_STUB(_DisableDmac, 0x17)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetAlarm);
#ifdef SKIP_ASM
SYSCALL_STUB(SetAlarm, 0xFC)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423B30);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423B30, 0xFD)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iEnableIntc);
#ifdef SKIP_ASM
SYSCALL_STUB(_iEnableIntc, -0x1A)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iDisableIntc);
#ifdef SKIP_ASM
SYSCALL_STUB(_iDisableIntc, -0x1B)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iEnableDmac);
#ifdef SKIP_ASM
SYSCALL_STUB(_iEnableDmac, -0x1C)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iDisableDmac);
#ifdef SKIP_ASM
SYSCALL_STUB(_iDisableDmac, -0x1D)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423B80);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423B80, -0xFE)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iReleaseAlarm);
#ifdef SKIP_ASM
SYSCALL_STUB(iReleaseAlarm, -0xFF)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", CreateThread);
#ifdef SKIP_ASM
SYSCALL_STUB(CreateThread, 0x20)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", DeleteThread);
#ifdef SKIP_ASM
SYSCALL_STUB(DeleteThread, 0x21)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", StartThread);
#ifdef SKIP_ASM
SYSCALL_STUB(StartThread, 0x22)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ExitThread);
#ifdef SKIP_ASM
SYSCALL_STUB(ExitThread, 0x23)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ExitDeleteThread);
#ifdef SKIP_ASM
SYSCALL_STUB(ExitDeleteThread, 0x24)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", TerminateThread);
#ifdef SKIP_ASM
SYSCALL_STUB(TerminateThread, 0x25)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iTerminateThread);
#ifdef SKIP_ASM
SYSCALL_STUB(iTerminateThread, -0x26)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", DisableDispatchThread);
#ifdef SKIP_ASM
SYSCALL_STUB(DisableDispatchThread, 0x27)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", EnableDispatchThread);
#ifdef SKIP_ASM
SYSCALL_STUB(EnableDispatchThread, 0x28)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ChangeThreadPriority);
#ifdef SKIP_ASM
SYSCALL_STUB(ChangeThreadPriority, 0x29)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iChangeThreadPriority);
#ifdef SKIP_ASM
SYSCALL_STUB(iChangeThreadPriority, -0x2A)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", RotateThreadReadyQueue);
#ifdef SKIP_ASM
SYSCALL_STUB(RotateThreadReadyQueue, 0x2B)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iRotateThreadReadyQueue);
#ifdef SKIP_ASM
SYSCALL_STUB(_iRotateThreadReadyQueue, -0x2C)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ReleaseWaitThread);
#ifdef SKIP_ASM
SYSCALL_STUB(ReleaseWaitThread, 0x2D)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iReleaseWaitThread);
#ifdef SKIP_ASM
SYSCALL_STUB(iReleaseWaitThread, -0x2E)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GetThreadId);
#ifdef SKIP_ASM
SYSCALL_STUB(GetThreadId, 0x2F)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ReferThreadStatus);
#ifdef SKIP_ASM
SYSCALL_STUB(ReferThreadStatus, 0x30)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iReferThreadStatus);
#ifdef SKIP_ASM
SYSCALL_STUB(iReferThreadStatus, -0x31)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SleepThread);
#ifdef SKIP_ASM
SYSCALL_STUB(SleepThread, 0x32)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", WakeupThread);
#ifdef SKIP_ASM
SYSCALL_STUB(WakeupThread, 0x33)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iWakeupThread);
#ifdef SKIP_ASM
SYSCALL_STUB(_iWakeupThread, -0x34)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", CancelWakeupThread);
#ifdef SKIP_ASM
SYSCALL_STUB(CancelWakeupThread, 0x35)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iCancelWakeupThread);
#ifdef SKIP_ASM
SYSCALL_STUB(iCancelWakeupThread, -0x36)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SuspendThread);
#ifdef SKIP_ASM
SYSCALL_STUB(SuspendThread, 0x37)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iSuspendThread);
#ifdef SKIP_ASM
SYSCALL_STUB(_iSuspendThread, -0x38)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ResumeThread);
#ifdef SKIP_ASM
SYSCALL_STUB(ResumeThread, 0x39)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iResumeThread);
#ifdef SKIP_ASM
SYSCALL_STUB(iResumeThread, -0x3A)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423D50);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423D50, 0x3B)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423D60);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423D60, 0x3C)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423D70);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423D70, 0x3D)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", EndOfHeap);
#ifdef SKIP_ASM
SYSCALL_STUB(EndOfHeap, 0x3E)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423D90);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423D90, 0x3F)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", CreateSema);
#ifdef SKIP_ASM
SYSCALL_STUB(CreateSema, 0x40)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", DeleteSema);
#ifdef SKIP_ASM
SYSCALL_STUB(DeleteSema, 0x41)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SignalSema);
#ifdef SKIP_ASM
SYSCALL_STUB(SignalSema, 0x42)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iSignalSema);
#ifdef SKIP_ASM
SYSCALL_STUB(iSignalSema, -0x43)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", WaitSema);
#ifdef SKIP_ASM
SYSCALL_STUB(WaitSema, 0x44)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", PollSema);
#ifdef SKIP_ASM
SYSCALL_STUB(PollSema, 0x45)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iPollSema);
#ifdef SKIP_ASM
SYSCALL_STUB(iPollSema, -0x46)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ReferSemaStatus);
#ifdef SKIP_ASM
SYSCALL_STUB(ReferSemaStatus, 0x47)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iReferSemaStatus);
#ifdef SKIP_ASM
SYSCALL_STUB(iReferSemaStatus, -0x48)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423E30);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423E30, 0x49)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetOsdConfigParam);
#ifdef SKIP_ASM
SYSCALL_STUB(SetOsdConfigParam, 0x4A)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GetOsdConfigParam);
#ifdef SKIP_ASM
SYSCALL_STUB(GetOsdConfigParam, 0x4B)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GetGsHParam);
#ifdef SKIP_ASM
SYSCALL_STUB(GetGsHParam, 0x4C)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GetGsVParam);
#ifdef SKIP_ASM
SYSCALL_STUB(GetGsVParam, 0x4D)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetGsHParam);
#ifdef SKIP_ASM
SYSCALL_STUB(SetGsHParam, 0x4E)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetGsVParam);
#ifdef SKIP_ASM
SYSCALL_STUB(SetGsVParam, 0x4F)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", CreateEventFlag);
#ifdef SKIP_ASM
SYSCALL_STUB(CreateEventFlag, 0x50)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", DeleteEventFlag);
#ifdef SKIP_ASM
SYSCALL_STUB(DeleteEventFlag, 0x51)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetEventFlag);
#ifdef SKIP_ASM
SYSCALL_STUB(SetEventFlag, 0x52)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423ED0);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423ED0, -0x53)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", xlaunch);
#ifdef SKIP_ASM
SYSCALL_STUB(xlaunch, 0x54)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423EF0);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423EF0, -0x55)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423F00);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423F00, 0x56)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423F10);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423F10, 0x57)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423F20);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423F20, -0x58)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423F30);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423F30, 0x59)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423F40);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423F40, -0x5A)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423F50);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423F50, 0x5B)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", EnableIntcHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(EnableIntcHandler, 0x5C)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iEnableIntcHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(iEnableIntcHandler, -0x5C)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", DisableIntcHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(DisableIntcHandler, 0x5D)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iDisableIntcHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(iDisableIntcHandler, -0x5D)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", EnableDmacHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(EnableDmacHandler, 0x5E)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iEnableDmacHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(iEnableDmacHandler, -0x5E)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", DisableDmacHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(DisableDmacHandler, 0x5F)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iDisableDmacHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(iDisableDmacHandler, -0x5F)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", KSeg0);
#ifdef SKIP_ASM
SYSCALL_STUB(KSeg0, 0x60)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", EnableCache);
#ifdef SKIP_ASM
SYSCALL_STUB(EnableCache, 0x61)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", DisableCache);
#ifdef SKIP_ASM
SYSCALL_STUB(DisableCache, 0x62)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GetCop0);
#ifdef SKIP_ASM
SYSCALL_STUB(GetCop0, 0x63)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", FlushCache);
#ifdef SKIP_ASM
SYSCALL_STUB(FlushCache, 0x64)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", CpuConfig);
#ifdef SKIP_ASM
SYSCALL_STUB(CpuConfig, 0x66)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iGetCop0);
#ifdef SKIP_ASM
SYSCALL_STUB(iGetCop0, -0x67)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iFlushCache);
#ifdef SKIP_ASM
SYSCALL_STUB(iFlushCache, -0x68)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iCpuConfig);
#ifdef SKIP_ASM
SYSCALL_STUB(iCpuConfig, -0x6A)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", sceSifStopDma);
#ifdef SKIP_ASM
SYSCALL_STUB(sceSifStopDma, 0x6B)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetCPUTimerHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(SetCPUTimerHandler, 0x6C)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetCPUTimer);
#ifdef SKIP_ASM
SYSCALL_STUB(SetCPUTimer, 0x6D)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetOsdConfigParam2);
#ifdef SKIP_ASM
SYSCALL_STUB(SetOsdConfigParam2, 0x6E)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GetOsdConfigParam2);
#ifdef SKIP_ASM
SYSCALL_STUB(GetOsdConfigParam2, 0x6F)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GsGetIMR);
#ifdef SKIP_ASM
SYSCALL_STUB(GsGetIMR, 0x70)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iGsGetIMR);
#ifdef SKIP_ASM
SYSCALL_STUB(iGsGetIMR, -0x70)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GsPutIMR);
#ifdef SKIP_ASM
SYSCALL_STUB(GsPutIMR, 0x71)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iGsPutIMR);
#ifdef SKIP_ASM
SYSCALL_STUB(iGsPutIMR, -0x71)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetPgifHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(SetPgifHandler, 0x72)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetVSyncFlag);
#ifdef SKIP_ASM
SYSCALL_STUB(SetVSyncFlag, 0x73)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00424120);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00424120, 0x74)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _print);
#ifdef SKIP_ASM
SYSCALL_STUB(_print, 0x75)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", sceSifDmaStat);
#ifdef SKIP_ASM
SYSCALL_STUB(sceSifDmaStat, 0x76)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", isceSifDmaStat);
#ifdef SKIP_ASM
SYSCALL_STUB(isceSifDmaStat, -0x76)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", sceSifSetDma);
#ifdef SKIP_ASM
SYSCALL_STUB(sceSifSetDma, 0x77)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", isceSifSetDma);
#ifdef SKIP_ASM
SYSCALL_STUB(isceSifSetDma, -0x77)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", sceSifSetDChain);
#ifdef SKIP_ASM
SYSCALL_STUB(sceSifSetDChain, 0x78)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", isceSifSetDChain);
#ifdef SKIP_ASM
SYSCALL_STUB(isceSifSetDChain, -0x78)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", sceSifSetReg);
#ifdef SKIP_ASM
SYSCALL_STUB(sceSifSetReg, 0x79)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", sceSifGetReg);
#ifdef SKIP_ASM
SYSCALL_STUB(sceSifGetReg, 0x7A)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _ExecOSD);
#ifdef SKIP_ASM
SYSCALL_STUB(_ExecOSD, 0x7B)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", Deci2Call);
#ifdef SKIP_ASM
SYSCALL_STUB(Deci2Call, 0x7C)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", PSMode);
#ifdef SKIP_ASM
SYSCALL_STUB(PSMode, 0x7D)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", MachineType);
#ifdef SKIP_ASM
SYSCALL_STUB(MachineType, 0x7E)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GetMemorySize);
#ifdef SKIP_ASM
SYSCALL_STUB(GetMemorySize, 0x7F)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _InitTLB);
#ifdef SKIP_ASM
SYSCALL_STUB(_InitTLB, 0x82)
#endif
