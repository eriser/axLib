ReWire SDK 2.6.4
2012-12-04

New in version 2.6.4, compared to 2.6.3
* Fixed a struct alignment bug that was introduced with the 64 bit version of ReWire.
The bug could cause RWMGetControllerInfo() or RWMGetNoteInfo() to fail on Mac.

* Fixed a bug that could occur in rare cases when opening and closing host and device in a specific order.

* Updated project files for sample applications.

New in version 2.6.3, compared to 2.6.0
* Stopped mixed drive of idle from both ReWire mixer (host) and built-in thread. Now only built-in thread is used.

ReWire SDK 2.6.0
2010-10-28

New in version 2.6.0, compared to 2.5.3

* Added support for 64-bit architectures.

* The ReWire API now uses specific ReWire_ types to avoid 32- 64-bit problems and be binary compatible with clients made for ReWire 2.5.3.

* ReWire.dll should now be installed in Program Files/Common Files/Propellerhead Software/ReWire. ReWire.bundle should be installed in /Library/Application Support/Propellerhead Software/ReWire.

* Replaced RWPRegister/UnregisterDevice() device with RWPRegister/UnregisterReWireDevice() to avoid using old Mac FSSpecs.

* ReWire no longer supports CFM libraries.

* Added RWDEFAPI.h with function prototypes for ReWire devices.

* Changed sample apps into console apps with source code included in the SDK.




New in version 2.5.3, compared to 2.5.2

* The Mac OS version of the ReWire library now requires Mac OS X 10.3.9 or later to run. (It is now built using gcc 4.)

* The testing/debugging version of the ReWire library will no longer write to the log file (or the System Console on Mac OS) from any threads other than the main thread.

* Fixed a problem with RWDComDoesMessageFit and RWPComDoesMessageFit that caused them to indicate that a message fitted even if it did not due to not considering the space the message size used.




New in version 2.5.2, compared to 2.5.1

* Fixed a rare race condition, in which the built-in idle thread in ReWire would hang in a dead lock.




New in  version 2.5.1, compared to 2.5

* When using the testing flavour of the ReWire library on Mac, the debugging logs that it writes to are now created in the ~/Library/Logs/Propellerhead Software/ReWire Library/ folder. Under Windows, log files are created in My Documents\Propellerhead Logs\ReWire Library\.

* It has previously been undocumented what text encoding should be used when working with the ReWire library. (All that has been documented is that the strings are char arrays, not how they should be interpreted.) We now reccomend that all applications should use utf-8 encoding.
Note that since you cannot be sure what encoding the strings you receive from ReWire actually uses (since that depends on the application that gave them to ReWire, and not all applications will move to urf-8 overnight), you need to be careful when trying utf-8. There are lots of byte sequences (for example strings using many valid "classic" encodings used on Mac OS and Windows) that are invalid utf-8 sequences, so you need to be prepared for the conversion to fail, and then fall back to some other encoding. We recommand that you try to interpret strings you receive from ReWire as utf-8, and if that fails fall back on MacRoman on Mac and Windows Latin-1 on Windows.
Also, when sending strings to ReWire, be careful if you need to truncate a string to make it fit in ReWires fixed-size char arrays. Since utf-8 uses variable length characters (a single Unicode code point can use anywhere between one and four bytes in utf-8), you need to make sure that you truncate the string at a code point boundary, otherwise the string will become corrupted.
For more info on utf-8 encoding, see
http://unicode.org/reports/tr17/




New in version 2.5, compared to 2.4

* Mac version is now a MachO loadable bundle. Additionally, it is compiled as a Universal binary. There is also a CFM bridge library for compatibility with older ReWire applications. See the Mac notes below.

* [originally introduced in ReWire 2.4.1] ReWire will periodically call RWDEFIdle on its own from a separate thread, in order to ensure a steady rate of RWDEFIdle calls, even if the mixer application neglects to call RWMIdle often enough. The RWDEFIdle callback is protected with a mutex by ReWire, so your device never has to worry about concurrent/reentrant calls to RWDEFIdle.

* The Mac OS 9 version is no longer included in the SDK. If you need it, you can get it from an older version of the SDK.



Installation notes
When you install ReWire on an end-user system as part of your app's installation process, please remember to always check the version number of any already installed ReWire library. Only install your version if it is newer than the already-installed version.
On Mac OS X, install the ReWire libraries (both the MachO version and the CFM version) in Application Support/Propellerhead Software/ReWire/ (use FSFindFolder to find the application support folder). You can choose to install ReWire for all users (in the kLocalDomain domain) or for the current user only (in the kUserDomain domain). Additionally, place an alias to the CFM bridge library at the top level of the Application Support folder. If you are installing a ReWire Device, place an alias to your device in the Application Support/Propellerhead Software/ReWire/ folder.
Under Windows, install the ReWire library in the Windows\System32 directory. For installation of your ReWire Device please refer to the ReWire.doc documentation, section 8.1 Registering a Device.


