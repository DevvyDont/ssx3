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
SYSCALL_STUB(AddSbusIntcHandler, 10)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", RemoveSbusIntcHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(RemoveSbusIntcHandler, 11)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", Interrupt2Iop);
#ifdef SKIP_ASM
SYSCALL_STUB(Interrupt2Iop, 0xC)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetVTLBRefillHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(SetVTLBRefillHandler, 13)
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
SYSCALL_STUB(RemoveIntcHandler, 17)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423AB0);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423AB0, 18)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423AC0);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423AC0, 18)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", RemoveDmacHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(RemoveDmacHandler, 19)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _EnableIntc);
#ifdef SKIP_ASM
SYSCALL_STUB(_EnableIntc, 20)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _DisableIntc);
#ifdef SKIP_ASM
SYSCALL_STUB(_DisableIntc, 21)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _EnableDmac);
#ifdef SKIP_ASM
SYSCALL_STUB(_EnableDmac, 22)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _DisableDmac);
#ifdef SKIP_ASM
SYSCALL_STUB(_DisableDmac, 23)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetAlarm);
#ifdef SKIP_ASM
SYSCALL_STUB(SetAlarm, 0xFC)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423B30);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423B30, 253)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iEnableIntc);
#ifdef SKIP_ASM
SYSCALL_STUB(_iEnableIntc, -26)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iDisableIntc);
#ifdef SKIP_ASM
SYSCALL_STUB(_iDisableIntc, -27)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iEnableDmac);
#ifdef SKIP_ASM
SYSCALL_STUB(_iEnableDmac, -0x1C)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iDisableDmac);
#ifdef SKIP_ASM
SYSCALL_STUB(_iDisableDmac, -29)
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
SYSCALL_STUB(DeleteThread, 33)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", StartThread);
#ifdef SKIP_ASM
SYSCALL_STUB(StartThread, 34)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ExitThread);
#ifdef SKIP_ASM
SYSCALL_STUB(ExitThread, 35)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ExitDeleteThread);
#ifdef SKIP_ASM
SYSCALL_STUB(ExitDeleteThread, 36)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", TerminateThread);
#ifdef SKIP_ASM
SYSCALL_STUB(TerminateThread, 37)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iTerminateThread);
#ifdef SKIP_ASM
SYSCALL_STUB(iTerminateThread, -38)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", DisableDispatchThread);
#ifdef SKIP_ASM
SYSCALL_STUB(DisableDispatchThread, 39)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", EnableDispatchThread);
#ifdef SKIP_ASM
SYSCALL_STUB(EnableDispatchThread, 40)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ChangeThreadPriority);
#ifdef SKIP_ASM
SYSCALL_STUB(ChangeThreadPriority, 41)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iChangeThreadPriority);
#ifdef SKIP_ASM
SYSCALL_STUB(iChangeThreadPriority, -42)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", RotateThreadReadyQueue);
#ifdef SKIP_ASM
SYSCALL_STUB(RotateThreadReadyQueue, 43)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iRotateThreadReadyQueue);
#ifdef SKIP_ASM
SYSCALL_STUB(_iRotateThreadReadyQueue, -44)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ReleaseWaitThread);
#ifdef SKIP_ASM
SYSCALL_STUB(ReleaseWaitThread, 45)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iReleaseWaitThread);
#ifdef SKIP_ASM
SYSCALL_STUB(iReleaseWaitThread, -46)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GetThreadId);
#ifdef SKIP_ASM
SYSCALL_STUB(GetThreadId, 47)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ReferThreadStatus);
#ifdef SKIP_ASM
SYSCALL_STUB(ReferThreadStatus, 0x30)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iReferThreadStatus);
#ifdef SKIP_ASM
SYSCALL_STUB(iReferThreadStatus, -49)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SleepThread);
#ifdef SKIP_ASM
SYSCALL_STUB(SleepThread, 50)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", WakeupThread);
#ifdef SKIP_ASM
SYSCALL_STUB(WakeupThread, 51)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iWakeupThread);
#ifdef SKIP_ASM
SYSCALL_STUB(_iWakeupThread, -52)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", CancelWakeupThread);
#ifdef SKIP_ASM
SYSCALL_STUB(CancelWakeupThread, 53)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iCancelWakeupThread);
#ifdef SKIP_ASM
SYSCALL_STUB(iCancelWakeupThread, -54)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SuspendThread);
#ifdef SKIP_ASM
SYSCALL_STUB(SuspendThread, 55)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _iSuspendThread);
#ifdef SKIP_ASM
SYSCALL_STUB(_iSuspendThread, -0x38)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ResumeThread);
#ifdef SKIP_ASM
SYSCALL_STUB(ResumeThread, 57)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iResumeThread);
#ifdef SKIP_ASM
SYSCALL_STUB(iResumeThread, -58)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423D50);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423D50, 59)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423D60);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423D60, 0x3C)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423D70);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423D70, 61)
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
SYSCALL_STUB(DeleteSema, 65)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SignalSema);
#ifdef SKIP_ASM
SYSCALL_STUB(SignalSema, 66)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iSignalSema);
#ifdef SKIP_ASM
SYSCALL_STUB(iSignalSema, -67)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", WaitSema);
#ifdef SKIP_ASM
SYSCALL_STUB(WaitSema, 68)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", PollSema);
#ifdef SKIP_ASM
SYSCALL_STUB(PollSema, 69)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iPollSema);
#ifdef SKIP_ASM
SYSCALL_STUB(iPollSema, -70)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", ReferSemaStatus);
#ifdef SKIP_ASM
SYSCALL_STUB(ReferSemaStatus, 71)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iReferSemaStatus);
#ifdef SKIP_ASM
SYSCALL_STUB(iReferSemaStatus, -72)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423E30);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423E30, 73)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetOsdConfigParam);
#ifdef SKIP_ASM
SYSCALL_STUB(SetOsdConfigParam, 74)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GetOsdConfigParam);
#ifdef SKIP_ASM
SYSCALL_STUB(GetOsdConfigParam, 75)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GetGsHParam);
#ifdef SKIP_ASM
SYSCALL_STUB(GetGsHParam, 76)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GetGsVParam);
#ifdef SKIP_ASM
SYSCALL_STUB(GetGsVParam, 77)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetGsHParam);
#ifdef SKIP_ASM
SYSCALL_STUB(SetGsHParam, 78)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetGsVParam);
#ifdef SKIP_ASM
SYSCALL_STUB(SetGsVParam, 79)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", CreateEventFlag);
#ifdef SKIP_ASM
SYSCALL_STUB(CreateEventFlag, 80)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", DeleteEventFlag);
#ifdef SKIP_ASM
SYSCALL_STUB(DeleteEventFlag, 81)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetEventFlag);
#ifdef SKIP_ASM
SYSCALL_STUB(SetEventFlag, 82)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423ED0);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423ED0, -83)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", xlaunch);
#ifdef SKIP_ASM
SYSCALL_STUB(xlaunch, 84)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423EF0);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423EF0, -85)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423F00);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423F00, 86)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423F10);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423F10, 87)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423F20);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423F20, -88)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423F30);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423F30, 89)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423F40);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423F40, -90)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00423F50);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00423F50, 91)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", EnableIntcHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(EnableIntcHandler, 92)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iEnableIntcHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(iEnableIntcHandler, -92)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", DisableIntcHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(DisableIntcHandler, 93)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iDisableIntcHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(iDisableIntcHandler, -93)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", EnableDmacHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(EnableDmacHandler, 94)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iEnableDmacHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(iEnableDmacHandler, -94)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", DisableDmacHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(DisableDmacHandler, 95)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iDisableDmacHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(iDisableDmacHandler, -95)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", KSeg0);
#ifdef SKIP_ASM
SYSCALL_STUB(KSeg0, 0x60)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", EnableCache);
#ifdef SKIP_ASM
SYSCALL_STUB(EnableCache, 97)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", DisableCache);
#ifdef SKIP_ASM
SYSCALL_STUB(DisableCache, 98)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GetCop0);
#ifdef SKIP_ASM
SYSCALL_STUB(GetCop0, 99)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", FlushCache);
#ifdef SKIP_ASM
SYSCALL_STUB(FlushCache, 100)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", CpuConfig);
#ifdef SKIP_ASM
SYSCALL_STUB(CpuConfig, 102)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iGetCop0);
#ifdef SKIP_ASM
SYSCALL_STUB(iGetCop0, -103)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iFlushCache);
#ifdef SKIP_ASM
SYSCALL_STUB(iFlushCache, -104)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iCpuConfig);
#ifdef SKIP_ASM
SYSCALL_STUB(iCpuConfig, -106)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", sceSifStopDma);
#ifdef SKIP_ASM
SYSCALL_STUB(sceSifStopDma, 107)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetCPUTimerHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(SetCPUTimerHandler, 108)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetCPUTimer);
#ifdef SKIP_ASM
SYSCALL_STUB(SetCPUTimer, 109)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetOsdConfigParam2);
#ifdef SKIP_ASM
SYSCALL_STUB(SetOsdConfigParam2, 110)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", GetOsdConfigParam2);
#ifdef SKIP_ASM
SYSCALL_STUB(GetOsdConfigParam2, 111)
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
SYSCALL_STUB(GsPutIMR, 113)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", iGsPutIMR);
#ifdef SKIP_ASM
SYSCALL_STUB(iGsPutIMR, -113)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetPgifHandler);
#ifdef SKIP_ASM
SYSCALL_STUB(SetPgifHandler, 114)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", SetVSyncFlag);
#ifdef SKIP_ASM
SYSCALL_STUB(SetVSyncFlag, 115)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", func_00424120);
#ifdef SKIP_ASM
SYSCALL_STUB(func_00424120, 116)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _print);
#ifdef SKIP_ASM
SYSCALL_STUB(_print, 117)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", sceSifDmaStat);
#ifdef SKIP_ASM
SYSCALL_STUB(sceSifDmaStat, 118)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", isceSifDmaStat);
#ifdef SKIP_ASM
SYSCALL_STUB(isceSifDmaStat, -118)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", sceSifSetDma);
#ifdef SKIP_ASM
SYSCALL_STUB(sceSifSetDma, 119)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", isceSifSetDma);
#ifdef SKIP_ASM
SYSCALL_STUB(isceSifSetDma, -119)
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
SYSCALL_STUB(sceSifSetReg, 121)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", sceSifGetReg);
#ifdef SKIP_ASM
SYSCALL_STUB(sceSifGetReg, 122)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", _ExecOSD);
#ifdef SKIP_ASM
SYSCALL_STUB(_ExecOSD, 123)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", Deci2Call);
#ifdef SKIP_ASM
SYSCALL_STUB(Deci2Call, 0x7C)
#endif

//100%
INCLUDE_ASM("seg/seg_3249A0", PSMode);
#ifdef SKIP_ASM
SYSCALL_STUB(PSMode, 125)
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
SYSCALL_STUB(_InitTLB, 130)
#endif
