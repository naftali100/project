## links to materials used in this project
- sfml wiki (state manager, animation)
- collision handling: https://trederia.blogspot.com/2016/02/2d-physics-101-pong.html
- imgui: https://github.com/ocornut/imgui
- plog: https://github.com/SergiusTheBest/plog
- background music: https://www.ashamaluevmusic.com/
- magic enum: https://github.com/Neargye/magic_enum

## technologies used in this project
- concepts
- views
- singelton pattern
- observer pattern
- callback lambdas
- pre compiled headers
- auto include cpp in cmake
- github actions

## Compile and run
```
cmake -B build
cmake --build build
./build/sfml-template
```

#### Debug
```
cmake -DCMAKE_BUILD_TYPE=Debug -B build
cmake --build build
gdb ./build/sfml-template
```

Monkeys Spinning Monkeys Kevin MacLeod (incompetech.com)
Licensed under Creative Commons: By Attribution 3.0 License
http://creativecommons.org/licenses/by/3.0/
Music promoted by https://www.chosic.com/free-music/all/ 