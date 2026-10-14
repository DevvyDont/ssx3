# Function matching report

Auto-generated from [objdiff](tools/objdiff) by diffing every unit's `obj/target` against `obj/current`. Regenerate with a dual build (`./configure.py -c -o && ninja`) followed by `python3 scripts/matching_report.py`; see [CONTRIBUTING.md](CONTRIBUTING.md).

Snapshot as of 2026-10-14.

**Status definitions:**
- **Matching** — byte-for-byte identical to the retail binary (100%), or a near-miss where objdiff's own report considers the function matched despite a lower raw percentage (this happens for a handful of functions where the only remaining byte difference is a relocation immediate baked in at link time by the original SN toolchain, not a real code difference — see the gp-relative note in [README.md](README.md)).
- **Partial** — has a C implementation (not raw `INCLUDE_ASM`) but doesn't yet byte-match; the percentage is objdiff's fuzzy match score.
- **Not started** — still a raw `INCLUDE_ASM` stub with no C implementation attempted.

| | Count | % of total |
|---|---|---|
| Matching | 81 | 0.24% |
| Partial | 10 | 0.03% |
| Not started | 33,172 | 99.73% |
| **Total** | **33,263** | |

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

0 matching, 0 partial, 3 not started (3 total)

### `data/33DF40.data`

0 matching, 0 partial, 15 not started (15 total)

### `data/342008.data`

0 matching, 0 partial, 7 not started (7 total)

### `data/342D38.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/343A18.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/346E40.data`

0 matching, 0 partial, 50 not started (50 total)

### `data/347990.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/34D1E0.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/34D218.data`

0 matching, 0 partial, 151 not started (151 total)

### `data/3522B0.data`

0 matching, 0 partial, 297 not started (297 total)

### `data/357900.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/357950.rodata`

0 matching, 0 partial, 6 not started (6 total)

### `data/357C58.rodata`

0 matching, 0 partial, 172 not started (172 total)

### `data/35B638.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/35B7C0.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/35B810.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/35B8A8.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/35CF30.rodata`

0 matching, 0 partial, 4 not started (4 total)

### `data/35DE30.rodata`

0 matching, 0 partial, 4 not started (4 total)

### `data/35E940.rodata`

0 matching, 0 partial, 8 not started (8 total)

### `data/35EC70.rodata`

0 matching, 0 partial, 8 not started (8 total)

### `data/360A80.rodata`

0 matching, 0 partial, 23 not started (23 total)

### `data/3611F0.rodata`

0 matching, 0 partial, 22 not started (22 total)

### `data/361B80.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/3623B8.rodata`

0 matching, 0 partial, 6 not started (6 total)

### `data/367BB8.rodata`

0 matching, 0 partial, 23 not started (23 total)

### `data/3687B8.rodata`

0 matching, 0 partial, 183 not started (183 total)

### `data/3714B8.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/372D50.rodata`

0 matching, 0 partial, 418 not started (418 total)

### `data/37D128.rodata`

0 matching, 0 partial, 21 not started (21 total)

### `data/3810A8.rodata`

0 matching, 0 partial, 187 not started (187 total)

### `data/382A80.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/382F48.rodata`

0 matching, 0 partial, 41 not started (41 total)

### `data/384C68.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/3875C8.rodata`

0 matching, 0 partial, 5 not started (5 total)

### `data/388480.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/388918.rodata`

0 matching, 0 partial, 2 not started (2 total)

### `data/388BB8.rodata`

0 matching, 0 partial, 54 not started (54 total)

### `data/38A618.rodata`

0 matching, 0 partial, 25 not started (25 total)

### `data/38AD60.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/38B5D0.rodata`

0 matching, 0 partial, 509 not started (509 total)

### `data/38E840.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/38E860.rodata`

0 matching, 0 partial, 14 not started (14 total)

### `data/394D60.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/394FC0.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/395078.rodata`

0 matching, 0 partial, 5 not started (5 total)

### `data/395D90.rodata`

0 matching, 0 partial, 16 not started (16 total)

### `data/396070.rodata`

0 matching, 0 partial, 142 not started (142 total)

### `data/397A58.rodata`

0 matching, 0 partial, 332 not started (332 total)

### `data/39C100.lit4`

0 matching, 0 partial, 135 not started (135 total)

### `data/39C4B0.lit4`

0 matching, 0 partial, 75 not started (75 total)

### `data/39E5D0.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/39E6F0.lit4`

0 matching, 0 partial, 3 not started (3 total)

### `data/39E724.lit4`

0 matching, 0 partial, 5 not started (5 total)

### `data/39E738.lit4`

0 matching, 0 partial, 2 not started (2 total)

### `data/39E7FC.lit4`

0 matching, 0 partial, 81 not started (81 total)

### `data/39EC48.lit4`

0 matching, 0 partial, 105 not started (105 total)

### `data/39EFA8.lit4`

0 matching, 0 partial, 80 not started (80 total)

### `data/39F344.lit4`

0 matching, 0 partial, 213 not started (213 total)

### `data/39F96C.lit4`

0 matching, 0 partial, 73 not started (73 total)

