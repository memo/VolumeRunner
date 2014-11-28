This the result of an intensive week of exploring real-time ray marching distance fields, procedural content and animation.
The whole scene is parametrically defined as distance functions and rendered in one single fragment-shader (no polygons involved).

A rock like figure can explore and modify a procedurally generated world.
Its gait is generated procedurally and complemented with motion capture data for additional animations. It can spew constructive or destrucitve magma which creates or destroys the terrain.

This code was written in under one week, in an extreme rush. It is messy with lots of irrelevant / out-dated code / experiments / tests lying around, commented out, or just not being used. Use with caution. 

Memo Akten [http://www.memo.tv](http://www.memo.tv), Daniel Berio [http://www.enist.org/](http://www.enist.org/) © 2014



Dependencies 
============
Built on openframeworks. HEAD as of 25/11/2014 with some minor modifications. Our modified version of OF can be found at [https://github.com/memo/openframeworks/tree/volumerunner](https://github.com/memo/openframeworks/tree/volumerunner)
(The main change from public OF is in [this commit](https://github.com/memo/openFrameworks/commit/876f135b0089338d631732ceb8f8de9ca4bae3e8), which allows passing of an arrow of Matrices as a uniform to a shader).

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
* w or UP: move forward
* a or LEFT: turn left
* d or RIGHT: turn right
* r: spew constructive magma
* f: spew destructive magma
* SPACE: toggle GUI (use with caution)
* l: load default settings (if you've messed with the GUI too much)
* F: toggle fullscreen (will be slow unless you have a good GPU)
* R: reset state

		
    
Contents
================

**addons**: Libraries and additional C++ code needed for the projects. These are included in the xcode projects as needed.
**VolumeRunnerApp**: Main app.


Acknowledgements
============

We were able to build the renderer in a short amount of time thanks to the many useful resources and code available online:

* http://iquilezles.org/www/
* https://www.shadertoy.com
* http://www.mazapan.se/news/2010/07/15/gpu-ray-marching-with-distance-fields/
* http://graphics.cs.williams.edu/courses/cs371/f14/reading/implicit.pdf

The motion capture data is from the Carnegie Mellon motion database, available in BVH format at [https://sites.google.com/a/cgspeed.com/cgspeed/motion-capture/cmu-bvh-conversion](https://sites.google.com/a/cgspeed.com/cgspeed/motion-capture/cmu-bvh-conversion).


License
================
This software is released under the GNU GPL v3. See included LICENSE file for details.

Additional supplied libraries are under their own licenses. Again see LICENSE file in relevant folders. 