# File splits

`config/ssx3_us.yaml` splits the code into 169 named translation units, all `cpp` except `sce/crt0`.
Six of them (`visualfx/crowdrender2d`, `hashvalue`, `bxrandom`, `bx/bxstring`,
`md5`, `dirtysock/tags`) were split by hand. The other 162 take their
boundaries from the segment ranges in `docs/notes/functions/`. Those ranges
come from an `AI-Split-ID` branch that is not in this repository.

Before use, each range was checked against the disassembly:

- no ranges overlap
- every boundary is 8-byte aligned
- every range starts on a function splat already recognises

The rebuilt ELF still matches the original (sha1 check), but that only proves
the boundaries are byte-safe. It does not prove they are the original file
boundaries. Code between named ranges stays in the offset-named `asm` blocks.

## Names

Names come from the notes. These 35 are not in the prototype file list in
`src/readme.md`, so treat them as guesses:

- `bx/bxstringctor`
- `bx/execman`
- `camera/trigger/cameratriggerfactory`
- `dirtysock/tags`
- `fe/fepopupmisc`
- `fe/festateaudiooptions`
- `fe/festatebonusmaterial`
- `fe/festatebuyattrib`
- `fe/festatecharequipdetail`
- `fe/festatelegal`
- `fe/festateloadhint`
- `fe/festateloadscreen`
- `fe/festatelodge`
- `fe/festateriderbiodetail`
- `fe/festatetransition`
- `fe/festatetrophyroom`
- `fe/ovstateenterlodge`
- `fe/ovstatetrophy`
- `fe/ovtemplatedialog`
- `fe/ovtemplatepausemenu`
- `fe/uitemplatemap`
- `main/gameload`
- `main/loadscreens_prestart`
- `main/ssxappdtor`
- `object/bucketman`
- `object/deadfadenode`
- `object/debouncenode`
- `object/effectlink`
- `object/flagset`
- `object/instanceman`
- `object/multiparticle`
- `visualfx/boardsparkfx`
- `worldpainter/worldpaintermanquery`
- `worldpainter/worldpainterqueryctor`
- `wscript/wscriptcompile`

`bx/bxstringctor` looks wrong: per the notes, its functions are path queries,
not string code.

## Units on the older assembler

These 32 units are assembled with `bin/ee-as.exe` instead of the
driver's assembler, because the driver's assembler pads some of their
INCLUDE_ASM code with nops that the game doesn't have. They are listed as
`SN_AS_2000_UNITS` in `configure.py`; see
[compiler-notes.md](compiler-notes.md) before decompiling in one of them.

- `ai/control/handplantcontrol`
- `be/beintnewrace`
- `be/belibrary`
- `be/beintecon`
- `be/beintmission`
- `be/beintreward`
- `camera/trigger/cameratriggerman`
- `fe/festateprofile`
- `fe/feasyncfile`
- `fe/festaterewards`
- `fe/fepopupmisc`
- `fe/ovstatehud`
- `fe/ovtemplatepausemenu`
- `fe/ovstatepause`
- `fe/uitemplatemap`
- `fe/ovstatemap`
- `main/ssxapp`
- `main/game`
- `main/gamestate`
- `main/gamemode`
- `replay/replaycache`
- `sound/soundsys`
- `visualfx/avalanche`
- `visualfx/boardtrailfx`
- `animation/animmodel`
- `bx/cubicspline`
- `input/inputparse`
- `intersect/aifwddiff`
- `render/graphicsman`
- `render/particle`
- `render/lightman`
- `render/font`
