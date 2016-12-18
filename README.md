# I237 Hardware Programming

This here is my repository for hardware programming course at
Estonian IT College that is based around a
[Arduino Mega 2560](https://www.arduino.cc/en/Main/ArduinoBoardMega2560).

This code us usualy updated once every two weeks when a new excersice is put up
by Silver Kits.

# How to install

Currently to test this code you have to clone the repository

```bash
git clone https://git.wut.ee/arti/i237.git hardware
cd hardware
```

And export a enviroment variable that points to a Arduino Mega board. The
real path will depend on the OS and Computer that you are using. For me it is
usualy `/dev/ttyACM0`.

```bash
export ARDUINO=/dev/ttyACM0
```

After that you can `make` the project.

```bash
make clean
make
make install
```

You can also chekcout previous labs using `git checkout <lab name>`. For example

```bash
git checkout lab02
```

Don't forget to `make clean` after each checkout!

# License

   Copyright (C) 2016 Arti Zirk <arti.zirk@gmail.com>

   This file is part of I237 Door Access program.

   I237 Door Access is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   I237 Door Access is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with I237 Door Access.  If not, see <http://www.gnu.org/licenses/>.

