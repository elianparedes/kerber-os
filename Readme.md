# KerberOS

64-bit multiprocessing operative system capable of running console programs. This project is built on top of [x64BareBones](https://bitbucket.org/RowDaBoat/x64barebones/wiki/Home).

<p align="center">
  <img width="600" src="https://user-images.githubusercontent.com/70670549/174647354-4fb9ae44-78f0-4839-aa49-88d345e46572.gif">
</p>

# Building

1. To be able to build this project, the following tools are needed:

    `build-essential` package, which includes [GCC](https://gcc.gnu.org/) and [Make](https://www.gnu.org/software/make/manual/make.html). 
    ```
    sudo apt-get install build-essential
    ```

    [NASM](https://www.nasm.us/) for compiling assembly code.
    ```
    sudo apt-get install nasm
    ``` 

    [QEMU](https://www.qemu.org/) for running the OS image. It also offers a comfortable way to do system-wide debugging. 
    ```
    sudo apt-get install qemu-kvm \
                         libvirt-daemon-system \
                         libvirt-clients \
                         bridge-utils
    ``` 
    The following package versions were used in the making of this project: `GCC version 4.9.2`, `Make version 4.0`, `NASM version 2.11.5`, `QEMU version 2.1.2`. 

2. Build the Toolchain
    Execute the following commands inside KerberOS root folder:
    ```
    cd Toolchain
    make all
    ```
    
3. Build the Kernel
    From the project root directory, run:
    ```
    make all
    ```
    
# Booting   
To boot the OS, type the following command inside project root directory:
   ```
   ./run.sh
   ```
    
# Developing
Most of the header files in this project are docummented using Javadoc-style comments. Code format was set using [Clang-Format](https://clang.llvm.org/docs/ClangFormatStyleOptions.html) formatter with the following rules:
```
{ 
  BasedOnStyle: LLVM,
  IndentWidth: 4,
  AllowShortIfStatementsOnASingleLine: false,
  BreakBeforeBraces: Attach,
  AllowAllParametersOfDeclarationOnNextLine: true,
  AllowShortFunctionsOnASingleLine: None,
  AllowShortEnumsOnASingleLine: false,
  IndentCaseLabels: true,
  AlignConsecutiveMacros: AcrossEmptyLines,
  SortIncludes: CaseInsensitive,
  AlignTrailingComments: true,
  PointerAlignment: Right
}
```