Version checking
Note that the version number of the ReWire library isn't the same as the version number of the ReWire SDK. The current ReWire library is version 1.7b1 (which corresponds to SDK version 2.5b1).

On Mac, you can use CFBundleGetVersionNumber to get the version of the MachO ReWire library. For the old CFM bridge library, just load the vers ID 1 resource.
Remember to take all fields in the version number into account when comparing versions. Use the algorithm described in Apple's versioning tech note:
http://developer.apple.com/technotes/tn/tn1132.html#ComparingVersionNumbers
For example, 2.2b1 is newer than 2.2a4, and 2.2f0 is newer than 2.2f1 (read the tech note).
If you find an alias to a ReWire library in Application Support and/or the ReWire subfolder, always follow the alias and check the version of the original!

Under Windows, use windows API functions GetFileVersionInfoSize, GetFileVersionInfo and VerQueryValue to retrieve a VS_FIXEDFILEINFO structure. Retrive the revision information like this:
WORD major = HIWORD(lFInfo->dwFileVersionMS);
WORD minor = LOWORD(lFInfo->dwFileVersionMS);
WORD revision = HIWORD(lFInfo->dwFileVersionLS);
WORD build = LOWORD(lFInfo->dwFileVersionLS);
Remember to take all the fields into consideration when comparing versions, "major" is more significant than "minor", "minor" is more significant than "revision" and finally "revision" is more significant than "build".



Mac notes
The ReWire library is now implemented as a MachO loadable bundle. The file name is "ReWire.bundle" and the bundle identifier is se.propellerheads.rewire.library.
There is also a CFM version of the library. It is included for backwards compatibility with applications compiled with older versions of the SDK. The CFM version is a bridge library; it doesn't actually implement any of the API functions itself, it just opens the MachO library, and calls through to the implementations there. The file name of the CFM bridge library is just "ReWire" (same as the name of the ReWire library in previous versions of the SDK).
The glue code has been updated to look for the MachO version of the library. (If it cannot find the MachO version, it will fall back on the CFM version.) The glue code can be compiled into both CFM and MachO applications; it will take care of any necessary thunking as needed.

ReWire devices can be packaged as either CFM shared libraries or MachO loadable bundles (which can be Universal, if desired). The same functions should be exported in both cases. CFM devices must have the creator code “ReWi” and the file type “shlb”. MachO devices don't need to have any special type/creator.
Devices should be installed in Application Support/Propellerhead Software/ReWire/ folder. Aliases are fine.

The ReWire requires Mac OS X 10.2 to run. Most of the test apps require Mac OS X 10.4 (they might run under 10.3, but that is untested). All the binaries included with the SDK are Universal.

There are Xcode project files included for the samples. The CodeWarrior project files have been removed.




Version history for 2.5:

Changes from beta 1 to release version
- [Mac PowerPC only] Fixed a possible crash problem when an application using ReWire quits if any C++ exceptions have been thrown inside the ReWire library this session. Fixed by not CFRelease:ing the ReWire library when it is closed by the glue code.
- Updated MixerApp to display MIDI busses and channels in a way that more corresponds to the way the docs say you should do it.

Changes from alpha 4 to beta 1:
- Now includes the Windows version and the rest of the test apps that were previously missing.
- The bundle identifier for the Mac ReWire MachO library has changed from se.propellerheads.rewire to se.propellerheads.rewire.library.
- Fixed a problem in the code that looks for devices that would sometimes cause a crash when resolving aliases in the Application Support folder.

Changes  from alpha 3 to alpha 4
- Alpha 3 didn't work on OS versions prior to Mac OS X 10.4. Now it should work all the way back to 10.2.
- A byte-swapping problem of file versions in the glue code has been fixed.

Changes from alpha 2 to alpha 3:
- Fixed a crashing problem with CFM devices. Any combination of CFM and MachO devices and applications should now work.
- Added ReWire Diagnose application
- Added TestPanel and TestDevice
- Included "testing" versions instead of "debugging" version of the compiled code. The main difference is that the symbolic info is now stripped from the binaries, so they are a lot smaller now.
- ReWire will now sometimes fake calls to RWMIdle from a separate thread. This is because some hosts doesn't always call RWMIdle often enough. This means that ReWire devices can now receive calls to RWDEFIdle even when the mixer app isn't calling RWMIdle. Also, those "extra" calls to RWDEFIdle occurs from a secondary thread.

Changes from alpha 1 to alpha 2:
- Device registration is now more robust. If ReWire encounters any errors when examining a (possibly) device, it just skips to the next one.
- The pipes used for inter-process communication now work better. In the previous release, there were problems with the locks used to protect the shared memory where the pipes "resides".
- The Mixer app included with the SDK now calls RWMSetAudioInfo when opening a device.
- A CFM version of the ReWire library is now included with the SDK. This is just a bridge library that calls through to the implementation in the MachO library. This way, ReWire will work both both old apps (that expects the CFM library) and new apps (that uses the MachO library, if available).
- Used the proper version number for the SDK (2.5). (The first alpha incorrectly used the version number of the ReWire library as the version number of the SDK.)