### `data/39FCDC.lit4`

0 matching, 0 partial, 99 not started (99 total)

### `data/3A015C.lit4`

0 matching, 0 partial, 151 not started (151 total)

### `data/3A0498.lit4`

0 matching, 0 partial, 5 not started (5 total)

### `data/3A07C0.lit4`

0 matching, 0 partial, 31 not started (31 total)

### `data/3A0D54.lit4`

0 matching, 0 partial, 14 not started (14 total)

### `data/3A11F4.lit4`

0 matching, 0 partial, 3 not started (3 total)

### `data/3A1200.lit4`

0 matching, 0 partial, 4 not started (4 total)

### `data/3A1210.lit4`

0 matching, 0 partial, 2 not started (2 total)

### `data/3A1BFC.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/3A1D2C.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/3A1D44.lit4`

0 matching, 0 partial, 69 not started (69 total)

### `data/3A1E58.lit4`

0 matching, 0 partial, 10 not started (10 total)

### `data/3A1E80.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/3A1E90.sdata`

0 matching, 0 partial, 59 not started (59 total)

### `data/3A21EC.sdata`

0 matching, 0 partial, 2 not started (2 total)

### `data/3A222C.sdata`

0 matching, 0 partial, 2 not started (2 total)

### `data/3A2254.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/3A22A0.sdata`

0 matching, 0 partial, 3 not started (3 total)

### `data/3A2418.sdata`

0 matching, 0 partial, 8 not started (8 total)

### `data/3A24C0.sdata`

0 matching, 0 partial, 2 not started (2 total)

### `data/3A26A8.sdata`

0 matching, 0 partial, 16 not started (16 total)

### `data/3A29E8.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/3A2EE0.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/3A3030.sdata`

0 matching, 0 partial, 8 not started (8 total)

### `data/3A3594.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/3A3770.sdata`

0 matching, 0 partial, 30 not started (30 total)

### `data/3A3E38.sdata`

0 matching, 0 partial, 170 not started (170 total)

### `data/3A44F0.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/3A4860.sdata`

0 matching, 0 partial, 4 not started (4 total)

### `data/3A4980.sdata`

0 matching, 0 partial, 3 not started (3 total)

### `data/3A4B88.sdata`

0 matching, 0 partial, 5 not started (5 total)

### `data/3A4BE8.sdata`

0 matching, 0 partial, 6 not started (6 total)

### `data/3A4DFC.sdata`

0 matching, 0 partial, 16 not started (16 total)

### `data/3A4E78.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/3A4E80.sdata`

0 matching, 0 partial, 2 not started (2 total)

### `data/3A4E94.sdata`

0 matching, 0 partial, 5 not started (5 total)

### `data/3A5768.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/3A57A8.sdata`

0 matching, 0 partial, 2 not started (2 total)

### `data/3A57D8.sdata`

0 matching, 0 partial, 7 not started (7 total)

### `data/3A5810.sdata`

0 matching, 0 partial, 68 not started (68 total)

### `data/3A5A94.sdata`

0 matching, 0 partial, 36 not started (36 total)

### `data/3A5BF4.sdata`

0 matching, 0 partial, 155 not started (155 total)

### `data/ai/ai.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/ai/ai.lit4`

0 matching, 0 partial, 353 not started (353 total)

### `data/ai/ai.rodata`

0 matching, 0 partial, 35 not started (35 total)

### `data/ai/ai.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/ai/airpredictor.lit4`

0 matching, 0 partial, 13 not started (13 total)

### `data/ai/computer.lit4`

0 matching, 0 partial, 88 not started (88 total)

### `data/ai/computer.rodata`

0 matching, 0 partial, 12 not started (12 total)

### `data/ai/control/handplantcontrol.lit4`

0 matching, 0 partial, 256 not started (256 total)

### `data/ai/control/handplantcontrol.rodata`

0 matching, 0 partial, 62 not started (62 total)

### `data/ai/control/handplantcontrol.sdata`

0 matching, 0 partial, 11 not started (11 total)

### `data/ai/motion/wipeoutmotion.lit4`

0 matching, 0 partial, 42 not started (42 total)

### `data/ai/rider.lit4`

0 matching, 0 partial, 94 not started (94 total)

### `data/ai/rider.rodata`

0 matching, 0 partial, 41 not started (41 total)

### `data/ai/rider.sdata`

0 matching, 0 partial, 3 not started (3 total)

### `data/ai/ridermetrix.data`

0 matching, 0 partial, 3 not started (3 total)

### `data/ai/ridermetrix.lit4`

0 matching, 0 partial, 86 not started (86 total)

### `data/ai/ridermetrix.rodata`

0 matching, 0 partial, 7 not started (7 total)

### `data/ai/ridermetrix.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/animation/animmodel.lit4`

0 matching, 0 partial, 2 not started (2 total)

### `data/animation/animmodel.rodata`

0 matching, 0 partial, 13 not started (13 total)

### `data/animation/animmodel.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/animation/animsequencer.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/animation/animsequencer.rodata`

0 matching, 0 partial, 2 not started (2 total)

