echo "/------------------------------------------------------------------------/"
echo "Test 0: Test first run"
echo "ush :"
echo 'exit' | ./ush
echo "zsh :"

echo "/------------------------------------------------------------------------/"
echo "Test 1: Test simple echo output"
echo "ush :"
echo 'echo "DzYibn13bQOSgW4XEfjFfD08LVO3AXG6b0AS30SeinNOQS61Zz44V6SY1cQRRiTq38YN4296ExSIV9pwwVop8Qy1LGMtQyLNg548Dj8wWQIUh0OzkCuKa6DKLRdWs95e2Bo"; exit' | ./ush
echo "zsh :"
echo "DzYibn13bQOSgW4XEfjFfD08LVO3AXG6b0AS30SeinNOQS61Zz44V6SY1cQRRiTq38YN4296ExSIV9pwwVop8Qy1LGMtQyLNg548Dj8wWQIUh0OzkCuKa6DKLRdWs95e2Bo"

echo "/------------------------------------------------------------------------/"
echo "Test 2 Test echo with options"
echo "ush :"
echo 'echo -n "TM0dRGgUenQO7IugMZHL920bsPOD3GVF4H7NVbAtSferTxcCoiUoLK8CtZ5CIAddwQvYoAruJiz71infiziSMXdDngkBQlNbHrizBD7BPNpzAJ4p"; exit' | ./ush
echo "\nzsh :"
echo "TM0dRGgUenQO7IugMZHL920bsPOD3GVF4H7NVbAtSferTxcCoiUoLK8CtZ5CIAddwQvYoAruJiz71infiziSMXdDngkBQlNbHrizBD7BPNpzAJ4p"

echo "/------------------------------------------------------------------------/"
echo "Test 3 Test echo with escape sequences"
echo "ush :"
echo 'echo -e "\t  \v \a s3ax \v  \tfWL3j6Rq47hPkZO \a" ; exit' | ./ush
echo "zsh :"
echo -e "\t  \v \a s3ax \v  \tfWL3j6Rq47hPkZO \a"

echo "/------------------------------------------------------------------------/"
echo "Test 4 Test which"
echo "ush :"
echo 'which exit ; exit' | ./ush
echo "zsh :"
which exit

echo "/------------------------------------------------------------------------/"
echo "Test 5 Test which with options"
echo "ush :"
echo 'which -a echo ; exit' | ./ush
echo "zsh :"
which -a echo

echo "/------------------------------------------------------------------------/"
echo "Test 6 Test which with options2"
echo "ush :"
echo 'which -s asdasdasdasdsa; echo $? ; exit' | ./ush
echo "zsh :"
which -s asdasdasdasdsa
echo $?

echo "/------------------------------------------------------------------------/"
echo "Test 7 Test which with options3"
echo "ush :"
echo 'which -s echo; echo $? ; exit' | ./ush
echo "zsh :"
which -s echo
echo $? 

echo "/------------------------------------------------------------------------/"
echo "Test 8 Test cd + pwd with a parent directory"
echo "ush :"
echo 'cd .. ; pwd ; exit' | ./ush
echo "zsh :"
cd ..
pwd
cd -

echo "/------------------------------------------------------------------------/"
echo "Test 9 Test cd with a single dash + pwd"
echo "ush :"
echo 'cd / ; pwd ; exit' | ./ush
echo "zsh :"
cd /
pwd
cd -

echo "/------------------------------------------------------------------------/"
echo "Test 10 Test cd + pwd with a root"
echo "ush :"
echo 'cd ../../../ ; cd - ; cd .. ; pwd ; exit' | ./ush
echo "zsh :"
cd ../../../
cd -
cd .. 
pwd 
cd -

echo "/------------------------------------------------------------------------/"
echo "Test 11 Test cd current/parent dir + pwd"
echo "ush :"
echo 'cd .. ; cd . ; pwd ; exit' | ./ush
echo "zsh :"
cd ..
pwd 
cd -

echo "/------------------------------------------------------------------------/"
echo "Test 12 Test cd + pwd with options"
echo "ush :"
echo 'cd /tmp ; pwd -L ; exit' | ./ush
echo "zsh :"
cd /tmp
pwd -L
cd -

echo "/------------------------------------------------------------------------/"
echo "Test 13 Test cd + pwd with options2"
echo "ush :"
echo 'cd /tmp ; pwd -P ; exit' | ./ush
echo "zsh :"
cd /tmp
pwd -P
cd -

echo "/------------------------------------------------------------------------/"
echo "Test 14 Test simple command substitution"
echo "ush :"
echo 'cd .. ; echo "$(pwd)" ; exit' | ./ush
echo "zsh :"
cd ..
echo "$(pwd)" 
cd -

echo "/------------------------------------------------------------------------/"
echo "Test 15: Test complicated command substitution"
echo "ush :"
echo 'echo "Hello, $(whoami)! $(echo "Hello, $(whoami)")" ; exit' | ./ush
echo "zsh :"
echo "Hello, $(whoami)! $(echo "Hello, $(whoami)")"

echo "/------------------------------------------------------------------------/"
echo "Test 16 Test complicated command substitution2"
echo "ush :"
echo 'cd .. ; echo "$(echo $(pwd))" ; exit' | ./ush
echo "zsh :"
cd ..
echo "$(echo $(pwd))" 
cd -

echo "/------------------------------------------------------------------------/"
echo "Test 17 Test work with shell variables"
echo "ush :"
echo 'echo ${SHLVL} ${HOME} ${LOGNAME} ${USR} ${TERM} ; exit' | ./ush
echo "zsh :"
echo ${SHLVL} ${HOME} ${LOGNAME} ${USR} ${TERM} #"2 /Users/oracle oracle"

echo "/------------------------------------------------------------------------/"
echo "Test 18 Test tilde expansion"
echo "ush :"
echo 'cd ~ ; pwd ; exit' | ./ush
echo "zsh :"
cd ~ 
pwd
cd -

echo "/------------------------------------------------------------------------/"
echo "Test 19 Test work with shell variables2"
echo "ush :"
echo 'export UNIT=location; export UFK=p2p ucode=cbl; echo $UNIT $UFK $ucode; exit' | ./ush
echo "zsh :"
export UNIT=location; export UFK=p2p ucode=cbl
echo $UNIT $UFK $ucode; #location p2p cbl

echo "/------------------------------------------------------------------------/"
echo "Test 20 Test work with shell variables3"
echo "ush :"
echo 'export UNIT=location UFK=p2p ucode=cbl ; echo $UNIT $UFK $ucode ; unset UNIT UFK ucode ; echo $UNIT $UFK $ucode ; exit' | ./ush
echo "zsh :"
export UNIT=location UFK=p2p ucode=cbl
echo $UNIT $UFK $ucode #location p2p cbl
unset UNIT UFK ucode
echo $UNIT $UFK $ucode #''

echo "/------------------------------------------------------------------------/"
echo "Test 21 Test work with shell variables3"
echo "ush :"
echo 'env -i env ; exit' | ./ush
echo "zsh :"
env -i env

echo "/------------------------------------------------------------------------/"