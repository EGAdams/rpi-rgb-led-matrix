clear;
make -f Makefile.remote_listener -k 2>&1 | tee /dev/tty | grep -c "error" | awk '{print "*** Make Error Count: " $1 " ***"}'
