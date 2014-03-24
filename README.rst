=====
CP3id
=====

----------------------------
Embedded PID Control Library
----------------------------

.. image:: https://travis-ci.org/gbmhunter/CP3id.png?branch=master   
	:target: https://travis-ci.org/gbmhunter/CP3id
	
- Author: gbmhunter <gbmhunter@gmail.com> (http://www.cladlab.com)
- Created: 2012/10/01
- Last Modified: 2014/03/24
- Version: v3.0.1.0
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

**Cpp-Pid supersedes C-Pid, the PID library written in C.**

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

::
	
	Pid<double> pidTest;

	main()
	{
		// Set-up PID controller, non accumulating
		pidTest.Init(
			1.0,									//!< Kp
			1.0,									//!< Ki
			1.0,									//!< Kd
			Pid<double>::PID_DIRECT,				//!< Control type
			Pid<double>::DONT_ACCUMULATE_OUTPUT,	//!< Control type
			10.0,									//!< Update rate (ms)
			-100.0,									//!< Min output
			100.0,									//!< Max output
			0.0										//!< Initial set-point
		);
	}
	
	// Call every 10.0ms (as set in Pid.Init())
	TimerIsr()
	{
		// Read input
		input = ReadPin(2);
		// Perform one execution
		pidTest.Run(input);
		// Set output
		SetPin(3) = pidTest.output;
	}
	
See test/PidTest.cpp for more examples.
	
Issues
------

For known bugs, desired enhancements e.t.c, see GitHub issues section.
	
Changelog
---------

======== ========== ===================================================================================================
Version  Date       Comment
======== ========== ===================================================================================================
v3.0.1.0 2014/03/24 Added this keyword to Init() function (and others) to prevent self-assignment bug, closes #19. Removed all references of the fixed-point library, now that the library accepts a dynamic data type, closes #20. Changed namespace from PidNs to CP3id, closes #22. Add more comments to PID functions. Updated README project title.
v3.0.0.0 2014/03/24 Moved src/include/ to include/. Added Eclipse C++ project files. Added api/CP3id.hpp, which is a single file you can include to use the CP3id library.
v2.5.1.0 2013/08/29 Cleaned up Makefile.
v2.5.0.1 2013/08/29 Fixed broken Travis CI build status image in README.
v2.5.0.0 2013/08/29 Added dependency awareness to Makefile, it now re-builds on header file changes.
v2.4.1.0 2013/08/28 Updated and fixed the build issue with the Makefile.
v2.4.0.3 2013/08/27 Changed reference to BitBucket in README to GitHub.
v2.4.0.2 2013/08/27 Changed README Changelog to table format.
v2.4.0.1 2013/08/27 Added Travis CI build status image that I had forgotten to do in the last commit.
v2.4.0.0 2013/08/27 Added .travis.yml file in root directory for Travis CI compatibility. Added Travis CI build status image to top of README.
v2.3.0.0 2013/05/31 Renamed README.md back to README.rst. Added Makefile in root dir that compiles everything and runs unit tests. Added note about Cpp-Pid superseding C-Pid. Removed object and .d files.
v2.2.2.1 2013/05/28 Added usage example to README.md.
v2.2.2.0 2013/05/28 Renamed namespace from Pid to PidNs and Pid class from PidDbl (no longer relevant with templating) to Pid.
v2.2.1.0 2013/05/28 Changed zKp, zKi, zKd (time-scaled constants) to Zp, Zi, Zd. Removed incorrect include directory in test/Makefile.
v2.2.0.0 2013/05/28 Removed specific fixed-point implementation in Pid.cpp, with the idea that you can now use the template code.
v2.1.0.0 2013/05/28 Converted more README sections to markdown format. Added Makefile for unit tests.
v2.0.0.3 2013/05/27 Fixed version numbers.
v2.0.0.2 2013/05/27 Changed README.rst to README.md (to markdown format).
v2.0.0.1 2013/05/27 Fixed commit issues, removed .goutstream files.
v2.0.0.0 2013/05/27 Added support for PID control of any data type by making PID class and functions templates. 
v1.1.0.0 2013/05/27 Added units tests and UnitTest++ library to test/. Made debug printing portable (by creating PrintDebug() function). Fixed bug with settingSampleMs for PidDbl object (added "this->"). Prevented derivative control for influencing the output until at least two calls to Run() have been made (i.e. doesn't assume previous input was 0.0 on first call to Run()).
v1.0.0.1 2013/05/27 Added README.rst.
v1.0.0.0 2012/10/01 Initial commit.
======== ========== ===================================================================================================