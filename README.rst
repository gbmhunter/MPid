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
- Last Modified: 2014/04/16
- Version: v3.1.2.2
- Company: CladLabs
- Project: n/a
- Language: C++
- Compiler: GCC	
- uC Model: All
- Computer Architecture: All
- Operating System: n/a
- Documentation Format: Doxygen
- License: GPLv3

.. role:: bash(code)
	:language: bash

Description
-----------

**Cpp-Pid supersedes C-Pid, the PID library written in C.**

A light-weight, fast PID library designed for use on embedded systems (but can also run on any machine which has a G++ compiler).

For better performance and control, the PID library supports a generic number type. Number type must support casting to doubles. Fixed-point numbers are recommended for high-speed operation, doubles are recommended for non-time critical algorithms.

Relies on used calling :code:`Pid.Run()` at a regular and fixed interval (usually in the milli-second range, via an interrupt).

Automatically adjusts :code:`Kp`, :code:`Ki` and :code:`Kd` depending on the chosen time step (:code:`Zp`, :code:`Zi`, and :code:`Zd` are the time-step adjusted values).

**DO NOT** try and make :code:`Kp`, :code:`Ki`, or :code:`Kd` negative. This results in undefined behaviour.

Smooth Control
--------------

Derivative control is only active when at least two calls to :code:`Run()` have been made (does not assume previous input was 0 on first call, which can cause a huge derivative jolt!).

Easy Debugging
--------------

You can print PID debug information by providing a callback via :code:`Pid::SetDebugPrintCallback()`, which supports method callbacks by utilizing the slotmachine-cpp library. 

You can disable all debug info (to free up some memory space) by setting :code:`cp3id_config_INCLUDE_DEBUG_CODE` in `include/Config.hpp` to `0`. The debug buffer size can be changed with `cp3id_config_DEBUG_BUFF_SIZE`, again in `Config.hpp`.

Dependencies
------------

====================== ==================== ======================================================================
Dependency             Delivery             Usage
====================== ==================== ======================================================================
<cstdint>              Standard C++ library Fixed-width variable type definitions (e.g. uint32_t).
UnitTest++             /lib/UnitTest++      Unit test framework (test code in ./test/).
slotmachine-cpp        /lib/slotmachine-cpp Callback support for debug and error messages.
====================== ==================== ======================================================================


Usage
-----

::

	// Create a PID object which uses double for all of it's calculations, inputs, and outputs.
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
v3.1.2.2 2014/04/16 Properly styled code in README, closes #29. Added slotmachine-cpp to list of dependencies in README, closes #27.
v3.1.2.1 2014/04/16 Attempt to fix identation issue in 'Usage' section of README, closes #28.
v3.1.2.0 2014/03/24 Fixed the git submodule address for slotmachine-cpp so it was public, now TravisCI should be able to access it, closes #26. 
v3.1.1.0 2014/03/24 Changed typedef enums into enum classes and added compiler flag -std=c++0x to Makefile, closes #18. Updated unit test file accordingly.
v3.1.0.0 2014/03/24 Added include/Config.hpp to hold all PID configuration settings. Renamed debug print macros to fit. Added slotmachine-cpp lib to lib/slotmachine-cpp, added callback functionality for printing debug information, closes #25. Fixed incorrect checking of the size of the debug buffer, closes #24. Added relevant info to README.
v3.0.2.0 2014/03/24 Renamed actualKp and associated variables to just Kp, as Zp is now used to describe the time-adjusted ones, closes #2. Added comment to example code in README. Pid::Run() now calculates pTerm correctly, closes #3. Fixed improper use of carat symbol in Pid::Run() bug, closes #23.
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