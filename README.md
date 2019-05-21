# MPid

A microcontroller-friendly PID module.	

[![Build Status](https://travis-ci.org/gbmhunter/MPid.png?branch=master)](https://travis-ci.org/gbmhunter/MPid)

## Description

A light-weight, fast PID library designed for use on embedded systems (but can also run on any machine which has a G++ compiler).

For better performance and control, the PID library supports a generic number type. Number type must support casting to doubles. Fixed-point numbers are recommended for high-speed operation, doubles are recommended for non-time critical algorithms.

Relies on used calling `Pid.Run()` at a regular and fixed interval (usually in the milli-second range, via an interrupt).

Automatically adjusts `Kp`, `Ki` and `Kd` depending on the chosen time step (`Zp`, `Zi`, and `Zd` are the time-step adjusted values).

**DO NOT** try and make `Kp`, `Ki`, or `Kd` negative. This results in undefined behaviour.

### Smooth Control

Derivative control is only active when at least two calls to `Run()` have been made (does not assume previous input was 0 on first call, which can cause a huge derivative jolt!).

### Easy Debugging

You can print PID debug information by providing a callback via :code:`Pid::SetDebugPrintCallback()`, which supports method callbacks by utilizing the slotmachine-cpp library. 

The following code shows you how to assign a callback for debug printing.

```c++
class Printer {
	public:
		void PrintDebug(const char* msg) {
			std::cout << msg;
		}
};

Printer myPrinter;
Pid pidTest;

// Asign callback to Printer's PrintDebug function.
this->pidTest.SetDebugPrintCallback(SlotMachine::CallbackGen<Printer, void, const char*>(&myPrinter, &Printer::PrintDebug));
```

You can disable all debug info (to free up some memory space) by setting `cp3id_config_INCLUDE_DEBUG_CODE` in `include/Config.hpp` to `0`. The debug buffer size can be changed with `cp3id_config_DEBUG_BUFF_SIZE`, again in `Config.hpp`.

## Code Dependencies

====================== ==================== ======================================================================
Dependency             Delivery             Usage
====================== ==================== ======================================================================
`<cstdint>`            Standard C++ library Fixed-width variable type definitions (e.g. `uint32_t`).
MAssert                External module      Providing runtime safety checks against this module.
MUnitTest              External module      Framework for unit tests.
====================== ==================== ======================================================================

## Building

This example assumes you are running on a Linux-like system and have basic build tools like `gcc` and `make` installed, as well as `cmake`.

```sh
$ git clone https://github.com/gbmhunter/MFixedPoint.git
$ cd MFixedPoint
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Usage

```c++
// Create a PID object which uses double for all of it's calculations, inputs, and outputs.
Pid<double> pidTest;

main() {
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
TimerIsr() {
	// Read input
	input = ReadPin(2);
	// Perform one execution
	pidTest.Run(input);
	// Set output
	SetPin(3) = pidTest.output;
}
```

See `test/PidTest.cpp` for more examples.
	
## Issues


For known bugs, desired enhancements e.t.c, see GitHub issues section.
	
## Changelog

See `CHANGELOG.md`.
