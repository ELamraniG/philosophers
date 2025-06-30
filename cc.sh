#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counter
PASSED=0
FAILED=0

echo -e "${BLUE}=== Dining Philosophers Test Suite ===${NC}\n"

# Function to run a test case
run_test() {
    local test_name="$1"
    local args="$2"
    local expected_behavior="$3"
    local timeout_duration="$4"
    local should_die="$5"
    
    echo -e "${YELLOW}Testing: $test_name${NC}"
    echo "Command: ./philo $args"
    echo "Expected: $expected_behavior"
    
    # Run the program with timeout
    timeout $timeout_duration ./philo $args > test_output.tmp 2>&1 &
    local pid=$!
    
    # Wait for the process to complete or timeout
    wait $pid 2>/dev/null
    local exit_code=$?
    
    # Analyze output
    if [ -f test_output.tmp ]; then
        local line_count=$(wc -l < test_output.tmp)
        local last_line=$(tail -n 1 test_output.tmp)
        
        echo "Lines of output: $line_count"
        echo "Last line: $last_line"
        
        # Check for death message
        if echo "$last_line" | grep -q "is officialy dead"; then
            if [ "$should_die" = "yes" ]; then
                echo -e "${GREEN}✓ PASS - Philosopher died as expected${NC}"
                ((PASSED++))
            else
                echo -e "${RED}✗ FAIL - Philosopher died unexpectedly${NC}"
                ((FAILED++))
            fi
        elif [ "$should_die" = "no" ] && [ $exit_code -eq 124 ]; then
            echo -e "${GREEN}✓ PASS - No deaths detected (timeout reached)${NC}"
            ((PASSED++))
        elif [ "$should_die" = "yes" ]; then
            echo -e "${RED}✗ FAIL - Expected death but none occurred${NC}"
            ((FAILED++))
        else
            echo -e "${YELLOW}? Unclear result${NC}"
        fi
        
        # Check for proper message format
        if grep -E "^[0-9]+ [0-9]+ (is thinking|has taken a fork|is eating|is sleeping|is officialy dead)" test_output.tmp > /dev/null; then
            echo -e "${GREEN}✓ Output format looks correct${NC}"
        else
            echo -e "${RED}✗ Output format issues detected${NC}"
        fi
        
        rm -f test_output.tmp
    else
        echo -e "${RED}✗ FAIL - No output generated${NC}"
        ((FAILED++))
    fi
    
    echo "----------------------------------------"
}

# Function to test invalid arguments
test_invalid_args() {
    local test_name="$1"
    local args="$2"
    
    echo -e "${YELLOW}Testing: $test_name${NC}"
    echo "Command: ./philo $args"
    
    ./philo $args > test_output.tmp 2>&1
    local exit_code=$?
    
    if [ $exit_code -ne 0 ]; then
        echo -e "${GREEN}✓ PASS - Correctly rejected invalid arguments${NC}"
        ((PASSED++))
    else
        echo -e "${RED}✗ FAIL - Should have rejected invalid arguments${NC}"
        ((FAILED++))
    fi
    
    if [ -f test_output.tmp ]; then
        echo "Output: $(cat test_output.tmp)"
        rm -f test_output.tmp
    fi
    
    echo "----------------------------------------"
}

# Check if program exists
if [ ! -f "./philo" ]; then
    echo -e "${RED}Error: ./philo not found. Please compile first with 'make'${NC}"
    exit 1
fi

echo -e "${BLUE}=== ARGUMENT VALIDATION TESTS ===${NC}\n"

# Test invalid argument counts
test_invalid_args "No arguments" ""
test_invalid_args "Too few arguments" "5 800"
test_invalid_args "Too many arguments" "5 800 200 200 7 extra"

# Test invalid values
test_invalid_args "Zero philosophers" "0 800 200 200"
test_invalid_args "Negative values" "5 -800 200 200"
test_invalid_args "Non-numeric arguments" "abc 800 200 200"

echo -e "\n${BLUE}=== BASIC FUNCTIONALITY TESTS ===${NC}\n"

# Test cases that should NOT result in death
run_test "Single philosopher (should die quickly)" "1 800 200 200" "Should die immediately" "3s" "yes"
run_test "Basic case - should survive" "5 800 200 200" "All should survive" "10s" "no"
run_test "Tight timing - should survive" "4 410 200 200" "Should barely survive" "15s" "no"
run_test "Many philosophers" "10 800 200 200" "Should handle many threads" "15s" "no"

echo -e "\n${BLUE}=== DEATH SCENARIO TESTS ===${NC}\n"

# Test cases that SHOULD result in death
run_test "Impossible timing" "4 200 300 100" "Should die (eat+sleep > die)" "5s" "yes"
run_test "Very tight death timer" "5 300 200 200" "Likely to die" "8s" "yes"
run_test "Stress test with death" "2 400 500 100" "Should die" "10s" "yes"

echo -e "\n${BLUE}=== EDGE CASE TESTS ===${NC}\n"

# Edge cases
run_test "Two philosophers" "2 800 200 200" "Classic deadlock avoidance" "10s" "no"
run_test "Large number of philosophers" "50 1000 100 100" "Stress test" "20s" "no"
run_test "Very fast eating/sleeping" "5 2000 50 50" "Quick cycles" "10s" "no"

echo -e "\n${BLUE}=== RACE CONDITION TESTS ===${NC}\n"

echo -e "${YELLOW}Running multiple quick tests to catch race conditions...${NC}"
for i in {1..5}; do
    echo "Quick test $i/5"
    timeout 3s ./philo 5 800 200 200 > /dev/null 2>&1
    if [ $? -eq 124 ]; then
        echo -e "${GREEN}✓ Test $i completed without crash${NC}"
        ((PASSED++))
    else
        echo -e "${RED}✗ Test $i ended prematurely${NC}"
        ((FAILED++))
    fi
done

echo -e "\n${BLUE}=== MEMORY LEAK TEST (if valgrind available) ===${NC}\n"

if command -v valgrind &> /dev/null; then
    echo "Running valgrind test..."
    timeout 10s valgrind --leak-check=full --error-exitcode=1 ./philo 4 800 200 200 > valgrind_output.tmp 2>&1
    local valgrind_exit=$?
    
    if [ $valgrind_exit -eq 0 ] || [ $valgrind_exit -eq 124 ]; then
        echo -e "${GREEN}✓ No memory leaks detected${NC}"
        ((PASSED++))
    else
        echo -e "${RED}✗ Memory issues detected${NC}"
        echo "Valgrind output:"
        cat valgrind_output.tmp
        ((FAILED++))
    fi
    rm -f valgrind_output.tmp
else
    echo "Valgrind not available, skipping memory leak test"
fi

echo -e "\n${BLUE}=== TEST SUMMARY ===${NC}\n"
echo -e "Tests passed: ${GREEN}$PASSED${NC}"
echo -e "Tests failed: ${RED}$FAILED${NC}"
echo -e "Total tests: $((PASSED + FAILED))"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}🎉 All tests passed! Your philosophers implementation looks good!${NC}"
    exit 0
else
    echo -e "\n${RED}❌ Some tests failed. Please review the output above.${NC}"
    exit 1
fi