### `data/animation/gameanimmap.lit4`

0 matching, 0 partial, 201 not started (201 total)

### `data/animation/rideranimbase.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/animation/rideranimbase.lit4`

0 matching, 0 partial, 4 not started (4 total)

### `data/animation/rideranimbase.rodata`

0 matching, 0 partial, 8 not started (8 total)

### `data/be/bedbreward.lit4`

0 matching, 0 partial, 69 not started (69 total)

### `data/be/bedbreward.rodata`

0 matching, 0 partial, 28 not started (28 total)

### `data/be/bedbreward.sdata`

0 matching, 0 partial, 2 not started (2 total)

### `data/be/beintaggression.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintaggression.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintbagt.rodata`

0 matching, 0 partial, 2 not started (2 total)

### `data/be/beintbagt.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintcharacter.lit4`

0 matching, 0 partial, 8 not started (8 total)

### `data/be/beintcharacter.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintecon.data`

0 matching, 0 partial, 3 not started (3 total)

### `data/be/beintecon.rodata`

0 matching, 0 partial, 6 not started (6 total)

### `data/be/beintecon.sdata`

0 matching, 0 partial, 2 not started (2 total)

### `data/be/beintmission.data`

0 matching, 0 partial, 4 not started (4 total)

### `data/be/beintmission.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintnetwork.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintnetwork.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintnewrace.data`

0 matching, 0 partial, 19 not started (19 total)

### `data/be/beintnewrace.lit4`

0 matching, 0 partial, 3 not started (3 total)

### `data/be/beintnewrace.rodata`

0 matching, 0 partial, 4 not started (4 total)

### `data/be/beintnewrace.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintoption.rodata`

0 matching, 0 partial, 2 not started (2 total)

### `data/be/beintoption.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintplayer.rodata`

0 matching, 0 partial, 10 not started (10 total)

### `data/be/beintplayer.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintreward.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/be/beintreward.rodata`

0 matching, 0 partial, 12 not started (12 total)

### `data/be/beintreward.sdata`

0 matching, 0 partial, 2 not started (2 total)

### `data/be/beintsavefile.rodata`

0 matching, 0 partial, 2 not started (2 total)

### `data/be/beintsavefile.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintscore.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/be/beintscore.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintscore.rodata`

0 matching, 0 partial, 2 not started (2 total)

### `data/be/beintscore.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintstat.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/beintstat.rodata`

0 matching, 0 partial, 2 not started (2 total)

### `data/be/beintstat.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/be/belibrary.rodata`

0 matching, 0 partial, 17 not started (17 total)

### `data/be/belibrary.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/bx/bxstring.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/bx/bxstring.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/bx/bxstringctor.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/bx/bxstringctor.lit4`

0 matching, 0 partial, 7 not started (7 total)

### `data/bx/bxstringctor.rodata`

0 matching, 0 partial, 2 not started (2 total)

### `data/bx/cubicspline.lit4`

0 matching, 0 partial, 3 not started (3 total)

### `data/bx/cubicspline.rodata`

0 matching, 0 partial, 4 not started (4 total)

### `data/bx/execman.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/bx/ps2main.lit4`

0 matching, 0 partial, 36 not started (36 total)

### `data/bx/ps2main.rodata`

0 matching, 0 partial, 8 not started (8 total)

### `data/bx/ps2main.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/camera/camera.lit4`

0 matching, 0 partial, 474 not started (474 total)

### `data/camera/camera.rodata`

0 matching, 0 partial, 21 not started (21 total)

### `data/camera/camera.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/camera/cameraalgolist.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/camera/cameraalgorithms.lit4`

0 matching, 0 partial, 9 not started (9 total)

### `data/camera/cameraalgorithms.rodata`

0 matching, 0 partial, 25 not started (25 total)

### `data/camera/cameracontroller.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/camera/script/scriptcontroller.lit4`

0 matching, 0 partial, 64 not started (64 total)

### `data/camera/script/scriptcontroller.rodata`

0 matching, 0 partial, 6 not started (6 total)

### `data/camera/trigger/cameratriggerfactory.lit4`

0 matching, 0 partial, 583 not started (583 total)

### `data/camera/trigger/cameratriggerfactory.rodata`

0 matching, 0 partial, 44 not started (44 total)

### `data/camera/trigger/cameratriggerfactory.sdata`

0 matching, 0 partial, 2 not started (2 total)

### `data/camera/trigger/cameratriggerman.lit4`

0 matching, 0 partial, 3 not started (3 total)

### `data/camera/trigger/cameratriggerman.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/dirtysock/tags.rodata`

0 matching, 0 partial, 10 not started (10 total)

### `data/dirtysock/tags.sdata`

0 matching, 0 partial, 5 not started (5 total)

### `data/fe/debugfe.lit4`

0 matching, 0 partial, 61 not started (61 total)

### `data/fe/debugfe.rodata`

0 matching, 0 partial, 59 not started (59 total)

### `data/fe/debugfe.sdata`

0 matching, 0 partial, 21 not started (21 total)

### `data/fe/feasyncfile.data`

