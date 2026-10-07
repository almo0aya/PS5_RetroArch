#!/usr/bin/env python3
"""Patch the installed prospero-lld so --shared outputs get FreeBSD EI_OSABI 9."""
from __future__ import annotations

from pathlib import Path
import sys

path = Path(sys.argv[1] if len(sys.argv) > 1 else ".deps/native/ps5-payload-sdk/bin/prospero-lld")
text = path.read_text()
old = (
    'exec "${BIN_PATH}" \\\n'
    '     -m elf_x86_64 $PIE $LDSCRIPT --eh-frame-hdr \\\n'
    '     -z max-page-size=0x4000 -mllvm -emulated-tls \\\n'
    '     "$@" --hash-style=gnu\n'
)
new = (
    '"${BIN_PATH}" \\\n'
    '     -m elf_x86_64 $PIE $LDSCRIPT --eh-frame-hdr \\\n'
    '     -z max-page-size=0x4000 -mllvm -emulated-tls \\\n'
    '     "$@" --hash-style=gnu\n'
    'status=$?\n'
    'if [[ $status -eq 0 ]]; then\n'
    '  _out=""; _shared=0; _args=("$@")\n'
    '  for ((i=0; i<${#_args[@]}; i++)); do\n'
    '    case "${_args[$i]}" in\n'
    '      --shared) _shared=1 ;;\n'
    '      -o) _out="${_args[$((i+1))]}" ;;\n'
    '    esac\n'
    '  done\n'
    '  if [[ $_shared -eq 1 && -n $_out && -f $_out ]]; then\n'
    '    python3 -c \'import pathlib,sys; p=pathlib.Path(sys.argv[1]); b=bytearray(p.read_bytes()); b[7]=9 if len(b)>8 and b[:4]==b"\\x7fELF" and b[7]!=9 else b[7]; p.write_bytes(b)\' "$_out"\n'
    '  fi\n'
    'fi\n'
    'exit $status\n'
)
if "b[7]=9" in text or "b[7] = 9" in text:
    print("prospero-lld already sets FreeBSD OSABI on shared objects")
elif old not in text:
    raise SystemExit("prospero-lld does not match the public SDK wrapper; not patching blindly")
else:
    path.write_text(text.replace(old, new, 1))
    print("patched installed prospero-lld for EI_OSABI FreeBSD on --shared outputs")
