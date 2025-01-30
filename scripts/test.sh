#!/usr/bin/bash

# https://stackoverflow.com/questions/5947742/how-to-change-the-output-color-of-echo-in-linux
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

error_msg(){
	echo -e "${RED}ERROR${NC}: $@"
}

success_msg(){
	echo -e "${GREEN}OK${NC}: $@"
}

warning_msg(){
	echo -e "${YELLOW}$@${NC}"
}

# $1 = test name
# $2 = executable name
# $3 = test dir
test_files(){
	local testfiles=$(find $3 -type f -name "*.c")
	local total="$(echo ${testfiles} | wc -w)"
	local success=0
	local fail=0

	for testfile in ${testfiles}; do
		$2 < ${testfile} &> /dev/null

		# https://stackoverflow.com/questions/21035121/increment-variable-value-by-1-shell-programming
		if [[ $? -ne 0 ]] ; then
			error_msg "${testfile}"
			$2 < ${testfile}
			fail=$((fail+1))
		else
			success_msg "${testfile}"
			$2 < ${testfile}
			success=$((success+1))
		fi

		echo ""
	done

	warning_msg "TEST SUMMARY - $1"
	echo -e "${RED}Fail${NC}: ${fail}/${total}"
	echo -e "${GREEN}OK${NC}:   ${success}/${total}"
}

run_tests(){
	local executable="./myparser"
	local testdir="$1"
	local testtitle=$testdir
	test_files ${testtitle} ${executable} ${testdir}
}

run_tests $1