0 matching, 0 partial, 12 not started (12 total)

### `data/fe/feasyncfile.lit4`

0 matching, 0 partial, 25 not started (25 total)

### `data/fe/feasyncfile.rodata`

0 matching, 0 partial, 712 not started (712 total)

### `data/fe/feasyncfile.sdata`

0 matching, 0 partial, 64 not started (64 total)

### `data/fe/fepopup.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/fe/fepopup.lit4`

0 matching, 0 partial, 5 not started (5 total)

### `data/fe/fepopup.rodata`

0 matching, 0 partial, 80 not started (80 total)

### `data/fe/fepopup.sdata`

0 matching, 0 partial, 85 not started (85 total)

### `data/fe/fepopupmisc.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/fepopupmisc.lit4`

0 matching, 0 partial, 13 not started (13 total)

### `data/fe/fepopupmisc.rodata`

0 matching, 0 partial, 141 not started (141 total)

### `data/fe/fepopupmisc.sdata`

0 matching, 0 partial, 34 not started (34 total)

### `data/fe/fereal.data`

0 matching, 0 partial, 4 not started (4 total)

### `data/fe/fereal.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/fereal.rodata`

0 matching, 0 partial, 60 not started (60 total)

### `data/fe/fereal.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/feridermanager.rodata`

0 matching, 0 partial, 19 not started (19 total)

### `data/fe/feridermanager.sdata`

0 matching, 0 partial, 9 not started (9 total)

### `data/fe/festateaudiooptions.rodata`

0 matching, 0 partial, 17 not started (17 total)

### `data/fe/festateaudiooptions.sdata`

0 matching, 0 partial, 14 not started (14 total)

### `data/fe/festatebigradio.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/festatebigradio.rodata`

0 matching, 0 partial, 32 not started (32 total)

### `data/fe/festatebigradio.sdata`

0 matching, 0 partial, 13 not started (13 total)

### `data/fe/festatebonusmaterial.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/festatebonusmaterial.rodata`

0 matching, 0 partial, 5 not started (5 total)

### `data/fe/festatebuyattrib.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/fe/festatebuyattrib.lit4`

0 matching, 0 partial, 2 not started (2 total)

### `data/fe/festatebuyattrib.rodata`

0 matching, 0 partial, 67 not started (67 total)

### `data/fe/festatebuyattrib.sdata`

0 matching, 0 partial, 24 not started (24 total)

### `data/fe/festatecharequipdetail.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/fe/festatecharequipdetail.lit4`

0 matching, 0 partial, 16 not started (16 total)

### `data/fe/festatecharequipdetail.rodata`

0 matching, 0 partial, 29 not started (29 total)

### `data/fe/festatecharequipdetail.sdata`

0 matching, 0 partial, 18 not started (18 total)

### `data/fe/festatecharselect.data`

0 matching, 0 partial, 3 not started (3 total)

### `data/fe/festatecharselect.lit4`

0 matching, 0 partial, 7 not started (7 total)

### `data/fe/festatecharselect.rodata`

0 matching, 0 partial, 23 not started (23 total)

### `data/fe/festatecharselect.sdata`

0 matching, 0 partial, 14 not started (14 total)

### `data/fe/festatecharsetup.lit4`

0 matching, 0 partial, 3 not started (3 total)

### `data/fe/festatecharsetup.rodata`

0 matching, 0 partial, 9 not started (9 total)

### `data/fe/festatecharsetup.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/festatecredits.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/festatecredits.rodata`

0 matching, 0 partial, 13 not started (13 total)

### `data/fe/festatedebug.rodata`

0 matching, 0 partial, 8 not started (8 total)

### `data/fe/festatedebug.sdata`

0 matching, 0 partial, 5 not started (5 total)

### `data/fe/festatelegal.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/festateload.rodata`

0 matching, 0 partial, 2 not started (2 total)

### `data/fe/festateload.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/festateloadhint.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/festateloadhint.rodata`

0 matching, 0 partial, 4 not started (4 total)

### `data/fe/festateloadhint.sdata`

0 matching, 0 partial, 57 not started (57 total)

### `data/fe/festateloadscreen.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/festateloadscreen.rodata`

0 matching, 0 partial, 7 not started (7 total)

### `data/fe/festatelodge.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/fe/festatelodge.rodata`

0 matching, 0 partial, 30 not started (30 total)

### `data/fe/festatelodge.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/festateoptions.data`

0 matching, 0 partial, 9 not started (9 total)

### `data/fe/festateoptions.lit4`

0 matching, 0 partial, 7 not started (7 total)

### `data/fe/festateoptions.rodata`

0 matching, 0 partial, 68 not started (68 total)

### `data/fe/festateoptions.sdata`

0 matching, 0 partial, 31 not started (31 total)

### `data/fe/festateprofile.rodata`

0 matching, 0 partial, 17 not started (17 total)

### `data/fe/festateprofile.sdata`

0 matching, 0 partial, 5 not started (5 total)

### `data/fe/festaterewards.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/fe/festaterewards.rodata`

0 matching, 0 partial, 52 not started (52 total)

