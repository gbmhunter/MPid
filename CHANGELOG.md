# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html) (as of v7.0.0).

## [Unreleased]

- Nothing here yet.

## [v5.0.0] - 2019-05-20

### Added

- Added .gitingore file.
- Added `LICENSE.txt` (MIT license).

### Changed

- Replaced Make build script with CMake.
- Replace the custom four number versioning with the standarized semantic verioning (three number).

## Old Style Changelog (pre v5.0.0)

========= ========== ===================================================================================================
Version   Date       Comment
========= ========== ===================================================================================================
v4.0.1.0  2014-10-11 Updated .travis.yml file to support C++11, closes #40.
v4.0.0.0  2014-10-10 Replaced UnitTest++ with the MUnitTest module, closes #32. Changed name to MPid, closes #31. Wrapped all code in MbeddedNinja namespace, closes #33. Removed slotmachine-cpp from 'lib/', closes #34. Updated Makefile to be similar to all of the other MToolkit modules, closes #35. Added main.cpp file for tests, closes #36. Moved all Pid::Init() code to constructor, closes #37.
v3.1.2.3  2014-04-16 Fixed up some code styling in the README. Added code example for assiging callback for debug printing, closes #30.
v3.1.2.2  2014-04-16 Properly styled code in README, closes #29. Added slotmachine-cpp to list of dependencies in README, closes #27.
v3.1.2.1  2014-04-16 Attempt to fix identation issue in 'Usage' section of README, closes #28.
v3.1.2.0  2014-03-24 Fixed the git submodule address for slotmachine-cpp so it was public, now TravisCI should be able to access it, closes #26. 
v3.1.1.0  2014-03-24 Changed typedef enums into enum classes and added compiler flag -std=c++0x to Makefile, closes #18. Updated unit test file accordingly.
v3.1.0.0  2014-03-24 Added include/Config.hpp to hold all PID configuration settings. Renamed debug print macros to fit. Added slotmachine-cpp lib to lib/slotmachine-cpp, added callback functionality for printing debug information, closes #25. Fixed incorrect checking of the size of the debug buffer, closes #24. Added relevant info to README.
v3.0.2.0  2014-03-24 Renamed actualKp and associated variables to just Kp, as Zp is now used to describe the time-adjusted ones, closes #2. Added comment to example code in README. Pid::Run() now calculates pTerm correctly, closes #3. Fixed improper use of carat symbol in Pid::Run() bug, closes #23.
v3.0.1.0  2014-03-24 Added this keyword to Init() function (and others) to prevent self-assignment bug, closes #19. Removed all references of the fixed-point library, now that the library accepts a dynamic data type, closes #20. Changed namespace from PidNs to CP3id, closes #22. Add more comments to PID functions. Updated README project title.
v3.0.0.0  2014-03-24 Moved src/include/ to include/. Added Eclipse C++ project files. Added api/CP3id.hpp, which is a single file you can include to use the CP3id library.
v2.5.1.0  2013-08-29 Cleaned up Makefile.
v2.5.0.1  2013-08-29 Fixed broken Travis CI build status image in README.
v2.5.0.0  2013-08-29 Added dependency awareness to Makefile, it now re-builds on header file changes.
v2.4.1.0  2013-08-28 Updated and fixed the build issue with the Makefile.
v2.4.0.3  2013-08-27 Changed reference to BitBucket in README to GitHub.
v2.4.0.2  2013-08-27 Changed README Changelog to table format.
v2.4.0.1  2013-08-27 Added Travis CI build status image that I had forgotten to do in the last commit.
v2.4.0.0  2013-08-27 Added .travis.yml file in root directory for Travis CI compatibility. Added Travis CI build status image to top of README.
v2.3.0.0  2013-05-31 Renamed README.md back to README.rst. Added Makefile in root dir that compiles everything and runs unit tests. Added note about Cpp-Pid superseding C-Pid. Removed object and .d files.
v2.2.2.1  2013-05-28 Added usage example to README.md.
v2.2.2.0  2013-05-28 Renamed namespace from Pid to PidNs and Pid class from PidDbl (no longer relevant with templating) to Pid.
v2.2.1.0  2013-05-28 Changed zKp, zKi, zKd (time-scaled constants) to Zp, Zi, Zd. Removed incorrect include directory in test/Makefile.
v2.2.0.0  2013-05-28 Removed specific fixed-point implementation in Pid.cpp, with the idea that you can now use the template code.
v2.1.0.0  2013-05-28 Converted more README sections to markdown format. Added Makefile for unit tests.
v2.0.0.3  2013-05-27 Fixed version numbers.
v2.0.0.2  2013-05-27 Changed README.rst to README.md (to markdown format).
v2.0.0.1  2013-05-27 Fixed commit issues, removed .goutstream files.
v2.0.0.0  2013-05-27 Added support for PID control of any data type by making PID class and functions templates. 
v1.1.0.0  2013-05-27 Added units tests and UnitTest++ library to test/. Made debug printing portable (by creating PrintDebug() function). Fixed bug with settingSampleMs for PidDbl object (added "this->"). Prevented derivative control for influencing the output until at least two calls to Run() have been made (i.e. doesn't assume previous input was 0.0 on first call to Run()).
v1.0.0.1  2013-05-27 Added README.rst.
v1.0.0.0  2012-10-01 Initial commit.
========= ========== ===================================================================================================