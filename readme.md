# battoexe

### Instalation

#### winget

Install llvm `winget install --id LLVM.LLVM` and add it's bin directory to your PATH.

Install BuildTools with VC
```
winget install Microsoft.VisualStudio.2022.BuildTools --force --override "--quiet --wait --add Microsoft.VisualStudio.Workload.VCTools;includeRecommended" 
```

Compile the builder using `build.bat`

### Ussage

```
builder.exe [relative path to a bat file]
```

The builder will create a directory (`output`). This directory will include: `b64data.h` - temporary file, `bat.exe` - your exe file. 