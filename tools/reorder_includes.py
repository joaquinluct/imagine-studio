#!/usr/bin/env python3
import re
import os

ROOT = os.path.dirname(os.path.dirname(__file__))
SRC_DIR = os.path.join(ROOT, 'src')

include_re = re.compile(r'^(\s*#include\s+["<].*[">])\s*$', re.MULTILINE)

exts = ['.cpp', '.cxx', '.cc', '.h', '.hpp']

def process_file(path):
    with open(path, 'r', encoding='utf-8') as f:
        text = f.read()

    lines = text.splitlines()
    # find first include line anywhere
    n = len(lines)
    inc_start = -1
    for idx in range(n):
        if include_re.match(lines[idx]):
            inc_start = idx
            break
    if inc_start == -1:
        return False

    # collect contiguous block of include or blank lines starting at inc_start
    i = inc_start
    includes = []
    while i < n and (include_re.match(lines[i]) or lines[i].strip() == ''):
        m = include_re.match(lines[i])
        if m:
            includes.append(m.group(1).strip())
        i += 1
    inc_end = i
    if not includes:
        return False
    # classify and sort
    quote_includes = [inc for inc in includes if '"' in inc]
    angle_includes = [inc for inc in includes if '<' in inc]
    quote_includes.sort(key=lambda s: s.lower())
    angle_includes.sort(key=lambda s: s.lower())
    new_block = quote_includes + ([''] if quote_includes and angle_includes else []) + angle_includes
    # build new content
    new_lines = lines[:inc_start] + new_block + lines[inc_end:]
    new_text = '\n'.join(new_lines) + ('\n' if text.endswith('\n') else '')
    if new_text != text:
        with open(path, 'w', encoding='utf-8') as f:
            f.write(new_text)
        return True
    return False

changed = []
for dirpath, dirnames, filenames in os.walk(SRC_DIR):
    for fn in filenames:
        if any(fn.endswith(ext) for ext in exts):
            path = os.path.join(dirpath, fn)
            try:
                if process_file(path):
                    changed.append(path)
            except Exception as e:
                print('ERROR processing', path, e)

print('Files changed:', len(changed))
for p in changed:
    print(p)
