# Setup

## Linux

```console
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=/path/to/your/boost/ ..
cmake --build .
```

then start server

```console
./server
```

# Settings

Some settings as server port, max allowed word size or other settings can be 
changed in settings.hpp

# Few words about decisions in code

There small inconsistency in sending data. As file we send as 
file_size+file_data, but json result is sent as json_string+'\0'. This decision 
was based on fact that in common case there possible zeros in file's data. But 
for string we know for sure that there aren't any zeros.


