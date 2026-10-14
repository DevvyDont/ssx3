# Function matching report

Auto-generated from [objdiff](tools/objdiff) by diffing every unit's `obj/target` against `obj/current`. Regenerate with a dual build (`./configure.py -c -o && ninja`) followed by `python3 scripts/matching_report.py`; see [CONTRIBUTING.md](CONTRIBUTING.md).

Snapshot as of 2026-10-14.

**Status definitions:**
- **Matching** — byte-for-byte identical to the retail binary (100%), or a near-miss where objdiff's own report considers the function matched despite a lower raw percentage (this happens for a handful of functions where the only remaining byte difference is a relocation immediate baked in at link time by the original SN toolchain, not a real code difference — see the gp-relative note in [README.md](README.md)).
- **Partial** — has a C implementation (not raw `INCLUDE_ASM`) but doesn't yet byte-match; the percentage is objdiff's fuzzy match score.
- **Not started** — still a raw `INCLUDE_ASM` stub with no C implementation attempted.

| | Count | % of total |
|---|---|---|
| Matching | 81 | 0.16% |
| Partial | 10 | 0.02% |
| Not started | 52,042 | 99.83% |
| **Total** | **52,133** | |

(Total here may differ slightly from the function count in README.md's progress table — that one comes from objdiff's own aggregate report, this one from summing every unit's individual symbol list, and the two count a handful of duplicated/weak symbols differently.)

## Per-file summary

Only files with at least one Matching or Partial function are listed here; files that are 100% Not Started are omitted from this table (see the full per-file breakdown further down for every file, including those).

| File | Matching | Partial | Not started | Total |
|---|---|---|---|---|
| `bx/bxstring` | 39 | 5 | 0 | 44 |
| `bxrandom` | 10 | 0 | 0 | 10 |
| `be/beintcharacter` | 9 | 0 | 11 | 20 |
| `hashvalue` | 6 | 0 | 0 | 6 |
| `main/sectionman` | 6 | 0 | 6 | 12 |
| `visualfx/crowdrender2d` | 1 | 5 | 7 | 13 |
| `fe/festatetransition` | 4 | 0 | 1 | 5 |
| `md5` | 4 | 0 | 0 | 4 |
| `data/vutext` | 1 | 0 | 0 | 1 |
| `dirtysock/tags` | 1 | 0 | 39 | 40 |

## Full per-file breakdown

Every unit, including ones with no progress yet. Matching and Partial functions are listed individually with their match percentage; Not Started functions are only counted (there's nothing to report per-function until an implementation is attempted).

### `101218`

0 matching, 0 partial, 4 not started (4 total)

### `1121A0`

0 matching, 0 partial, 183 not started (183 total)

### `1218`

0 matching, 0 partial, 168 not started (168 total)

### `13D808`

0 matching, 0 partial, 241 not started (241 total)

### `14B10`

0 matching, 0 partial, 68 not started (68 total)

### `152718`

0 matching, 0 partial, 595 not started (595 total)

### `1763B8`

0 matching, 0 partial, 8 not started (8 total)

### `182F30`

0 matching, 0 partial, 167 not started (167 total)

### `1BA100`

0 matching, 0 partial, 300 not started (300 total)

### `1C9348`

0 matching, 0 partial, 18 not started (18 total)

### `1D3510`

0 matching, 0 partial, 113 not started (113 total)

### `1DCD10`

0 matching, 0 partial, 7 not started (7 total)

### `1F1548`

0 matching, 0 partial, 56 not started (56 total)

### `1FBE38`

0 matching, 0 partial, 169 not started (169 total)

### `2125C8`

0 matching, 0 partial, 6 not started (6 total)

### `215CB0`

0 matching, 0 partial, 10 not started (10 total)

### `217898`

0 matching, 0 partial, 30 not started (30 total)

### `218AE8`

0 matching, 0 partial, 18 not started (18 total)

### `21A1C0`

0 matching, 0 partial, 61 not started (61 total)

### `21E5A8`

0 matching, 0 partial, 3 not started (3 total)

### `28BE10`

0 matching, 0 partial, 4 not started (4 total)

### `298140`

0 matching, 0 partial, 27 not started (27 total)

### `29F8B8`

0 matching, 0 partial, 17 not started (17 total)

### `2A15E8`

0 matching, 0 partial, 38 not started (38 total)

### `2A7D50`

0 matching, 0 partial, 77 not started (77 total)

### `2AD800`

0 matching, 0 partial, 1342 not started (1342 total)

### `2EEF50`

0 matching, 0 partial, 1655 not started (1655 total)

### `4EC80`

0 matching, 0 partial, 12 not started (12 total)

### `53398`

0 matching, 0 partial, 5 not started (5 total)

### `54088`

0 matching, 0 partial, 18 not started (18 total)

### `57338`

0 matching, 0 partial, 41 not started (41 total)

### `6CF20`

0 matching, 0 partial, 28 not started (28 total)

### `7BCC8`

0 matching, 0 partial, 19 not started (19 total)

### `84DE0`

0 matching, 0 partial, 9 not started (9 total)

### `87B50`

0 matching, 0 partial, 21 not started (21 total)

### `923B8`

0 matching, 0 partial, 59 not started (59 total)

### `966F8`

0 matching, 0 partial, 5 not started (5 total)

### `A1648`

0 matching, 0 partial, 4 not started (4 total)

### `A4090`

0 matching, 0 partial, 16 not started (16 total)

### `CF758`

0 matching, 0 partial, 9 not started (9 total)

### `D76B0`

0 matching, 0 partial, 386 not started (386 total)

### `ai/ai`

0 matching, 0 partial, 269 not started (269 total)

### `ai/airpredictor`

0 matching, 0 partial, 10 not started (10 total)

### `ai/computer`

0 matching, 0 partial, 160 not started (160 total)

### `ai/control/handplantcontrol`

0 matching, 0 partial, 233 not started (233 total)

### `ai/motion/wipeoutmotion`

0 matching, 0 partial, 16 not started (16 total)

### `ai/rider`

0 matching, 0 partial, 168 not started (168 total)

### `ai/ridermetrix`

0 matching, 0 partial, 73 not started (73 total)

### `animation/animmodel`

0 matching, 0 partial, 34 not started (34 total)

### `animation/animsequencer`

0 matching, 0 partial, 44 not started (44 total)

### `animation/gameanimmap`

0 matching, 0 partial, 18 not started (18 total)

### `animation/rideranimbase`

0 matching, 0 partial, 60 not started (60 total)

### `be/bedbreward`

0 matching, 0 partial, 146 not started (146 total)

### `be/beintaggression`

0 matching, 0 partial, 10 not started (10 total)

### `be/beintbagt`

0 matching, 0 partial, 31 not started (31 total)

### `be/beintcharacter`

9 matching, 0 partial, 11 not started (20 total)

| Function | Status | Match % | Size |
|---|---|---|---|
| `cBECharacterInterface::getWeight(int)` | Matching | 100.00% | 60 |
| `cBECharacterInterface::isGoofy(int)` | Matching | 100.00% | 56 |
| `cBECharacterInterface::func_0014F100` | Matching | 100.00% | 8 |
| `cBECharacterInterface::getBloodType(int)` | Matching | 100.00% | 48 |
| `cBECharacterInterface::isFemale(int)` | Matching | 100.00% | 56 |
| `cBECharacterInterface::getAge(int)` | Matching | 100.00% | 60 |
| `cBECharacterInterface::getHeight(int)` | Matching | 100.00% | 48 |
| `cBECharacterInterface::getNationality(int)` | Matching | 100.00% | 48 |
| `cBECharacterInterface::func_0014F210(int)` | Matching | 100.00% | 60 |

### `be/beintecon`

0 matching, 0 partial, 53 not started (53 total)

### `be/beintmission`

0 matching, 0 partial, 50 not started (50 total)

### `be/beintnetwork`

0 matching, 0 partial, 19 not started (19 total)

### `be/beintnewrace`

0 matching, 0 partial, 59 not started (59 total)

### `be/beintoption`

0 matching, 0 partial, 17 not started (17 total)

### `be/beintplayer`

0 matching, 0 partial, 123 not started (123 total)

### `be/beintreward`

0 matching, 0 partial, 173 not started (173 total)

### `be/beintsavefile`

0 matching, 0 partial, 12 not started (12 total)

### `be/beintscore`

0 matching, 0 partial, 34 not started (34 total)

### `be/beintstat`

0 matching, 0 partial, 15 not started (15 total)

### `be/belibrary`

0 matching, 0 partial, 90 not started (90 total)

### `bx/bxstring`

39 matching, 5 partial, 0 not started (44 total)

| Function | Status | Match % | Size |
|---|---|---|---|
| `cBXString::cBXString(cBXString const &)` | Matching | 100.00% | 88 |
| `cBXString::Release` | Matching | 100.00% | 100 |
| `cBXString::AllocBeforeWrite(int)` | Matching | 100.00% | 96 |
| `cBXString::AllocCopy(cBXString &, int, int, int) const` | Matching | 100.00% | 120 |
| `cBXString::~cBXString(void)` | Matching | 100.00% | 120 |
| `cBXString::cBXString(char const *)` | Matching | 100.00% | 120 |
| `cBXString::cBXString(char const *, unsigned int)` | Matching | 100.00% | 132 |
| `cBXString::AssignCopy(int, char const *)` | Matching | 100.00% | 108 |
| `cBXString::operator=(cBXString const &)` | Matching | 100.00% | 156 |
| `cBXString::operator=(char const *)` | Matching | 100.00% | 88 |
| `cBXString::ConcatCopy(int, char const *, int, char const *)` | Matching | 100.00% | 132 |
| `operator+(cBXString const &, cBXString const &)` | Matching | 100.00% | 100 |
| `operator+(const char*, const cBXString&)` | Matching | 100.00% | 140 |
| `operator+(const cBXString&, char)` | Matching | 100.00% | 100 |
| `operator+(char, const cBXString&)` | Matching | 100.00% | 100 |
| `cBXString::operator+=(char const *)` | Matching | 100.00% | 88 |
| `cBXString::operator+=(char)` | Matching | 100.00% | 52 |
| `cBXString::operator+=(cBXString const &)` | Matching | 100.00% | 48 |
| `cBXString::GetBuffer(int)` | Matching | 100.00% | 184 |
| `cBXString::ReleaseBuffer(int)` | Matching | 100.00% | 108 |
| `cBXString::GetBufferSetLength(int)` | Matching | 100.00% | 72 |
| `cBXString::LockBuffer` | Matching | 100.00% | 52 |
| `cBXString::UnlockBuffer` | Matching | 100.00% | 32 |
| `cBXString::Find(char, int) const` | Matching | 100.00% | 72 |
| `cBXString::FindOneOf(char const *) const` | Matching | 100.00% | 60 |
| `cBXString::ReverseFind(char) const` | Matching | 100.00% | 68 |
| `cBXString::Find(char const *, int) const` | Matching | 100.00% | 64 |
| `cBXString::MakeUpper` | Matching | 100.00% | 104 |
| `cBXString::MakeLower` | Matching | 100.00% | 104 |
| `cBXString::Replace(char, char)` | Matching | 100.00% | 112 |
| `cBXString::SetAt(int, char)` | Matching | 100.00% | 76 |
| `cBXString::cBXString(char, int)` | Matching | 100.00% | 100 |
| `cBXString::operator=(char)` | Matching | 100.00% | 52 |
| `cBXString::Mid(int) const` | Matching | 100.00% | 52 |
| `cBXString::Mid(int, int) const` | Matching | 100.00% | 148 |
| `cBXString::Right(int) const` | Matching | 100.00% | 124 |
| `cBXString::Left(int) const` | Matching | 100.00% | 116 |
| `cBXString::SpanIncluding(char const *) const` | Matching | 100.00% | 76 |
| `cBXString::SpanExcluding(char const *) const` | Matching | 100.00% | 76 |
| `operator+(const cBXString&, const char*)` | Partial | 97.42% | 160 |
| `cBXString::FreeExtra` | Partial | 93.69% | 128 |
| `cBXString::AllocBuffer(int)` | Partial | 85.57% | 120 |
| `cBXString::ConcatInPlace(int, char const *)` | Partial | 83.30% | 224 |
| `cBXString::CopyBeforeWrite` | Partial | 75.04% | 104 |

### `bx/bxstringctor`

0 matching, 0 partial, 99 not started (99 total)

### `bx/cubicspline`

0 matching, 0 partial, 76 not started (76 total)

### `bx/execman`

0 matching, 0 partial, 31 not started (31 total)

### `bx/ps2main`

0 matching, 0 partial, 18 not started (18 total)

### `bxrandom`

10 matching, 0 partial, 0 not started (10 total)

| Function | Status | Match % | Size |
|---|---|---|---|
| `BXsrand(unsigned int)` | Matching | 100.00% | 36 |
| `BXrand()` | Matching | 100.00% | 32 |
| `AIrand()` | Matching | 100.00% | 32 |
| `AIrandf(float, float)` | Matching | 100.00% | 96 |
| `func_00317890(float, float)` | Matching | 100.00% | 76 |
| `func_003178E0(unsigned int*)` | Matching | 100.00% | 36 |
| `func_00317908(unsigned int*)` | Matching | 100.00% | 40 |
| `func_00317930(unsigned int*)` | Matching | 100.00% | 36 |
| `cBxPseudoRng_Seed(unsigned int*, unsigned int)` | Matching | 100.00% | 172 |
| `cBxPseudoRng_NextInt(unsigned int*)` | Matching | 100.00% | 224 |

### `camera/camera`

0 matching, 0 partial, 238 not started (238 total)

### `camera/cameraalgolist`

0 matching, 0 partial, 3 not started (3 total)

### `camera/cameraalgorithms`

0 matching, 0 partial, 38 not started (38 total)

### `camera/cameracontroller`

0 matching, 0 partial, 15 not started (15 total)

### `camera/script/scriptcontroller`

0 matching, 0 partial, 41 not started (41 total)

### `camera/trigger/cameratriggerfactory`

0 matching, 0 partial, 150 not started (150 total)

### `camera/trigger/cameratriggerman`

0 matching, 0 partial, 96 not started (96 total)

### `data/33DE00.data`

0 matching, 0 partial, 688 not started (688 total)

### `data/357900.rodata`

0 matching, 0 partial, 6604 not started (6604 total)

### `data/39C100.lit4`

0 matching, 0 partial, 26486 not started (26486 total)

### `data/vutext`

1 matching, 0 partial, 0 not started (1 total)

| Function | Status | Match % | Size |
|---|---|---|---|
| `D_0042E590` | Matching | 100.00% | 59348 |

### `dirtysock/tags`

1 matching, 0 partial, 39 not started (40 total)

| Function | Status | Match % | Size |
|---|---|---|---|
| `cDirtysock_tag__TagFieldSetupAppend(char *, char *, char *)` | Matching | 100.00% | 72 |

### `fe/debugfe`

0 matching, 0 partial, 156 not started (156 total)

### `fe/feasyncfile`

0 matching, 0 partial, 818 not started (818 total)

### `fe/fepopup`

0 matching, 0 partial, 190 not started (190 total)

### `fe/fepopupmisc`

0 matching, 0 partial, 435 not started (435 total)

### `fe/fereal`

0 matching, 0 partial, 112 not started (112 total)

### `fe/feridermanager`

0 matching, 0 partial, 61 not started (61 total)

### `fe/festateaudiooptions`

0 matching, 0 partial, 13 not started (13 total)

### `fe/festatebigradio`

0 matching, 0 partial, 56 not started (56 total)

### `fe/festatebonusmaterial`

0 matching, 0 partial, 5 not started (5 total)

### `fe/festatebuyattrib`

0 matching, 0 partial, 31 not started (31 total)

### `fe/festatecharequipdetail`

0 matching, 0 partial, 102 not started (102 total)

### `fe/festatecharselect`

0 matching, 0 partial, 25 not started (25 total)

### `fe/festatecharsetup`

0 matching, 0 partial, 9 not started (9 total)

### `fe/festatecredits`

0 matching, 0 partial, 10 not started (10 total)

### `fe/festatedebug`

0 matching, 0 partial, 3 not started (3 total)

### `fe/festatelegal`

0 matching, 0 partial, 4 not started (4 total)

### `fe/festateload`

0 matching, 0 partial, 13 not started (13 total)

### `fe/festateloadhint`

0 matching, 0 partial, 48 not started (48 total)

### `fe/festateloadscreen`

0 matching, 0 partial, 34 not started (34 total)

### `fe/festatelodge`

0 matching, 0 partial, 13 not started (13 total)

### `fe/festateoptions`

0 matching, 0 partial, 59 not started (59 total)

### `fe/festateprofile`

0 matching, 0 partial, 26 not started (26 total)

### `fe/festaterewards`

0 matching, 0 partial, 94 not started (94 total)

### `fe/festateriderbio`

0 matching, 0 partial, 11 not started (11 total)

### `fe/festateriderbiodetail`

0 matching, 0 partial, 9 not started (9 total)

### `fe/festatestore`

0 matching, 0 partial, 20 not started (20 total)

### `fe/festatetransition`

4 matching, 0 partial, 1 not started (5 total)

| Function | Status | Match % | Size |
|---|---|---|---|
| `cFEStateBackground::onCreateScreen` | Matching | 100.00% | 96 |
| `cFEStateTransition::onCreateScreen` | Matching | 100.00% | 120 |
| `cFEStateTransition::onScreenEvent(cUIScreen *, unsigned int)` | Matching | 100.00% | 168 |
| `cFEStateTransition::func_001946A8(int)` | Matching | 100.00% | 140 |

### `fe/festatetrophyroom`

0 matching, 0 partial, 36 not started (36 total)

### `fe/messagecenter`

0 matching, 0 partial, 23 not started (23 total)

### `fe/ovstateenterlodge`

0 matching, 0 partial, 27 not started (27 total)

### `fe/ovstatehud`

0 matching, 0 partial, 84 not started (84 total)

### `fe/ovstatelodge`

0 matching, 0 partial, 22 not started (22 total)

### `fe/ovstatemap`

0 matching, 0 partial, 30 not started (30 total)

### `fe/ovstatepause`

0 matching, 0 partial, 79 not started (79 total)

### `fe/ovstatereplay`

0 matching, 0 partial, 17 not started (17 total)

### `fe/ovstates`

0 matching, 0 partial, 22 not started (22 total)

### `fe/ovstatetrophy`

0 matching, 0 partial, 24 not started (24 total)

### `fe/ovtemplatedialog`

0 matching, 0 partial, 56 not started (56 total)

### `fe/ovtemplatepausemenu`

0 matching, 0 partial, 33 not started (33 total)

### `fe/uitemplatemap`

0 matching, 0 partial, 52 not started (52 total)

### `hashvalue`

6 matching, 0 partial, 0 not started (6 total)

| Function | Status | Match % | Size |
|---|---|---|---|
| `tHashName32_getHashValue(unsigned int*, char*)` | Matching | 100.00% | 88 |
| `GetHashValue32(char*)` | Matching | 100.00% | 32 |
| `tHashName64_getHashValue(unsigned long*, char*)` | Matching | 100.00% | 96 |
| `GetHashValue64(char*)` | Matching | 100.00% | 32 |
| `func_00317710(tHash128*, char*)` | Matching | 100.00% | 132 |
| `func_00317798(char*)` | Matching | 100.00% | 48 |

### `input/inputmap`

0 matching, 0 partial, 21 not started (21 total)

### `input/inputparse`

0 matching, 0 partial, 184 not started (184 total)

### `intersect/aifwddiff`

0 matching, 0 partial, 21 not started (21 total)

### `intersect/riderspheretree`

0 matching, 0 partial, 77 not started (77 total)

### `intersect/worldsphtree`

0 matching, 0 partial, 86 not started (86 total)

### `luno/lunovm`

0 matching, 0 partial, 71 not started (71 total)

### `main/debugmenu`

0 matching, 0 partial, 94 not started (94 total)

### `main/game`

0 matching, 0 partial, 67 not started (67 total)

### `main/gameload`

0 matching, 0 partial, 105 not started (105 total)

### `main/gamemode`

0 matching, 0 partial, 68 not started (68 total)

### `main/gamerender`

0 matching, 0 partial, 8 not started (8 total)

### `main/gamestate`

0 matching, 0 partial, 79 not started (79 total)

### `main/loadscreens_prestart`

0 matching, 0 partial, 34 not started (34 total)

### `main/sectionman`

6 matching, 0 partial, 6 not started (12 total)

| Function | Status | Match % | Size |
|---|---|---|---|
| `cSectionMan::func_0022E228` | Matching | 100.00% | 76 |
| `cSectionMan::func_0022E278` | Matching | 100.00% | 16 |
| `cSectionMan::func_0022E288(tSectionNode *)` | Matching | 100.00% | 16 |
| `cSectionMan::func_0022E298(int)` | Matching | 100.00% | 172 |
| `cSectionMan::func_0022E348(tSectionNode *)` | Matching | 100.00% | 24 |
| `cSectionMan::func_0022E360(tSectionNode *, tSectionNode *)` | Matching | 100.00% | 84 |

### `main/ssxapp`

0 matching, 0 partial, 58 not started (58 total)

### `main/ssxappdtor`

0 matching, 0 partial, 197 not started (197 total)

### `md5`

4 matching, 0 partial, 0 not started (4 total)

| Function | Status | Match % | Size |
|---|---|---|---|
| `md5_process` | Matching | 100.00% | 4116 |
| `md5_init` | Matching | 100.00% | 60 |
| `md5_append` | Matching | 100.00% | 292 |
| `md5_finish` | Matching | 100.00% | 212 |

### `object/animnode`

0 matching, 0 partial, 9 not started (9 total)

### `object/bucketman`

0 matching, 0 partial, 19 not started (19 total)

### `object/deadfadenode`

0 matching, 0 partial, 13 not started (13 total)

### `object/debouncenode`

0 matching, 0 partial, 44 not started (44 total)

### `object/effectlink`

0 matching, 0 partial, 30 not started (30 total)

### `object/flagset`

0 matching, 0 partial, 36 not started (36 total)

### `object/flexbridgenode`

0 matching, 0 partial, 17 not started (17 total)

### `object/flexrailnode`

0 matching, 0 partial, 34 not started (34 total)

### `object/floatingnode`

0 matching, 0 partial, 10 not started (10 total)

### `object/instanceman`

0 matching, 0 partial, 25 not started (25 total)

### `object/instancenode`

0 matching, 0 partial, 17 not started (17 total)

### `object/modifierblock`

0 matching, 0 partial, 37 not started (37 total)

### `object/movenode`

0 matching, 0 partial, 96 not started (96 total)

### `object/multiparticle`

0 matching, 0 partial, 33 not started (33 total)

### `object/objnode`

0 matching, 0 partial, 11 not started (11 total)

### `object/railmodifier`

0 matching, 0 partial, 401 not started (401 total)

### `object/spline3d`

0 matching, 0 partial, 4 not started (4 total)

### `object/splinemodifier`

0 matching, 0 partial, 35 not started (35 total)

### `object/spring`

0 matching, 0 partial, 4 not started (4 total)

### `path/pathsys`

0 matching, 0 partial, 24 not started (24 total)

### `render/bezierman`

0 matching, 0 partial, 20 not started (20 total)

### `render/font`

0 matching, 0 partial, 446 not started (446 total)

### `render/graphicsman`

0 matching, 0 partial, 59 not started (59 total)

### `render/irradiance`

0 matching, 0 partial, 2 not started (2 total)

### `render/lightman`

0 matching, 0 partial, 75 not started (75 total)

### `render/particle`

0 matching, 0 partial, 258 not started (258 total)

### `render/ps2graphicsman`

0 matching, 0 partial, 78 not started (78 total)

### `replay/frameptr`

0 matching, 0 partial, 56 not started (56 total)

### `replay/playbackman`

0 matching, 0 partial, 47 not started (47 total)

### `replay/replay`

0 matching, 0 partial, 44 not started (44 total)

### `replay/replaycache`

0 matching, 0 partial, 214 not started (214 total)

### `sce/crt0`

0 matching, 0 partial, 4 not started (4 total)

### `scripter/datamanager`

0 matching, 0 partial, 57 not started (57 total)

### `scripter/ssxscriptengine`

0 matching, 0 partial, 234 not started (234 total)

### `sound/asyncsys`

0 matching, 0 partial, 23 not started (23 total)

### `sound/bankmanager`

0 matching, 0 partial, 92 not started (92 total)

### `sound/bankmonitor`

0 matching, 0 partial, 213 not started (213 total)

### `sound/icepick/worldtriggermanager`

0 matching, 0 partial, 66 not started (66 total)

### `sound/ps2soundman`

0 matching, 0 partial, 3 not started (3 total)

### `sound/soundsys`

0 matching, 0 partial, 407 not started (407 total)

### `sound/ssxAudio`

0 matching, 0 partial, 105 not started (105 total)

### `sound/streamsys`

0 matching, 0 partial, 87 not started (87 total)

### `ui/uiengine`

0 matching, 0 partial, 36 not started (36 total)

### `ui/uilistbox`

0 matching, 0 partial, 16 not started (16 total)

### `ui/uimenu`

0 matching, 0 partial, 25 not started (25 total)

### `ui/uiscreen`

0 matching, 0 partial, 77 not started (77 total)

### `ui/uistatestack`

0 matching, 0 partial, 25 not started (25 total)

### `ui/uithing`

0 matching, 0 partial, 42 not started (42 total)

### `ui/uivector`

0 matching, 0 partial, 317 not started (317 total)

### `util/locale`

0 matching, 0 partial, 16 not started (16 total)

### `util/menu`

0 matching, 0 partial, 206 not started (206 total)

### `util/objectinterface`

0 matching, 0 partial, 25 not started (25 total)

### `util/statemachine`

0 matching, 0 partial, 20 not started (20 total)

### `visualfx/angerfx`

0 matching, 0 partial, 11 not started (11 total)

### `visualfx/avalanche`

0 matching, 0 partial, 35 not started (35 total)

### `visualfx/boardsparkfx`

0 matching, 0 partial, 5 not started (5 total)

### `visualfx/boardtrailfx`

0 matching, 0 partial, 35 not started (35 total)

### `visualfx/boardwakefx`

0 matching, 0 partial, 84 not started (84 total)

### `visualfx/crowdrender2d`

1 matching, 5 partial, 7 not started (13 total)

| Function | Status | Match % | Size |
|---|---|---|---|
| `cCrowdRender2D_cCrowdRender2D(int)` | Matching | 100.00% | 40 |
| `cCrowdRender2D_constructCrowdAnim2D(void*)` | Partial | 94.17% | 72 |
| `cCrowdRender2D__cCrowdRender2D(int *, int)` | Partial | 93.89% | 72 |
| `cCrowdAnim2D_cCrowdAnim2D(void *, void *)` | Partial | 8.75% | 64 |
| `cCrowdRender2D_purge(int*)` | Partial | 2.33% | 240 |
| `cCrowdRender2D_init()` | Partial | 1.71% | 328 |

### `visualfx/lensfx`

0 matching, 0 partial, 50 not started (50 total)

### `visualfx/renderstateman`

0 matching, 0 partial, 55 not started (55 total)

### `visualfx/worldlightman`

0 matching, 0 partial, 147 not started (147 total)

### `world/world`

0 matching, 0 partial, 19 not started (19 total)

### `world/worldview`

0 matching, 0 partial, 102 not started (102 total)

### `worldpainter/quadtree`

0 matching, 0 partial, 222 not started (222 total)

### `worldpainter/worldpainterman`

0 matching, 0 partial, 1 not started (1 total)

### `worldpainter/worldpaintermanquery`

0 matching, 0 partial, 76 not started (76 total)

### `worldpainter/worldpainterqueryctor`

0 matching, 0 partial, 18 not started (18 total)

### `wscript/wscriptcompile`

0 matching, 0 partial, 5 not started (5 total)

### `wscript/wscriptman`

0 matching, 0 partial, 182 not started (182 total)

### `wscript/wscriptprocess`

0 matching, 0 partial, 88 not started (88 total)