### `data/fe/festaterewards.sdata`

0 matching, 0 partial, 4 not started (4 total)

### `data/fe/festateriderbio.rodata`

0 matching, 0 partial, 12 not started (12 total)

### `data/fe/festateriderbiodetail.rodata`

0 matching, 0 partial, 288 not started (288 total)

### `data/fe/festateriderbiodetail.sdata`

0 matching, 0 partial, 16 not started (16 total)

### `data/fe/festatestore.lit4`

0 matching, 0 partial, 2 not started (2 total)

### `data/fe/festatestore.rodata`

0 matching, 0 partial, 22 not started (22 total)

### `data/fe/festatestore.sdata`

0 matching, 0 partial, 11 not started (11 total)

### `data/fe/festatetrophyroom.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/fe/festatetrophyroom.rodata`

0 matching, 0 partial, 12 not started (12 total)

### `data/fe/festatetrophyroom.sdata`

0 matching, 0 partial, 8 not started (8 total)

### `data/fe/messagecenter.data`

0 matching, 0 partial, 3 not started (3 total)

### `data/fe/messagecenter.rodata`

0 matching, 0 partial, 28 not started (28 total)

### `data/fe/messagecenter.sdata`

0 matching, 0 partial, 7 not started (7 total)

### `data/fe/ovstateenterlodge.rodata`

0 matching, 0 partial, 43 not started (43 total)

### `data/fe/ovstateenterlodge.sdata`

0 matching, 0 partial, 17 not started (17 total)

### `data/fe/ovstatehud.lit4`

0 matching, 0 partial, 140 not started (140 total)

### `data/fe/ovstatehud.rodata`

0 matching, 0 partial, 32 not started (32 total)

### `data/fe/ovstatehud.sdata`

0 matching, 0 partial, 37 not started (37 total)

### `data/fe/ovstatelodge.data`

0 matching, 0 partial, 8 not started (8 total)

### `data/fe/ovstatelodge.rodata`

0 matching, 0 partial, 67 not started (67 total)

### `data/fe/ovstatelodge.sdata`

0 matching, 0 partial, 28 not started (28 total)

### `data/fe/ovstatemap.lit4`

0 matching, 0 partial, 4 not started (4 total)

### `data/fe/ovstatemap.rodata`

0 matching, 0 partial, 27 not started (27 total)

### `data/fe/ovstatemap.sdata`

0 matching, 0 partial, 2 not started (2 total)

### `data/fe/ovstatepause.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/fe/ovstatepause.lit4`

0 matching, 0 partial, 4 not started (4 total)

### `data/fe/ovstatepause.rodata`

0 matching, 0 partial, 147 not started (147 total)

### `data/fe/ovstatepause.sdata`

0 matching, 0 partial, 32 not started (32 total)

### `data/fe/ovstatereplay.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/ovstatereplay.rodata`

0 matching, 0 partial, 11 not started (11 total)

### `data/fe/ovstatereplay.sdata`

0 matching, 0 partial, 6 not started (6 total)

### `data/fe/ovstates.rodata`

0 matching, 0 partial, 8 not started (8 total)

### `data/fe/ovstates.sdata`

0 matching, 0 partial, 14 not started (14 total)

### `data/fe/ovstatetrophy.lit4`

0 matching, 0 partial, 19 not started (19 total)

### `data/fe/ovstatetrophy.rodata`

0 matching, 0 partial, 7 not started (7 total)

### `data/fe/ovstatetrophy.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/ovtemplatedialog.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/ovtemplatedialog.rodata`

0 matching, 0 partial, 20 not started (20 total)

### `data/fe/ovtemplatedialog.sdata`

0 matching, 0 partial, 3 not started (3 total)

### `data/fe/ovtemplatepausemenu.data`

0 matching, 0 partial, 3 not started (3 total)

### `data/fe/ovtemplatepausemenu.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/fe/ovtemplatepausemenu.rodata`

0 matching, 0 partial, 7 not started (7 total)

### `data/fe/ovtemplatepausemenu.sdata`

0 matching, 0 partial, 6 not started (6 total)

### `data/fe/uitemplatemap.lit4`

0 matching, 0 partial, 10 not started (10 total)

### `data/fe/uitemplatemap.rodata`

0 matching, 0 partial, 246 not started (246 total)

### `data/fe/uitemplatemap.sdata`

0 matching, 0 partial, 42 not started (42 total)

### `data/input/inputmap.rodata`

0 matching, 0 partial, 6 not started (6 total)

### `data/input/inputmap.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/input/inputparse.lit4`

0 matching, 0 partial, 14 not started (14 total)

### `data/input/inputparse.rodata`

0 matching, 0 partial, 44 not started (44 total)

### `data/input/inputparse.sdata`

0 matching, 0 partial, 24 not started (24 total)

### `data/intersect/aifwddiff.lit4`

0 matching, 0 partial, 7 not started (7 total)

### `data/intersect/aifwddiff.rodata`

0 matching, 0 partial, 2 not started (2 total)

### `data/intersect/aifwddiff.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/intersect/riderspheretree.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/intersect/riderspheretree.lit4`

