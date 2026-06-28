#!/usr/bin/env python3
import os


INCLUDE_ROOTS = [
    'Config',
    'Content/Data',
    'Content/Map/Heightmaps',
    'Scripts',
    'Source',
    'Tools',
]

ROOT_FILES = [
    '.editorconfig',
    '.gitattributes',
    '.gitignore',
    '.gitmodules',
    'DemoProject.uproject',
    'README.md',
    'run-tests.sh',
    'style-guide.md',
    'update-sdk.sh',
]

TEXT_EXTENSIONS = {
    '.build.cs',
    '.c',
    '.cpp',
    '.cs',
    '.csv',
    '.h',
    '.hpp',
    '.ini',
    '.json',
    '.md',
    '.py',
    '.sh',
    '.txt',
    '.uproject',
    '.yml',
    '.yaml',
}

EXCLUDED_DIRS = {
    '.git',
    '.vs',
    '.vscode',
    'Binaries',
    'Build',
    'DerivedDataCache',
    'Intermediate',
    'Plugins',
    'Saved',
    '__pycache__',
}


def project_root():
    return os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))


def has_text_extension(path):
    normalized = path.lower()
    return any(normalized.endswith(ext) for ext in TEXT_EXTENSIONS)


def is_excluded_dir(path):
    return os.path.basename(path) in EXCLUDED_DIRS


def collect_files(base_dir):
    files = []

    for root_file in ROOT_FILES:
        path = os.path.join(base_dir, root_file)
        if os.path.isfile(path):
            files.append(path)

    for include_root in INCLUDE_ROOTS:
        root_path = os.path.join(base_dir, include_root)
        if not os.path.isdir(root_path):
            continue

        for current_dir, dirs, names in os.walk(root_path):
            dirs[:] = [name for name in dirs if not is_excluded_dir(name)]

            for name in names:
                path = os.path.join(current_dir, name)
                if has_text_extension(path):
                    files.append(path)

    return sorted(set(files))


def line_count_for_file(path):
    try:
        with open(path, 'r', encoding='utf-8') as handle:
            return sum(1 for _ in handle)
    except UnicodeDecodeError:
        return None
    except OSError as error:
        print(f"Error reading {path}: {error}")
        return None


def file_line_counts(base_dir):
    counts = []

    for path in collect_files(base_dir):
        line_count = line_count_for_file(path)
        if line_count is None:
            continue

        rel_path = os.path.relpath(path, base_dir).replace('\\', '/')
        counts.append((line_count, rel_path))

    counts.sort(key=lambda item: item[0], reverse=True)
    return counts


def write_report(output_file, counts):
    with open(output_file, 'w', encoding='utf-8', newline='\n') as handle:
        handle.write('# Unreal Demo Text Files by Line Number\n\n')
        handle.write('| Lines | File Path |\n')
        handle.write('|-------|-----------|\n')
        for count, path in counts:
            handle.write(f'| {count} | `{path}` |\n')


def main():
    base_dir = project_root()
    docs_dir = os.path.join(base_dir, 'DOCS')
    output_file = os.path.join(docs_dir, 'FILES_BY_LINE_NUMBER.md')

    os.makedirs(docs_dir, exist_ok=True)

    counts = file_line_counts(base_dir)
    write_report(output_file, counts)

    print(f"Successfully generated {output_file} with {len(counts)} entries.")


if __name__ == '__main__':
    main()
