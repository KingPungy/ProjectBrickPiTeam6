scp -r ../cppserver/ pi@groep6-dex:~/socket_tests/files_new/
ssh pi@groep6-dex "cd socket_tests/ ; rm -r files/ ; mv files_new/ files/ ; cd files/ ; make"

