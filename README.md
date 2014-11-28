This code was written in under one week, in an extreme rush. Use with caution

Memo Akten, Daniel Berio © 2014

(A lot of GLSL code borrowed from Inigo Quilez, [http://www.iquilezles.org/](http://www.iquilezles.org/) )

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
		
		
    
Contents
================

**addons**: Libraries and additional C++ code needed for the projects. These are included in the xcode projects as needed.
**VolumeRunnerApp**: Main app.







