# hengam engine
![Capture](https://user-images.githubusercontent.com/75095946/162565226-141da942-f0a1-4058-9089-67f558f655d8.PNG)

## Goal
automatic creation processor of the hengam calendar

## Application
hengam engine converts Gregorian years to Jalali calendar years and exports .ics file.
you can import exported file to your laptop, pc or phone calendar app and use jalali calendar, plan and enjoy.

more about [icalendar](https://www.kanzaki.com/docs/ical/) format.

## install and use
in the [bin/](https://github.com/itismoradi/hengam/tree/master/engine/bin) directory, 
you can find executable hengam engine files and run.

### download 
- windows         : hengamEngine.exe [downlaod](https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/itismoradi/hengam/blob/master/engine/bin/hengamEngine.exe)
- linux and macOs : hengamEngine.out [download](https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/itismoradi/hengam/blob/master/engine/bin/hengamEngine.out)

### are you a developer?
you can compile it with a c++ compiler like [gcc](https://gcc.gnu.org/).

### using cmake build system
go to bin directory, write :
```
cmake ..
```

then use your generator, in MinGW Makefiles you must just write :
```
make
```

run hengam engine binary compiled file :
```
./hengamEngine
```

## libraries
libraries that used in this project are located in [library/](https://github.com/itismoradi/hengam/tree/master/engine/library) directory.

- [jalaliDate-Cpp](https://github.com/SCR-IR/jalaliDate-Cpp)  : Jalali-Gregorian date convertor

- [PicoSHA2](https://github.com/okdshin/PicoSHA2)  : hash UID property for each VEVENT

## License
the project published under [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html).
