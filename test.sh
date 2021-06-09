# echo "/------------------------------------------------------------------------/"
# echo "Test 1: Test simple echo output"
# echo "ush :"
# echo 'echo "DzYibn13bQOSgW4XEfjFfD08LVO3AXG6b0AS30SeinNOQS61Zz44V6SY1cQRRiTq38YN4296ExSIV9pwwVop8Qy1LGMtQyLNg548Dj8wWQIUh0OzkCuKa6DKLRdWs95e2Bo"; exit' | ./ush
# echo "zsh :"
# echo "DzYibn13bQOSgW4XEfjFfD08LVO3AXG6b0AS30SeinNOQS61Zz44V6SY1cQRRiTq38YN4296ExSIV9pwwVop8Qy1LGMtQyLNg548Dj8wWQIUh0OzkCuKa6DKLRdWs95e2Bo"

# echo "/------------------------------------------------------------------------/"
# echo "Test 2 Test echo with options"
# echo "ush :"
# echo 'echo "TM0dRGgUenQO7IugMZHL920bsPOD3GVF4H7NVbAtSferTxcCoiUoLK8CtZ5CIAddwQvYoAruJiz71infiziSMXdDngkBQlNbHrizBD7BPNpzAJ4p"; exit' | ./ush
# echo "zsh :"
# echo "TM0dRGgUenQO7IugMZHL920bsPOD3GVF4H7NVbAtSferTxcCoiUoLK8CtZ5CIAddwQvYoAruJiz71infiziSMXdDngkBQlNbHrizBD7BPNpzAJ4p"

echo "/------------------------------------------------------------------------/"
echo "Test 3 Test echo with escape sequences"
echo "ush :"
echo 'echo -e "\\t \\v \\a MwdFmFpmm \\v \\twwimyHu8qA \\a"' | ./ush
echo "zsh :"
echo -e "\\t \\v \\a MwdFmFpmm \\v \\twwimyHu8qA \\a"

echo "Test 3_1 Test with -E option"
echo "ush :"
echo 'echo -E "\\t \\v \\a MwdFmFpmm \\v \\twwimyHu8qA \\a"' | ./ush
echo "zsh :"
echo -E "\\t \\v \\a MwdFmFpmm \\v \\twwimyHu8qA \\a"

# echo "/------------------------------------------------------------------------/"
# echo "Test 4 Test which"
# echo "ush :"
# echo 'which exit' | ./ush
# echo "zsh :"
# echo "exit: ush built-in command"

# echo "/------------------------------------------------------------------------/"
# echo "Test 5 Test which with options"
# echo "ush :"
# echo 'which echo' | ./ush
# echo "zsh :"
# echo "echo: ush built-in command"

# echo "/------------------------------------------------------------------------/"
# echo "Test 6 Test which with options2"
# echo "ush :"
# echo 'which asdasdasdasdsa ; echo $?' | ./ush
# echo "zsh :"
# echo "asdasdasdasdsa not found\n1"

# echo "/------------------------------------------------------------------------/"
# echo "Test 7 Test which with options3"
# echo "ush :"
# echo 'which echo ; echo $?' | ./ush ; 
# echo "zsh :"
# echo "echo: ush built-in command\n0"

# echo "/------------------------------------------------------------------------/"
# echo "Test 10 Test cd + pwd with a root"
# echo "ush :"
# echo 'echo ' | ./ush
# echo "zsh :"
# echo ""

# echo "/------------------------------------------------------------------------/"
# echo "Test 12 Test cd + pwd with options"
# echo "ush :"
# echo 'echo ' | ./ush
# echo "zsh :"
# echo ""

echo "/------------------------------------------------------------------------/"
echo "Test 14 Test simple command substitution"
echo "ush :"
echo 'echo "$(pwd)"' | ./ush
echo "zsh :"
echo "$(pwd)"

echo "/------------------------------------------------------------------------/"
echo "Test 15: Test complicated command substitution"
echo "ush :"
echo 'echo "Hello, $(whoami)! $(echo "Hello, $(whoami)")"' | ./ush
echo "zsh :"
echo "Hello, $(whoami)! $(echo "Hello, $(whoami)")"

echo "/------------------------------------------------------------------------/"
echo "Test 16 Test complicated command substitution2"
echo "ush :"
echo 'echo "$(echo $(pwd))"' | ./ush
echo "zsh :"
echo "$(echo $(pwd))"

# echo "/------------------------------------------------------------------------/"
# echo "Test 17 Test work with shell variables"
# echo "ush :"
# echo 'echo ${SHLVL} ${HOME} ${LOGNAME} ${USER} ${TERM}' | ./ush
# echo "zsh :"
# echo ${SHLVL} ${HOME} ${LOGNAME} ${USER} ${TERM} #"2 /Users/oracle oracle"

# echo "Test 17_2"
# echo "ush :"
# echo ${SHLVL} ${HOME} ${LOGNAME} ${USER} ${TERM}
# echo "zsh :"
# echo ${SHLVL} ${HOME} ${LOGNAME} ${USER} ${TERM} #"2 /Users/oracle oracle"

# echo "/------------------------------------------------------------------------/"
# echo "Test 18 Test tilde expansion"
# echo "ush :"
# echo "cd ~ ; pwd" | ./ush
# echo "zsh :"
# cd ~ ; pwd ; cd -

echo "/------------------------------------------------------------------------/"
echo "Test 19 Test work with shell variables2"
echo "ush :"
echo 'UNIT=location UFK=p2p ucode=cbl ; echo $UNIT $UFK $ucode' | ./ush
echo "zsh :"
UNIT=location UFK=p2p ucode=cbl
echo "$UNIT $UFK $ucode" #location p2p cbl

echo "/------------------------------------------------------------------------/"
echo "Test 20 Test work with shell variables3"
echo "ush :"
echo 'UNIT=location UFK=p2p ucode=cbl ; echo $UNIT $UFK $ucode ; unset UNIT UFK ucode ; echo $UNIT $UFK $ucode' | ./ush
# OR
# echo "export UNIT=location UFK=p2p ucode=cbl ; echo $UNIT $UFK $ucode ; UNIT='' UFK='' ucode='' ; echo $UNIT $UFK $ucode" | ./ush
echo "zsh :"
UNIT=location UFK=p2p ucode=cbl
echo "$UNIT $UFK $ucode" #location p2p cbl
UNIT= UFK= ucode=
echo "$UNIT $UFK $ucode" #''

echo "/------------------------------------------------------------------------/"