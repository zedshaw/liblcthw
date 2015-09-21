echo "Running unit tests:"

for i in tests/*_tests
do
    $i 2>&1 >> tests/tests.log
    if [ ! $? ]
    then
        echo "ERROR in test $i: here's tests/tests.log"
        echo "------"
        tail tests/tests.log
        exit 1
    fi
done

echo ""
