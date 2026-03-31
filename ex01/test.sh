#!/usr/bin/env bash

green='\033[0;32m'
red='\033[0;31m'
yellow='\033[1;33m'
blue='\033[0;34m'
nc='\033[0m'

prog="./RPN"
passed=0
failed=0
total=0

printLine() {
	printf '%s\n' "------------------------------------------------------------"
}

printCaseResult() {
	local status="$1"
	local title="$2"
	local input="$3"
	local expectedOut="$4"
	local realOut="$5"
	local expectedErr="$6"
	local realErr="$7"
	local expectedCode="$8"
	local realCode="$9"

	printLine
	if [ "$status" = "OK" ]; then
		printf "${green}%s${nc} %s\n" "$status" "$title"
	else
		printf "${red}%s${nc} %s\n" "$status" "$title"
	fi
	printf "input:         [%s]\n" "$input"
	printf "expected out:  [%s]\n" "$expectedOut"
	printf "actual out:    [%s]\n" "$realOut"
	printf "expected err:  [%s]\n" "$expectedErr"
	printf "actual err:    [%s]\n" "$realErr"
	printf "expected code: [%s]\n" "$expectedCode"
	printf "actual code:   [%s]\n" "$realCode"
}

runCase() {
	local title="$1"
	local input="$2"
	local expectedOut="$3"
	local expectedErr="$4"
	local expectedCode="$5"

	local outFile
	local errFile
	local realOut
	local realErr
	local realCode

	outFile=$(mktemp)
	errFile=$(mktemp)

	if [ -z "$input" ]; then
		$prog >"$outFile" 2>"$errFile"
	else
		$prog "$input" >"$outFile" 2>"$errFile"
	fi
	realCode=$?

	realOut=$(cat "$outFile")
	realErr=$(cat "$errFile")

	rm -f "$outFile" "$errFile"

	total=$((total + 1))

	if [ "$realOut" = "$expectedOut" ] && [ "$realErr" = "$expectedErr" ] && [ "$realCode" -eq "$expectedCode" ]; then
		passed=$((passed + 1))
		printCaseResult "OK" "$title" "$input" "$expectedOut" "$realOut" "$expectedErr" "$realErr" "$expectedCode" "$realCode"
	else
		failed=$((failed + 1))
		printCaseResult "FAIL" "$title" "$input" "$expectedOut" "$realOut" "$expectedErr" "$realErr" "$expectedCode" "$realCode"
	fi
}

runCaseArgs() {
	local title="$1"
	local expectedOut="$2"
	local expectedErr="$3"
	local expectedCode="$4"
	shift 4

	local outFile
	local errFile
	local realOut
	local realErr
	local realCode
	local inputText

	outFile=$(mktemp)
	errFile=$(mktemp)

	$prog "$@" >"$outFile" 2>"$errFile"
	realCode=$?

	realOut=$(cat "$outFile")
	realErr=$(cat "$errFile")
	inputText="$*"

	rm -f "$outFile" "$errFile"

	total=$((total + 1))

	if [ "$realOut" = "$expectedOut" ] && [ "$realErr" = "$expectedErr" ] && [ "$realCode" -eq "$expectedCode" ]; then
		passed=$((passed + 1))
		printCaseResult "OK" "$title" "$inputText" "$expectedOut" "$realOut" "$expectedErr" "$realErr" "$expectedCode" "$realCode"
	else
		failed=$((failed + 1))
		printCaseResult "FAIL" "$title" "$inputText" "$expectedOut" "$realOut" "$expectedErr" "$realErr" "$expectedCode" "$realCode"
	fi
}

printf "${blue}Checking executable...${nc}\n"
if [ ! -e "$prog" ]; then
	printf "${red}Error:${nc} file %s not found\n" "$prog"
	exit 1
fi

if [ ! -x "$prog" ]; then
	printf "${red}Error:${nc} file %s is not executable\n" "$prog"
	exit 1
fi

printLine
printf "${yellow}VALID TESTS${nc}\n"

runCase "subject example" "8 9 * 9 - 9 - 9 - 4 - 1 +" "42" "" 0
runCase "simple addition" "1 2 +" "3" "" 0
runCase "simple subtraction" "9 3 -" "6" "" 0
runCase "simple multiplication" "7 6 *" "42" "" 0
runCase "simple division" "8 2 /" "4" "" 0
runCase "mixed operations" "5 1 2 + 4 * + 3 -" "14" "" 0
runCase "zero result" "1 2 * 2 / 2 * 2 4 - +" "0" "" 0
runCase "single digit alone" "7" "7" "" 0
runCase "negative result" "2 9 -" "-7" "" 0

printLine
printf "${yellow}INVALID TESTS${nc}\n"

runCase "no argument" "" "" "Error" 1
runCase "empty string argument" " " "" "Error" 1
runCase "invalid token letters" "1 a +" "" "Error" 1
runCase "multi digit token" "12 3 +" "" "Error" 1
runCase "parentheses not allowed" "(1 + 2)" "" "Error" 1
runCase "not enough operands 1" "+" "" "Error" 1
runCase "not enough operands 2" "1 +" "" "Error" 1
runCase "too many operators" "1 2 + +" "" "Error" 1
runCase "too many numbers" "1 2 3 +" "" "Error" 1
runCase "division by zero" "4 0 /" "" "Error" 1
runCase "bad symbol" "2 3 &" "" "Error" 1
runCase "only numbers" "1 2" "" "Error" 1
runCase "operator first" "* 2 3" "" "Error" 1
runCase "long invalid expression" "8 9 * 9 - x - 4 - 1 +" "" "Error" 1

printLine
printf "${yellow}ARGUMENT COUNT TESTS${nc}\n"

runCaseArgs "too many arguments" "" "Error" 1 "1 2 +" "3 4 +"

printLine
printf "${blue}RESULT:${nc} total=%d passed=%d failed=%d\n" "$total" "$passed" "$failed"
printLine

if [ "$failed" -ne 0 ]; then
	exit 1
fi
exit 0