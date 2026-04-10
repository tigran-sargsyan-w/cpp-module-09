#!/bin/bash

name="./PmergeMe"
total=0
passed=0

green="\033[0;32m"
red="\033[0;31m"
yellow="\033[1;33m"
blue="\033[0;34m"
reset="\033[0m"

printTitle() {
	echo
	echo -e "${blue}========================================${reset}"
	echo -e "${blue}$1${reset}"
	echo -e "${blue}========================================${reset}"
}

pass() {
	echo -e "${green}[OK]${reset} $1"
	passed=$((passed + 1))
	total=$((total + 1))
}

fail() {
	echo -e "${red}[KO]${reset} $1"
	total=$((total + 1))
}

skip() {
	echo -e "${yellow}[SKIP]${reset} $1"
}

extractAfter() {
	echo "$1" | grep "^After:" | sed 's/^After: //'
}

isSortedLine() {
	line="$1"
	prev=""
	for num in $line
	do
		if [ -n "$prev" ] && [ "$prev" -gt "$num" ]; then
			return 1
		fi
		prev="$num"
	done
	return 0
}

expectedSorted() {
	printf "%s\n" "$@" | sort -n | xargs
}

checkExecutable() {
	printTitle "Checking executable"

	if [ ! -f "$name" ]; then
		echo -e "${red}Executable not found:${reset} $name"
		echo "Run: make"
		exit 1
	fi

	if [ ! -x "$name" ]; then
		echo -e "${red}File is not executable:${reset} $name"
		echo "Run: chmod +x $name"
		exit 1
	fi

	echo -e "${green}Found executable:${reset} $name"
}

runValidTest() {
	label="$1"
	shift
	args="$*"

	output=$($name $args 2>&1)
	status=$?

	if [ "$status" -ne 0 ]; then
		fail "$label -> program returned error"
		echo "$output"
		return
	fi

	after=$(extractAfter "$output")
	expected=$(expectedSorted "$@")

	if [ -z "$after" ]; then
		fail "$label -> no After line found"
		echo "$output"
		return
	fi

	if ! isSortedLine "$after"; then
		fail "$label -> After line is not sorted"
		echo "$output"
		return
	fi

	if [ "$after" != "$expected" ]; then
		fail "$label -> sorted result mismatch"
		echo "Expected: $expected"
		echo "Got     : $after"
		echo "$output"
		return
	fi

	pass "$label"
}

runErrorTest() {
	label="$1"
	shift
	args="$*"

	output=$($name $args 2>&1)
	status=$?

	if [ "$status" -eq 0 ]; then
		fail "$label -> expected error but got success"
		echo "$output"
		return
	fi

	if echo "$output" | grep -q "^Error$"; then
		pass "$label"
	else
		fail "$label -> error message mismatch"
		echo "$output"
	fi
}

runNoArgTest() {
	label="$1"

	output=$($name 2>&1)
	status=$?

	if [ "$status" -eq 0 ]; then
		fail "$label -> expected failure without args"
		echo "$output"
		return
	fi

	if echo "$output" | grep -q "^Error$"; then
		pass "$label"
	else
		fail "$label -> wrong output without args"
		echo "$output"
	fi
}

runLargeUniqueTest() {
	label="$1"
	count="$2"

	if command -v shuf >/dev/null 2>&1; then
		args=$(shuf -i 1-100000 -n "$count" | tr '\n' ' ')
	elif command -v jot >/dev/null 2>&1; then
		args=$(jot -r "$count" 1 100000 | tr '\n' ' ')
	else
		skip "$label -> neither shuf nor jot found"
		return
	fi

	output=$($name $args 2>&1)
	status=$?

	if [ "$status" -ne 0 ]; then
		fail "$label -> program returned error"
		echo "$output"
		return
	fi

	after=$(extractAfter "$output")
	if [ -z "$after" ]; then
		fail "$label -> no After line found"
		echo "$output"
		return
	fi

	if ! isSortedLine "$after"; then
		fail "$label -> result is not sorted"
		echo "$output"
		return
	fi

	pass "$label"
}

runDuplicateTest() {
	label="$1"
	shift
	args="$*"

	output=$($name $args 2>&1)
	status=$?

	if [ "$status" -ne 0 ]; then
		fail "$label -> program returned error"
		echo "$output"
		return
	fi

	after=$(extractAfter "$output")
	expected=$(expectedSorted "$@")

	if [ -z "$after" ]; then
		fail "$label -> no After line found"
		echo "$output"
		return
	fi

	if [ "$after" != "$expected" ]; then
		fail "$label -> duplicate handling mismatch"
		echo "Expected: $expected"
		echo "Got     : $after"
		echo "$output"
		return
	fi

	pass "$label"
}

checkExecutable

printTitle "Mandatory / Basic valid tests"

runValidTest "single number" 42
runValidTest "two numbers sorted" 1 2
runValidTest "two numbers reverse" 2 1
runValidTest "odd count small" 3 5 9 7 4
runValidTest "even count small" 8 3 7 4 9 2
runValidTest "already sorted" 1 2 3 4 5 6 7 8
runValidTest "reverse sorted" 8 7 6 5 4 3 2 1
runValidTest "mixed medium" 8 3 7 4 9 2 6
runValidTest "another mixed medium" 10 42 3 17 8 99 1 5 73
runValidTest "bigger values" 2147483647 3 999999 42 17

printTitle "Duplicate tests"

runDuplicateTest "duplicates simple" 4 2 4 1 2 1
runDuplicateTest "duplicates many" 5 5 5 5 5
runDuplicateTest "duplicates mixed" 9 1 9 3 1 7 3 7 2

printTitle "Error tests"

runNoArgTest "no arguments"
runErrorTest "negative number" -1 2 3
runErrorTest "zero not allowed" 0 1 2
runErrorTest "word token" abc 1 2
runErrorTest "mixed invalid token" 12 x 4
runErrorTest "overflow int" 2147483648 1
runErrorTest "empty quoted string" ""
runErrorTest "signed plus token" + 1
runErrorTest "double sign token" --5 1
runErrorTest "float token" 1.5 2 3

printTitle "Large tests"

runLargeUniqueTest "large unique 100" 100
runLargeUniqueTest "large unique 1000" 1000
runLargeUniqueTest "large unique 3000" 3000

printTitle "Summary"

echo -e "Passed: ${green}${passed}${reset} / ${total}"

if [ "$passed" -eq "$total" ]; then
	echo -e "${green}All tests passed.${reset}"
	exit 0
else
	echo -e "${red}Some tests failed.${reset}"
	exit 1
fi