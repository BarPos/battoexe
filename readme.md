# battoexe

### Instalation

#### winget

Install llvm `winget install --id LLVM.LLVM` and add it's bin directory to your PATH.

Install BuildTools with VC
```
winget install Microsoft.VisualStudio.2022.BuildTools --force --override "--quiet --wait --add Microsoft.VisualStudio.Workload.VCTools;includeRecommended" 
```

Compile the builder using `build.bat`

Optional: Add the bin folder to path

### Ussage

```
battoexe.exe [bat file] [output file]
```