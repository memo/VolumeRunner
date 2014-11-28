
This code was written in under one week, in an extreme rush. It is messy with lots of irrelevant / out-dated code / experiments / tests lying around, commented out, or just not being used. Use with caution. 

Memo Akten, Daniel Berio © 2014



Dependencies 
============
Built on openframeworks. HEAD as of 25/11/2014 with some minor modifications. Our modified version of OF can be found at [https://github.com/memo/openframeworks/tree/volumerunner](https://github.com/memo/openframeworks/tree/volumerunner)

Other dependencies are included in this repo (under **addons**).


Compatibility
=============
Developed and tested on OSX (10.9.5). Only XCode projects are provided, but it should compile and run on Windows and Linux too provided the projects are setup correctly with dependency header search paths. 

Requires a good graphics card! (Developed and tested on Macbook Pro Retina with Geforce 650M)



Compilation instructions
========================
* Clone or download the **openFrameworks** repo, **volumerunner** branch from
[https://github.com/memo/openframeworks/tree/volumerunner](https://github.com/memo/openframeworks/tree/volumerunner)
* Clone or download this repo into its own folder under the openframeworks/apps folder. I.e.:

		openframeworks
		__apps
		____VolumeRunner <-- this repo goes in here.
		______.git
		______addons
		______VolumeRunnerApp
		etc.
		

Keys
============
* w or UP........: move forward
* a or LEFT......: turn left
* d or RIGHT.....: turn right
* r..............: spew constructive magma
* f..............: spew destructive magma
* SPACE..........: toggle GUI (use with caution)
* l..............: load default settings (if you've messed with the GUI too much)
* F..............: toggle fullscreen (will be slow unless you have a good GPU)
* R..............: reset state

		
    
Contents
================

**addons**: Libraries and additional C++ code needed for the projects. These are included in the xcode projects as needed.
**VolumeRunnerApp**: Main app.


Acknowledgements
============
A lot of GLSL code borrowed from Inigo Quilez, [http://www.iquilezles.org/](http://www.iquilezles.org/)







