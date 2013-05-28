Embedded PID Control Library
----------------------------

- Author: gbmhunter <<gbmhunter@gmail.com>> (http://www.cladlab.com)
- Created: 2012/10/01
- Last Modified: 2013/05/28
- Version: v2.2.1.0
- Company: CladLabs
- Project: n/a
- Language: C++
- Compiler: GCC	
- uC Model: All
- Computer Architecture: All
- Operating System: n/a
- Documentation Format: Doxygen
- License: GPLv3

Description
-----------

A light-weight, fast PID library designed for use on embedded systems (but can also run on any machine which has a G++ compiler).

For better performance and control, the PID library supports a generic number type. Number type must support casting to doubles. Fixed-point numbers are recommended for high-speed operation, doubles are recommended for non-time critical algorithms.

Relies on used calling `Pid.Run()` at a regular and fixed interval (usually in the milli-second range, via an interrupt).

Automatically adjusts Kp, Ki and Kd depending on the chosen time step (Zp, Zi, and Zd are the time-step adjusted values).

**DO NOT** try and make Kp, Ki, or Kd negative. This results in undefined behaviour.

Derivative control is only active when at least two calls to `Run()` have been made (does not assume previous input was 0 on first call, which can cause a huge derivative jolt!).

Can print debug information by setting `pidPRINT_DEBUG` in Pid.hpp to 1. Supports multiple debug outputs for portability (e.g. a hardware UART peripheral in a embedded environment, or the standard `printf()` for linux systems). Debug buffer size can be changed with `pidDEBUG_BUFF_SIZE`, again in Pid.hpp.

Internal Dependencies
---------------------
	
- UnitTest++ (test/UnitTest++). Lightweight C++ library for unit testing.
		
External Dependencies
---------------------

- FixedPoint library. Required if you want to use fixed-point number types with the PID control loops.

Usage
-----

	:::c++
	PidDbl pidTest;
	
See test/PidTest.cpp for more examples.
	
Issues
------

For known bugs, desired enhancements e.t.c, see BitBucket issues section.
	
Changelog
---------

- v1.0.0.0 		-> (2012/10/01) Initial commit.
- v1.0.0.1		-> (2013/05/27) Added README.rst.
- v1.1.0.0		-> (2013/05/27) Added units tests and UnitTest++ library to test/. Made debug printing portable (by creating PrintDebug() function). Fixed bug with settingSampleMs for PidDbl object (added "this->"). Prevented derivative control for influencing the output until at least two calls to Run() have been made (i.e. doesn't assume previous input was 0.0 on first call to Run()).
- v2.0.0.0		-> (2013/05/27) Added support for PID control of any data type by making PID class and functions templates. 
- v2.0.0.1		-> (2013/05/27) Fixed commit issues, removed .goutstream files.
- v2.0.0.2		-> (2013/05/27) Changed README.rst to README.md (to markdown format).
- v2.0.0.3		-> (2013/05/27) Fixed version numbers.
- v2.1.0.0		-> (2013/05/28) Converted more README sections to markdown format. Added Makefile for unit tests.
- v2.2.0.0		-> (2013/05/28) Removed specific fixed-point implementation in Pid.cpp, with the idea that you can now use the template code.
- v2.2.1.0		-> (2013/05/28) Changed zKp, zKi, zKd (time-scaled constants) to Zp, Zi, Zd. Removed incorrect include directory in test/Makefile.
