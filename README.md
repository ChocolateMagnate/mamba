# Mamba JIT Repository
Welcome to the Mamba official repository! This is the project that aims to build a versatile,
modular, and fault-tolerant JIT compiler infrastructure. We draw inspiration from the LLVM 
project and recognise the technical difficulties and engineering efforts behind making quality
JIT as the vast majority of compilers are tailored to specific languages. This is why the Mamba
project was started to research JIT innovations and build a language-agnostic runtime environment 
backend for a number of programming languages, originally targeting Python3. 

A large part of the motivation behind making this project is to research and try new solutions in 
the domain of JIT and to approach commonly found problems with a fresh perspective. As so, it is 
composed of a set of district modules that can be "plugged and play": arbitrarily replaced with 
other modules that fulfil the same public interface, and it would compile properly. These efforts
are done towards the spirit of experimentation and test different solutions in integration, as well
as allow front-end authors to tailor Mamba to their respective needs. 

Currently, the project is the work-in-progress and primarily targets Linux. The stable version is 
expected to be released by the end of the 2024th year. 

# Project structure
* `.conan/`: hidden directory used for the Conan package manager where its artefacts go
* `benchmarks/`: benchmarks evaluating performance
* `docs/`: documentation and client guides
* `include/`: header files and the public interface
  * `givers/`: OS resource managers, such as memory
    * `multithreading/`: support for multithreading capabilities
  * `os/`: operating-system-specific API wrappers. All code in this directory is separated into a
  distinct `os` namespace that includes routines that expand to specific system calls.
  * `types/`: implementation of the built-in types in the Mamba IR
* `src/`: the main implementation tree of the said interface. The code inside src reassembles the 
   structure of the header files, yet provides the implementations in separate directories.
* `target/`: the build directory used by the Make build system
* `test/`: the tests

# Compile from source
Currently on Linux is supported. If you are looking to compile Mamba from source on Windows,
consider installing WSL 2.0. Follow [the instructions](https://learn.microsoft.com/en-us/windows/wsl/install-manual)
from Microsoft. 

1. Clone repository:
```commandline
git clone https://github.com/ChocolateMagnate/mamba
```
2. Install dependencies using Conan:
```commandline
cd mamba
mkdir .conan
conan install . --build=missing --output-folder .conan/    
```
This command will install the dependencies on your local machine 
and compile the libraries if pre-built packages are not available.
3. Build:
```commandline
make all
```
You may also specify the `-j` switch to your thread count to compile 
multiple files in parallel or replace `all` with `libmamba.so` if you 
are not looking for the library itself. An example:
```commandline
make -j8 libmamba.so
```
4. (Optional) Run tests (requires debug):
```commandline
make tests
./target/tests
```
5. (Optional) Run benchmarks:
```commandline
make benchmarks
./target/benchmarks
```

# Licence
Copyright 2023-2024 Mamba Authors   
SPDX-Licence-Identifier: Apache-2.0

Licensed under the Apache Licence, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the Licence at

      http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software 
distributed under the Licence is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the Licence.