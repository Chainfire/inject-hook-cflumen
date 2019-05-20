# *CF.lumen* *performance* driver

Source code for *CF.lumen*'s *performance* driver. While that might be
useful in itself, this release is really about the injection and
hooking code, with the *CF.lumen*-related code serving as an example of
this.

Library injection and function hooking with support for:
- arm32
- arm64
- x86
- x86-64
- Android 4.0 - 9.0

## License

Please see the [LICENSE](LICENSE) file for the exact details.

In summary:

Based on Simone *evilsocket* Margaritelli's [ARM Inject](https://github.com/evilsocket/arminject) (&copy; 2015, BSD 3-clause).

Modifications and additions by Jorrit *Chainfire* Jongma (&copy; 2015, BSD 3-clause).

Excerpts from The Android Open Source Project (&copy; 2008, APLv2).

Credits are always appreciated if you use my code.

## Spaghetti Sauce Project

This release is part of the [Spaghetti Sauce Project](https://github.com/Chainfire/spaghetti_sauce_project).

## About

*CF.lumen* is an Android (root) app that changes the colors on your
screen, its original purpose was primarily to save battery on AMOLED
devices, provide a red-only night mode that didn't destroy your night
vision, and to provide *f.lux*-like functionality on Android. It has
existed in various forms since the Android 2.x days.

In the 2.x days, Chainfire3D hijacked Android's entire GL rendering
pipeline, by proxying the EGL and GL libraries. Some of its purposes
were to run apps and games written with support only for specific GPUs
to work on all the phones, and tweak shaders and select graphics
settings. *CF.lumen* (CF3D Night Mode) was an add-on to that, tapping into
CF3D's plug-in system.

Chainfire3D never made it into the 4.x era, as apps and games stopped
being GPU-specific and started supporting most (if not all) of the
hardware out there. The most used graphics tweaks (forcing MSAA for
example) also became options in Android's developer settings, and the
need to tweak individual shaders almost disappeared as well. *CF.lumen*
was released from its Chainfire3D dependency and re-implemented as
a library LD_PRELOAD'd into SurfaceFlinger, which required modifications
to /system.

On 5.x and beyond, root itself moved away from modifying the /system
partition, and apps soon followed wherever possible; *CF.lumen* was no
exception. Since LD_PRELOAD could no longer easily be made to work,
another solution had to be found. The answer was injecting the *CF.lumen*
library into SufaceFlinger directly, and manually hooking the required
(E)GL functions rather than letting the linker doing it for us (as was
the case with LD_PRELOAD).

*evilsocket*'s [ARM Inject](https://github.com/evilsocket/arminject)
provided the base code necessary to perform this hijack. I restructured
the code to suit my needs, and added arm64, x86 and x86-64 support.
Over time, linker structures were adjusted to reflect changes in
Android, while maintaining compatibility with older versions. It took
quite a bit of effort to get all that working semi-reliably.

Android itself also gained functionality to apply color matrices to
the rendered surfaces around this time, but the early implementations
were not very efficient. Support for that functionality was named the
*compatibility* driver, as though it was not the fastest, it was
supported on all devices. The code released here was named the
*performance* driver as it was significantly faster on many devices
*when it worked*, though on some devices it just didn't.

## Sources: Injecting/hooking

The injecting and run-time hooking code is fairly well separated
from *CF.lumen*'s specific code. See [inject_main.cpp](inject_main.cpp)
and [libinject](libinject) for the injector code, and
[hook_main.cpp](hook_main.cpp) and [libhook](libhook) for the hooking
code.

Comments are sparse, but if you're messing with injecting and hooking,
you should be at the level where you can read and understand the code
itself. If not, this is the time to learn.

Update: I elaborate some more on how the injector works in the
[injectvm-binderjack repo](https://github.com/Chainfire/injectvm-binderjack).

## Sources: *CF.lumen* *performance* driver

As stated earlier, this serves more of a working example on how to
use the injecting and hooking functionality than that the code is
particularly interesting by itself, but (skipping the injecting and
hooking parts), here's how it basically works:

- Android is forced to use GPU compositing: individual surfaces are
rendered to the final framebuffer via GL calls (GPU resources used), 
rather than passing the individual surfaces to the display hardware
that does this for you (essentially for free). Forcing GPU compositing
is done by other parts of *CF.lumen*, it is not part of this code.

- The driver hijacks the fragment shaders Android uses for this 
compositing stage, and injects code into them to reference our
color matrix.

- In the final compositing stage, these modified shaders are applied
and the surfaces are shaded with the configured color matrix.

## Why release this?

I have been less involved with (root) Android development for quite
some time now, and I am not aware of the existence of any other 
injecting/hooking code for Android with this level of compatibility. 
It was always my intention to release these modifications of 
*evilsocket*'s work to the public, but "life happens" and I didn't 
get around to it. It would be a shame for the work to be lost, I'm
sure it could be useful to others.
 
At the same time, while the first Android internal implementation to
apply color matrices was somewhat inefficient and also used GPU
compositing, it has become more efficient in recent releases. On the
latest devices, if the hardware supports it, it is even done without
GPU compositing at all, directly in the display hardware (essentially
for free, and also works with protected surfaces). The need for the
*performance* driver in *CF.lumen* is thus dwindling quickly, and the
benefits for me to keep the source to myself are disappearing. This
conveniently saves me the time of having to write an example project
for the usage of the injecting/hooking code as well (which really is
what ultimately triggered the release of the code at this time).

Besides, recent Android versions (and before that, several OEMs) now
have basic capabilities to provide *f.lux*-like sundown modes, reducing
or eliminating the need for *CF.lumen* itself for many users. Of course
there's always a group of users that likes to have *CF.lumen*'s extended
functionalities, but there's no denying *CF.lumen*'s best days are
behind it.

## Compatiblity

While all this has been heavily tested on 5.x-7.x, only basic testing
has been done on 8.x and 9.0.
