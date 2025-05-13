#!/usr/bin/awk -f

BEGIN {
    for (i=1; i<ARGC; ++i)
        process_file(ARGV[i])
}

function process_file(file,   line, header) {
    if (file in included) return
    included[file] = 1

    while ((getline line <file) > 0) {
        if (line !~ /#include "/) {
            print line
            continue
        }
        match(line, /"(\\.|[^"])+"/)
        header = normalize_header(substr(line, RSTART+1, RLENGTH-2), file)

        if (header in included) continue
        else process_file(header)
    }
    close(file)
}

function normalize_header(header, file) {
    sub(/[^\/]*$/, "", file) # basically dirname

    while (match(header, /^\.\.\//)) {
        sub(/^\.\.\//, "", header)
        sub(/[^\/]+\/$/, "", file)
    }
    return file header
}
