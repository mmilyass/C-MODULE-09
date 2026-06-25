#!/bin/bash

BIN=./PmergeMe

# ===== F(n) table from TAOCP Vol.3 p.198 =====
# index = n
F=(
0   # 0 (unused)
0   # 1
1   # 2
3   # 3
5   # 4
7   # 5
10  # 6
13  # 7
16  # 8
19  # 9
22  # 10
26  # 11
30  # 12
34  # 13
38  # 14
42  # 15
46  # 16
50  # 17
54  # 18
58  # 19
62  # 20
66  # 21
71  # 22
76  # 23
81  # 24
86  # 25
91  # 26
96  # 27
101 # 28
106 # 29
111 # 30
116 # 31
121 # 32
126 # 33
)

# ===== helpers =====

is_sorted() {
    echo "$1" | tr ' ' '\n' | awk '
    NR==1 {prev=$1; next}
    { if ($1 < prev) exit 1; prev=$1 }
    END { exit 0 }'
}

run_test_shuf() {
    N="$1"

    INPUT=$(shuf -i 1-100000 -n "$N" | tr '\n' ' ')

    echo "-------------------------------------"
    echo "Shuffled input (n=$N)"

    OUT=$($BIN $INPUT 2>/dev/null)
    if [ -z "$OUT" ]; then
        echo "✘ Program produced no output"
        exit 1
    fi

    SORTED=$(echo "$OUT" | sed -n '1p')
    CMP=$(echo "$OUT" | sed -n '2p' | tr -d ' ')

    # 1️⃣ sorted check
    if is_sorted "$SORTED"; then
        echo "✔ Sorted: OK"
    else
        echo "✘ Sorted: FAIL"
        exit 1
    fi

    # 2️⃣ comparison LIMIT check
    if [ "$N" -lt "${#F[@]}" ]; then
        LIMIT=${F[$N]}
        if [ "$CMP" -le "$LIMIT" ]; then
            echo "✔ Comparisons: $CMP (≤ $LIMIT)"
        else
            echo "✘ Comparisons: $CMP (> $LIMIT)"
            exit 1
        fi
    else
        echo "ℹ Comparisons skipped (n too large)"
    fi
}

# ===== TESTS =====

echo "===== SMALL N (1 → 15) ====="
for n in $(seq 1 15); do
    run_test_shuf "$n"
done

echo
echo "===== MEDIUM RANDOM ====="
for n in 20 25 30; do
    run_test_shuf "$n"
done

echo
echo "===== LARGE (STRESS) ====="
run_test_shuf 100
run_test_shuf 500

echo
echo "===== MAX SIZE (NO CMP CHECK) ====="
INPUT=$(shuf -i 1-100000 -n 3000 | tr '\n' ' ')
$BIN $INPUT > /dev/null
echo "✔ Large test (3000 elements) executed"

echo
echo "✅ ALL SHUFFLE TESTS PASSED"