0 matching, 0 partial, 32 not started (32 total)

### `data/intersect/riderspheretree.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/intersect/riderspheretree.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/intersect/worldsphtree.lit4`

0 matching, 0 partial, 133 not started (133 total)

### `data/intersect/worldsphtree.rodata`

0 matching, 0 partial, 7 not started (7 total)

### `data/luno/lunovm.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/luno/lunovm.lit4`

0 matching, 0 partial, 69 not started (69 total)

### `data/luno/lunovm.rodata`

0 matching, 0 partial, 7 not started (7 total)

### `data/main/debugmenu.lit4`

0 matching, 0 partial, 150 not started (150 total)

### `data/main/debugmenu.rodata`

0 matching, 0 partial, 348 not started (348 total)

### `data/main/debugmenu.sdata`

0 matching, 0 partial, 44 not started (44 total)

### `data/main/game.data`

0 matching, 0 partial, 5 not started (5 total)

### `data/main/game.lit4`

0 matching, 0 partial, 2 not started (2 total)

### `data/main/game.rodata`

0 matching, 0 partial, 9 not started (9 total)

### `data/main/game.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/main/gameload.data`

0 matching, 0 partial, 9 not started (9 total)

### `data/main/gameload.lit4`

0 matching, 0 partial, 10 not started (10 total)

### `data/main/gameload.rodata`

0 matching, 0 partial, 38 not started (38 total)

### `data/main/gameload.sdata`

0 matching, 0 partial, 13 not started (13 total)

### `data/main/gamemode.lit4`

0 matching, 0 partial, 5 not started (5 total)

### `data/main/gamemode.rodata`

0 matching, 0 partial, 4 not started (4 total)

### `data/main/gamerender.lit4`

0 matching, 0 partial, 5 not started (5 total)

### `data/main/gamerender.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/main/gamestate.lit4`

0 matching, 0 partial, 5 not started (5 total)

### `data/main/gamestate.rodata`

0 matching, 0 partial, 6 not started (6 total)

### `data/main/loadscreens_prestart.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/main/loadscreens_prestart.lit4`

0 matching, 0 partial, 9 not started (9 total)

### `data/main/loadscreens_prestart.rodata`

0 matching, 0 partial, 84 not started (84 total)

### `data/main/loadscreens_prestart.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/main/sectionman.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/main/sectionman.sdata`

0 matching, 0 partial, 5 not started (5 total)

### `data/main/ssxapp.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/main/ssxapp.lit4`

0 matching, 0 partial, 5 not started (5 total)

### `data/main/ssxapp.rodata`

0 matching, 0 partial, 238 not started (238 total)

### `data/main/ssxapp.sdata`

0 matching, 0 partial, 42 not started (42 total)

### `data/main/ssxappdtor.rodata`

0 matching, 0 partial, 50 not started (50 total)

### `data/main/ssxappdtor.sdata`

0 matching, 0 partial, 2 not started (2 total)

### `data/object/animnode.lit4`

0 matching, 0 partial, 3 not started (3 total)

### `data/object/animnode.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/object/bucketman.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/deadfadenode.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/debouncenode.rodata`

0 matching, 0 partial, 4 not started (4 total)

### `data/object/debouncenode.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/effectlink.lit4`

0 matching, 0 partial, 6 not started (6 total)

### `data/object/effectlink.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/flagset.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/flagset.lit4`

0 matching, 0 partial, 24 not started (24 total)

### `data/object/flagset.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/flagset.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/flexbridgenode.lit4`

0 matching, 0 partial, 14 not started (14 total)

### `data/object/flexbridgenode.rodata`

0 matching, 0 partial, 8 not started (8 total)

### `data/object/flexbridgenode.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/flexrailnode.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/object/flexrailnode.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/floatingnode.lit4`

0 matching, 0 partial, 2 not started (2 total)

### `data/object/floatingnode.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/instanceman.lit4`

0 matching, 0 partial, 2 not started (2 total)

### `data/object/modifierblock.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/modifierblock.rodata`

0 matching, 0 partial, 13 not started (13 total)

### `data/object/movenode.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/movenode.rodata`

0 matching, 0 partial, 4 not started (4 total)

### `data/object/movenode.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/multiparticle.lit4`

0 matching, 0 partial, 8 not started (8 total)

### `data/object/multiparticle.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/multiparticle.sdata`

0 matching, 0 partial, 3 not started (3 total)

### `data/object/objnode.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/object/railmodifier.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/object/railmodifier.lit4`

0 matching, 0 partial, 92 not started (92 total)

### `data/object/railmodifier.rodata`

0 matching, 0 partial, 58 not started (58 total)

### `data/object/railmodifier.sdata`

0 matching, 0 partial, 2 not started (2 total)

### `data/object/splinemodifier.lit4`

0 matching, 0 partial, 21 not started (21 total)

### `data/object/splinemodifier.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/object/spring.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/path/pathsys.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/path/pathsys.lit4`

0 matching, 0 partial, 71 not started (71 total)

### `data/path/pathsys.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/path/pathsys.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/render/bezierman.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/render/bezierman.lit4`

