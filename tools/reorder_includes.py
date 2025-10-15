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
    # find top include block: start at first line, allow comments and blank lines, then include lines until first non-include
    i = 0
    n = len(lines)
    # skip initial shebang or BOM? just proceed
    # allow initial comments or blank lines
    while i < n and (lines[i].strip().startswith('//') or lines[i].strip().startswith('/*') or lines[i].strip()=='' or lines[i].strip().startswith('/*') or lines[i].lstrip().startswith('/*')):
        i += 1
    # collect includes from i
    inc_start = i
    includes = []
    while i < n and include_re.match(lines[i]):
        includes.append(lines[i].strip())
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
