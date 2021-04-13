# VSCODE learning

Back to [README](README.md).

I have been learning about workspaces and started to work with them.

I also need to find out more about Arduino configuration as I am having some problems even with simple examples.

## Problems, some solved and some not.

 I have been having some strange errors. One of them looks like this:

### ENOENT problem (solved)

[Starting] Update package index files...
dummy
[Error] Exit with code=undefined

This goes away with a local settings.json in each repository.

I still have this one:

[Starting] Analyzing sketch 'pwmtest\pwmtest.ino'
[Warning] Output path is not specified. Unable to reuse previously compiled files. Build will be slower. See README.
[Warning] Failed to generate IntelliSense configuration.
[Error] Analyzing sketch 'pwmtest\pwmtest.ino': {"error":{"errno":"ENOENT","code":"ENOENT","syscall":"spawn E:\\Program Files (x86)\\Arduino\\E:\\Program Files (x86)\\Arduino","path":"E:\\Program Files (x86)\\Arduino\\E:\\Program Files (x86)\\Arduino","spawnargs":["--board","arduino:avr:uno","--verify","--verbose-build","e:\\GitHub\\Arduino\\MERG-Arduino-Software\\pwmtest\\pwmtest.ino"]}}

which I have for some cases but not others. This was because of a configuration error and I have now solved it. I had duplicated the "E:\\Program Files (x86)\\Arduino" part of the configuration and confused it. It took a few sessions of hunting to get rid of them all.

### Workspaces (in progress)

I have been doing some work with workspaces on an example which is not in GitHub. I have a suspicion that the configuration needs inside GitHub are different and I have not sorted that out. Some of the errors have started since I did the workspace work. I still have problems about access to Arduino default settings.

### Serial and USBCON (in progress)

I have now found that the problems with Serial and other things defined automatically in the Arduino IDE seems to be sorted out by defining USBCON. I have not found documentation for this, only a comment in a discussion. It turns out that this is not the full answer for this.

This is even more mysterious than I thought. The c_cpp_properties.json file here has two configurations called "Win32" and "Arduino" which come in that order. The file pwmtest.ino uses Serial. It fails unless USBCON is defined in Win32. If I put a define for USBCON into Arduino the intellisense removes it and the system fails to identify Serial. I report the facts but do not have an explanation.

### avr32/io.h (reported as a bug)

I still have this error with the CBUS codes. I cannot find the reference in CBUS2515.h. It is connected to use of SPI. I have flagged it as an issue with the Arduino extension to VS Code. I won't expect an answer there soon as they have 134 open issues. 

{
	"resource": "/E:/GitHub/Arduino/CBUSmINnOUT/CBUSmINnOUT.ino",
	"owner": "C/C++",
	"code": "1696",
	"severity": 8,
	"message": "cannot open source file \"avr32/io.h\" (dependency of \"CBUS2515.h\")",
	"source": "C/C++",
	"startLineNumber": 80,
	"startColumn": 1,
	"endLineNumber": 80,
	"endColumn": 22
}

In fact discussion of the problem has already started.

I moved the FreeRTOS library to other_libraries as it has a reference in it to AVR32. This did not solve the problem.

## Conclusion

I have not sorted out all the problems and I still do not understand why some things work and others not. Some of the more mysterious errors were due to mistakes I made in configuration of the arduino path information.


Back to [README](README.md).