0 matching, 0 partial, 7 not started (7 total)

### `data/render/bezierman.rodata`

0 matching, 0 partial, 4 not started (4 total)

### `data/render/bezierman.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/render/font.lit4`

0 matching, 0 partial, 83 not started (83 total)

### `data/render/font.rodata`

0 matching, 0 partial, 24 not started (24 total)

### `data/render/font.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/render/graphicsman.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/render/graphicsman.lit4`

0 matching, 0 partial, 35 not started (35 total)

### `data/render/graphicsman.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/render/irradiance.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/render/lightman.data`

0 matching, 0 partial, 6 not started (6 total)

### `data/render/lightman.lit4`

0 matching, 0 partial, 7 not started (7 total)

### `data/render/lightman.rodata`

0 matching, 0 partial, 4 not started (4 total)

### `data/render/lightman.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/render/particle.data`

0 matching, 0 partial, 3 not started (3 total)

### `data/render/particle.lit4`

0 matching, 0 partial, 81 not started (81 total)

### `data/render/particle.rodata`

0 matching, 0 partial, 43 not started (43 total)

### `data/render/particle.sdata`

0 matching, 0 partial, 10 not started (10 total)

### `data/render/ps2graphicsman.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/render/ps2graphicsman.rodata`

0 matching, 0 partial, 13 not started (13 total)

### `data/render/ps2graphicsman.sdata`

0 matching, 0 partial, 2 not started (2 total)

### `data/replay/frameptr.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/replay/frameptr.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/replay/playbackman.lit4`

0 matching, 0 partial, 2 not started (2 total)

### `data/replay/playbackman.rodata`

0 matching, 0 partial, 7 not started (7 total)

### `data/replay/playbackman.sdata`

0 matching, 0 partial, 4 not started (4 total)

### `data/replay/replay.rodata`

0 matching, 0 partial, 15 not started (15 total)

### `data/replay/replay.sdata`

0 matching, 0 partial, 8 not started (8 total)

### `data/replay/replaycache.lit4`

0 matching, 0 partial, 69 not started (69 total)

### `data/replay/replaycache.rodata`

0 matching, 0 partial, 11 not started (11 total)

### `data/replay/replaycache.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/scripter/datamanager.lit4`

0 matching, 0 partial, 4 not started (4 total)

### `data/scripter/datamanager.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/scripter/datamanager.sdata`

0 matching, 0 partial, 3 not started (3 total)

### `data/scripter/ssxscriptengine.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/scripter/ssxscriptengine.lit4`

0 matching, 0 partial, 28 not started (28 total)

### `data/scripter/ssxscriptengine.rodata`

0 matching, 0 partial, 20 not started (20 total)

### `data/scripter/ssxscriptengine.sdata`

0 matching, 0 partial, 5 not started (5 total)

### `data/sound/asyncsys.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/sound/asyncsys.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/sound/bankmanager.lit4`

0 matching, 0 partial, 5 not started (5 total)

### `data/sound/bankmanager.rodata`

0 matching, 0 partial, 9 not started (9 total)

### `data/sound/bankmanager.sdata`

0 matching, 0 partial, 12 not started (12 total)

### `data/sound/bankmonitor.lit4`

0 matching, 0 partial, 3 not started (3 total)

### `data/sound/bankmonitor.rodata`

0 matching, 0 partial, 80 not started (80 total)

### `data/sound/bankmonitor.sdata`

0 matching, 0 partial, 26 not started (26 total)

### `data/sound/icepick/worldtriggermanager.lit4`

0 matching, 0 partial, 77 not started (77 total)

### `data/sound/icepick/worldtriggermanager.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/sound/ps2soundman.rodata`

0 matching, 0 partial, 10 not started (10 total)

### `data/sound/ps2soundman.sdata`

0 matching, 0 partial, 4 not started (4 total)

### `data/sound/soundsys.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/sound/soundsys.lit4`

0 matching, 0 partial, 25 not started (25 total)

### `data/sound/soundsys.rodata`

0 matching, 0 partial, 53 not started (53 total)

### `data/sound/soundsys.sdata`

0 matching, 0 partial, 42 not started (42 total)

### `data/sound/ssxAudio.lit4`

0 matching, 0 partial, 34 not started (34 total)

### `data/sound/ssxAudio.rodata`

0 matching, 0 partial, 12 not started (12 total)

### `data/sound/ssxAudio.sdata`

0 matching, 0 partial, 23 not started (23 total)

### `data/sound/streamsys.lit4`

0 matching, 0 partial, 3 not started (3 total)

### `data/sound/streamsys.rodata`

0 matching, 0 partial, 2 not started (2 total)

### `data/sound/streamsys.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/ui/uiengine.lit4`

0 matching, 0 partial, 2 not started (2 total)

### `data/ui/uiengine.rodata`

0 matching, 0 partial, 5 not started (5 total)

### `data/ui/uiengine.sdata`

0 matching, 0 partial, 3 not started (3 total)

### `data/ui/uilistbox.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/ui/uiscreen.rodata`

0 matching, 0 partial, 12 not started (12 total)

