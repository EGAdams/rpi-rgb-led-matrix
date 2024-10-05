I need you to make a menu for a tennis game.

When the user enters a font name, the program will load the font.

Here is the C++ code that loads the font:
```cpp
FontLoader fontLoader( font_file.c_str());
fontLoader.LoadFont( _little_number_font ); 
```

Here is the list of the fonts directory:
```bash
roy@ROYS-LAPTOP:~$ ll rpi-rgb-led-matrix/fonts
total 12528
drwxr-xr-x  2 roy roy    4096 Jul 17 13:26 ./
drwxr-xr-x 18 roy roy    4096 Jul 17 13:27 ../
-rw-r--r--  1 roy roy  974681 Jul 17 13:26 10x20.bdf
-rw-r--r--  1 roy roy   94333 Jul 17 13:26 4x6.bdf
-rw-r--r--  1 roy roy  194747 Jul 17 13:26 5x7.bdf
-rw-r--r--  1 roy roy  154919 Jul 17 13:26 5x8.bdf
-rw-r--r--  1 roy roy  211073 Jul 17 13:26 6x10.bdf
-rw-r--r--  1 roy roy  549703 Jul 17 13:26 6x12.bdf
-rw-r--r--  1 roy roy  512118 Jul 17 13:26 6x13.bdf
-rw-r--r--  1 roy roy  159441 Jul 17 13:26 6x13B.bdf
-rw-r--r--  1 roy roy   95801 Jul 17 13:26 6x13O.bdf
-rw-r--r--  1 roy roy  144785 Jul 17 13:26 6x9.bdf
-rw-r--r--  1 roy roy  400761 Jul 17 13:26 7x13.bdf
-rw-r--r--  1 roy roy  124748 Jul 17 13:26 7x13B.bdf
-rw-r--r--  1 roy roy  103390 Jul 17 13:26 7x13O.bdf
-rw-r--r--  1 roy roy  327838 Jul 17 13:26 7x14.bdf
-rw-r--r--  1 roy roy  128518 Jul 17 13:26 7x14B.bdf
-rw-r--r--  1 roy roy  459959 Jul 17 13:26 8x13.bdf
-rw-r--r--  1 roy roy  141768 Jul 17 13:26 8x13B.bdf
-rw-r--r--  1 roy roy  160613 Jul 17 13:26 8x13O.bdf
-rw-r--r--  1 roy roy  765675 Jul 17 13:26 9x15.bdf
-rw-r--r--  1 roy roy  270622 Jul 17 13:26 9x15B.bdf
-rw-r--r--  1 roy roy  835008 Jul 17 13:26 9x18.bdf
-rw-r--r--  1 roy roy  133756 Jul 17 13:26 9x18B.bdf
-rw-r--r--  1 roy roy    2405 Jul 17 13:26 AUTHORS
-rw-r--r--  1 roy roy   15259 Jul 17 13:26 README
-rw-r--r--  1 roy roy    2472 Jul 17 13:26 README.md
-rw-r--r--  1 roy roy  145955 Jul 17 13:26 clR6x12.bdf
-rw-r--r--  1 roy roy   47662 Jul 17 13:26 fgm_27_ee.bdf
-rw-r--r--  1 roy roy   32704 Jul 17 13:26 fgm_27_ee.fon
-rw-r--r--  1 roy roy   32672 Jul 17 13:26 fgm_27_ee_may_3_bak.fon
-rw-r--r--  1 roy roy   32448 Jul 17 13:26 franklin_gothic_medium_27_eastern_european.fon
-rw-r--r--  1 roy roy  234648 Jul 17 13:26 helvR12.bdf
-rw-r--r--  1 roy roy   30983 Jul 17 13:26 little_numbers.bdf
-rw-r--r--  1 roy roy   13456 Jul 17 13:26 little_numbers.fon
-rw-r--r--  1 roy roy   13456 Jul 17 13:26 little_numbers_may_3_bak.fon
-rw-r--r--  1 roy roy 4795639 Jul 17 13:26 mspgothic
-rw-r--r--  1 roy roy   29452 Jul 17 13:26 mspgothic_030623.bdf
-rw-r--r--  1 roy roy   32064 Jul 17 13:26 mspgothic_030623.fon
-rw-r--r--  1 roy roy   29748 Jul 17 13:26 mspgothic_042623.bdf
-rw-r--r--  1 roy roy   32064 Jul 17 13:26 mspgothic_042623.fon
-rw-r--r--  1 roy roy   32064 Jul 17 13:26 mspgothic_small_numbers.fon
-rw-r--r--  1 roy roy  221573 Jul 17 13:26 texgyre-27.bdf
-rw-r--r--  1 roy roy   19964 Jul 17 13:26 tom-thumb.bdf
roy@ROYS-LAPTOP:~$
```

So the menu should have a list of the fonts, like this:
```
1. 10x20
2. 4x6
3. 5x7
...
```

Please write the C++ code for this new method.
