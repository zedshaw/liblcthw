echo "Running unit tests:"


#eg: sh  ./tests/runtests.sh leetcode romantransfer
testsuit="*"
testpath=tests/${testsuit}_tests

if [[ $# == 2 && "$2" != "all" ]];then
    project=$1
    testsuit=$2
    testpath=tests/${project}/${testsuit}_tests
fi

for i in ${testpath}
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