### `data/ui/uiscreen.sdata`

0 matching, 0 partial, 4 not started (4 total)

### `data/ui/uithing.lit4`

0 matching, 0 partial, 19 not started (19 total)

### `data/ui/uithing.rodata`

0 matching, 0 partial, 2 not started (2 total)

### `data/ui/uivector.lit4`

0 matching, 0 partial, 75 not started (75 total)

### `data/ui/uivector.rodata`

0 matching, 0 partial, 29 not started (29 total)

### `data/util/locale.data`

0 matching, 0 partial, 2 not started (2 total)

### `data/util/locale.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/util/menu.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/util/menu.lit4`

0 matching, 0 partial, 26 not started (26 total)

### `data/util/menu.rodata`

0 matching, 0 partial, 44 not started (44 total)

### `data/util/menu.sdata`

0 matching, 0 partial, 12 not started (12 total)

### `data/util/objectinterface.lit4`

0 matching, 0 partial, 1 not started (1 total)

### `data/util/objectinterface.sdata`

0 matching, 0 partial, 5 not started (5 total)

### `data/util/statemachine.rodata`

0 matching, 0 partial, 8 not started (8 total)

### `data/visualfx/angerfx.lit4`

0 matching, 0 partial, 9 not started (9 total)

### `data/visualfx/angerfx.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/visualfx/avalanche.lit4`

0 matching, 0 partial, 24 not started (24 total)

### `data/visualfx/avalanche.rodata`

0 matching, 0 partial, 5 not started (5 total)

### `data/visualfx/avalanche.sdata`

0 matching, 0 partial, 6 not started (6 total)

### `data/visualfx/boardsparkfx.lit4`

0 matching, 0 partial, 23 not started (23 total)

### `data/visualfx/boardsparkfx.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/visualfx/boardsparkfx.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/visualfx/boardtrailfx.lit4`

0 matching, 0 partial, 22 not started (22 total)

### `data/visualfx/boardtrailfx.rodata`

0 matching, 0 partial, 2 not started (2 total)

### `data/visualfx/boardtrailfx.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/visualfx/boardwakefx.lit4`

0 matching, 0 partial, 157 not started (157 total)

### `data/visualfx/boardwakefx.rodata`

0 matching, 0 partial, 25 not started (25 total)

### `data/visualfx/boardwakefx.sdata`

0 matching, 0 partial, 3 not started (3 total)

### `data/visualfx/crowdrender2d.data`

0 matching, 0 partial, 6 not started (6 total)

### `data/visualfx/crowdrender2d.rodata`

0 matching, 0 partial, 4 not started (4 total)

### `data/visualfx/lensfx.lit4`

0 matching, 0 partial, 14 not started (14 total)

### `data/visualfx/lensfx.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/visualfx/renderstateman.lit4`

0 matching, 0 partial, 4 not started (4 total)

### `data/visualfx/renderstateman.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/visualfx/renderstateman.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/visualfx/worldlightman.data`

0 matching, 0 partial, 1 not started (1 total)

### `data/visualfx/worldlightman.lit4`

0 matching, 0 partial, 326 not started (326 total)

### `data/visualfx/worldlightman.rodata`

0 matching, 0 partial, 27 not started (27 total)

### `data/visualfx/worldlightman.sdata`

0 matching, 0 partial, 1 not started (1 total)

### `data/vutext`

1 matching, 0 partial, 0 not started (1 total)

| Function | Status | Match % | Size |
|---|---|---|---|
| `D_0042E590` | Matching | 100.00% | 59348 |

### `data/world/world.rodata`

0 matching, 0 partial, 3 not started (3 total)

### `data/world/world.sdata`

0 matching, 0 partial, 3 not started (3 total)

### `data/world/worldview.lit4`

0 matching, 0 partial, 5 not started (5 total)

### `data/world/worldview.rodata`

0 matching, 0 partial, 8 not started (8 total)

### `data/worldpainter/quadtree.data`

0 matching, 0 partial, 8 not started (8 total)

### `data/worldpainter/quadtree.lit4`

0 matching, 0 partial, 90 not started (90 total)

### `data/worldpainter/quadtree.rodata`

0 matching, 0 partial, 17 not started (17 total)

### `data/worldpainter/quadtree.sdata`

0 matching, 0 partial, 8 not started (8 total)

### `data/worldpainter/worldpaintermanquery.lit4`

0 matching, 0 partial, 69 not started (69 total)

### `data/worldpainter/worldpainterqueryctor.lit4`

0 matching, 0 partial, 3 not started (3 total)

### `data/worldpainter/worldpainterqueryctor.rodata`

0 matching, 0 partial, 32 not started (32 total)

### `data/wscript/wscriptcompile.rodata`

0 matching, 0 partial, 1 not started (1 total)

### `data/wscript/wscriptman.lit4`

0 matching, 0 partial, 74 not started (74 total)

### `data/wscript/wscriptman.rodata`

0 matching, 0 partial, 11 not started (11 total)

### `data/wscript/wscriptprocess.rodata`

0 matching, 0 partial, 7 not started (7 total)

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

