-------------------------------------------------------------------------------
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	README.txt
	Some useful information about your animal3D distribution.
-------------------------------------------------------------------------------

Welcome to "animal3D", the minimal 3D animation framework! This file has some 
useful information about developing with animal3D.

Contents (or, TL;WR): 
1. Disclaimer
2. Distribution
	A: Edition
	B: Structure
3. Platform-specific information
	A. Windows
		i.	 IDE: Visual Studio
		ii.  Dependencies
		iii. Notes
	B. Future Platforms
4. Features
	A. Windowing
	B. Hot Build & Load Demo Project
	C. Input
	D. Utilities
	E. Graphics & Rendering
	F. Animation & Physics
5. Using the animal3D Framework (Windows)
	
	
-------------------------------------------------------------------------------
1. Disclaimer

As per the license (Apache 2.0), this distribution of animal3D comes as-is and 
without warranties or conditions of any kind, either express or implied. The 
framework may be partially closed-source to support educational use. Any re-
distribution of the framework in any form MUST include the licence and notice 
files provided, and whatever credit is due to contributions by anyone other 
than the original author of the framework.


-------------------------------------------------------------------------------
2. Distribution

	A: Edition
This distribution is the Educational Edition of animal3D. Some of the features 
have been removed and/or closed-sourced for the following reasons: 
	- The point of using the framework is to develop and test animation 
		algorithms and techniques; some of the built-in features are used for 
		demos that are provided with this distribution, but should serve as 
		guidelines for new features, additions and extensions to the framework.
	- There is a lot that goes on behind the scenes that does not directly 
		pertain to animation programming (e.g. graphics); these supplementary 
		features are here to help get you off the ground, but are hidden to 
		avoid getting distracted, lost or overwhelmed.
Closed-sourced features may or may not be opened in a future educational 
distribution.

	B: Structure
This is the directory structure of the animal3D framework: 

----------------------------------------+--------------------------------------
	"Directory/" or "FILE.ext"          |	Description
----------------------------------------+--------------------------------------
-animal3D-SDK/ (root)                   | Root directory
	-LICENSE.txt                        | Apache 2.0 licence file
	-NOTICE.txt                         | Apache 2.0 notice file
	-animal3D-SDK/                      | (**you are here**)
		-README.txt                     | (**you are reading this file**)
		-CONFIG_VS_RUN-AS-ADMIN.bat     | Visual Studio config (**SEE BELOW**)
		-LAUNCH_VS.bat                  | Visual Studio launcher (**MUST USE**)
		-PACKAGE_VS.bat                 | Packaging utility (see below)
		-include/                       | Header files
			-animal3D/                  | Core headers and inline files
			-animal3D-A3DG/             | Graphics headers and inline files
			-animal3D-A3DM/             | Math headers and inline files
		-project/                       | Project and solution files
			-VisualStudio/              | Visual Studio projects and solutions
				-_SLN/animal3d-sdk/     | A3 solution (**USE LAUNCHER ONLY**)
				-animal3D-A3DG-OpenGL/  | A3 OpenGL graphics project
				-animal3D-DemoPlugin/   | A3 demo plugin project (**WORKING**)
				-animal3D-DemoPlayerApp/| A3 demo player app project (window)
		-resource/                      | Loaded assets of all sorts live here
			-animal3D-data/             | Data for playing animal3D demos
		-source/                        | Source code lives here
			-animal3D-A3DG/             | Graphics source files
				-a3graphics/            | Renderer-agnostic source
				-a3graphics-OpenGL/     | OpenGL-specific source
			-animal3D-DemoPlugin/A3DEMO/| Demo source **YOUR CODE GOES HERE**
			-animal3D-DemoPlayerApp/    | Player app (window) source
		-utility/                       | Developer utilities
			-win/animal3D-VSSetPath/    | Utility for locating missing VS path
			-win/dev/                   | Windows utilities
----------------------------------------+--------------------------------------

	
-------------------------------------------------------------------------------
3. Platform-specific information

	A. Windows
		i.	 IDE: Visual Studio
The supported IDE for Windows is MICROSOFT VISUAL STUDIO (VS or MSVS).
The supported editions are 2019 (16, v142), 2017 (15, v141), 2015 (14, v140), 
and 2013 (12, v120).

		ii.  Dependencies
The animal3D demo project has ONE (1) external dependency, and uses one or 
more of the components therein: 
			1. DEV-SDKs: Developer SDK collection
				- A collection of useful libraries
				- animal3D uses DevIL (Developer's image library), and GLEW 
					(OpenGL Extension Wrangler)		

		iii. Notes
Update since last version: DLL hotbuild should now work in VS2017 and VS2019.

	B. Future Platforms 
A future version of the framework will include the necessary means to build 
animal3D projects using Xcode on Mac OSX.
Update: Support for Mac and Xcode has been started. Contributions by Colin 
Brady, 2018, will hopefully be integrated soon.
	
	
-------------------------------------------------------------------------------
4. Features

	A. Windowing
		i.	 Window with OpenGL rendering context and input: 
		
The main window of any animal3D demo is a built-in feature. The player app's 
only real job is to initialize this window so that a demo can be loaded into 
it. The window has a File menu with options to load demos.

		ii.  Console
animal3D runs off of a windowed application, not a console application; an 
output-only console is available in Debug mode only as a way to catch errors 
or warnings thrown by the library itself or external libraries.
	
	B. Hot Build & Load Demo Project
The backbone of the windowed application is the ability to hotbuild your demo 
into it without having to consistently close the window and recompile. We all 
know this process gets tedious when we only need to change one line of code 
and then we have to play through half the game to see if the change worked. 
With animal3D, these tiny changes are made much more efficient by keeping the 
demo code entirely separate from the windowing code. Now you can run the app, 
hotload the working debug project, and not need to close the window because 
one little thing looks off... just change it and hit the load button again, 
and your application should keep going from where you left off.
NOTE: this does not work if your memory layout changes, so be careful! It's 
really just a way to get around long wait times for tiny logical changes.
	
	C. Input
The window is equipped with callbacks for mouse and keyboard input. Your demo 
can hook itself into these callbacks if you prepare appropriate functions to 
do so. See section 5 for info on how this works. The callbacks are triggered 
events, so the framework also has data structures to store and update keyboard 
and mouse states as persistent members of your demo. Additionally, animal3D 
has Xbox 360 controller support built-in; however, this is not hooked directly 
into the window, so you'll need to keep track of everything on your own.
	
	D. Utilities
		i.	 File and string streaming
The framework provides the means to quickly copy data to and from strings and 
files. Example use: first run, read an OBJ file and have it stream to a binary 
file; if the file exists, load it instead of reading the OBJ again.

		ii.	 High-precision timer
Probably one of the most important things you could have when dealing with 
graphics and animation. Set any number of timers to manage different time 
intervals for whatever reason. Prime example: a render timer in the idle 
function that controls when draw calls actually happen.

		iii. Threading
Start a new thread just like that. It is often useful to delegate different 
types of engine activities that take a lot of time to complete to different 
threads, such as graphics... or animation.
	
	E. Graphics & Rendering
		i.   Buffer objects
A general CPU-side interface for a general container of data on the GPU.
			1. Vertex and index buffers
				Organized vertex buffers store vertex data, while index 
				buffers store indices that tell the GPU the order in which to 
				draw vertices (see drawables).
			2. Uniform buffers
				Uniform buffers can be used to store whole uniform structures 
				or groups of individual shader uniforms (see shaders).
				
		ii.  Vertex arrays and drawables
Vertex arrays are used to keep track of a vertex format on the GPU. They 
describe the attributes contained in a vertex buffer and their relative 
positions in the buffer. It's basically a drawing manager. To supplement this, 
the drawable feature provides a "description" of a section of a vertex buffer 
and an optional index buffer that should be drawn using a vertex array.
			1. Geometry utilities
				The framework provides some common utilities that make putting 
				together renderable shapes a little more pleasant, such as 
				buffer sharing and measuring space requirements.
			2. Procedural geometry
				Need a basic 3D primitive but don't have a model? No problem. 
				Create geometry fast with a wide array of fun 3D shapes, such 
				as spheres, capsules and octahedrons. You can also choose the 
				vertex attributes to be generated with each shape.
			3. Loaded models (OBJ)
				Have a model? No problem. If it's a triangulated or quad mesh 
				in the Wavefront OBJ file format, you can load it in with the 
				option to generate tangents and skin weights along with the 
				basic data that comes with the OBJ format.

		iii. GLSL shaders and programs
GLSL shader compilation and program linking support is built-in. You can write 
shader code directly in your C/C++ source, or load it in from a text file (or 
multiple files) and have it compiled into a GLSL shader. Attach one or more 
shaders to a program object, and link it to build your own vertex pipeline.
		
		iv.  Textures and material descriptors
A simple texture interface allows you to create OpenGL textures from raw data 
or load them from image files using DevIL. One or more textures can be active 
at a time by binding them to OpenGL's "texture units". To further facilitate 
the relationship between textures and shaders, the material interface makes 
use of uniform buffers to send material data to shaders.
		
		v.   Framebuffer objects
In case you feel the need to do any post-processing, animal3D is here for you. 
Another fun feature is the double-FBO, which acts like a normal double buffer 
but off-screen so you can post-process without needing to manage an abundance 
of different render targets.
		
		vi.  Text overlay
Finally (for now), it is possible to draw text directly to your render window, 
either as an overlay or as part of the scene itself. Create a text renderer 
object and use it to draw text at a given normalized device coordinate.
	
	F. Animation & Physics
Using the powers of all the other things combined, you are destined to learn 
and discover this part on your own.
	

-------------------------------------------------------------------------------
5. Using the animal3D Framework (Windows)

First time using the framework: 

1.	Close all instances of Visual Studio.
	
2. 	Make sure you have DEV-SDKs on your computer and have run the install 
batch file therein. **YOUR FRAMEWORK WILL NOT WORK IF YOU DON'T DO THIS!**

3.	To open the framework, run the batch file called "LAUNCH_VS.bat" in 
the root folder.
	

For all uses of the framework: 
	
To open the framework, follow step 4 above.

Known issue starting with Visual Studio 2017+: The VS2017+ installer does not 
set an environment variable for a path to the install directory, which was 
extremely helpful with earlier versions. Therefore there is a tool to help 
locate VS2017+. Thanks Microsoft for this silly change. A minor inconvenience; 
this one-time fix should help: 
If at any time the launcher does not work (e.g. nothing happens or a console 
opens and doesn't go away), navigate to the root directory and run the file 
CONFIG_VS_RUN-AS-ADMIN.bat **AS AN ADMINISTRATOR**. When Visual Studio opens, 
simply build the project (while running as admin) and the project will force 
Visual Studio to set a path to itself, permanently. Close VS and the animal3D 
launcher should function as expected after this. Try the launcher again.


If any Intellisense problems occur (red lines that won't go away despite 
there being no errors), close the problem instance of Visual Studio and 
navigate to the solution directory and delete the Intellisense database 
file (.db): 
	"./projects/VisualStudio/_SLN/animal3d-sdk/animal3d-sdk.VC.db"
Delete the Intellisense database (.db) file here. **IF YOU ARE USING VERSION 
CONTROL, DO NOT COMMIT THIS FILE!**


The solution contains THREE (3) projects: 
	-animal3D-A3DG-OpenGL: This is the open-source animal3D OpenGL renderer.
	-animal3D-DemoProject: This is your back-end, hotloadable library. All of 
		your development takes place in this project; feel free to organize 
		your code files however you please.
	-animal3D-LaunchApp: This is your front-end application that links your 
		demo library on the fly.
	
There are no required file names or paths for your hotloading library, however 
you will need to fill modify the callbacks configuration file, found here: 
	"./resource/animal3d-data/animal3D-demoinfo-debug.txt"
A full explanation of callback hooks, with an example, can be found here: 
	"./resource/animal3d-data/animal3D-demoinfo-example.txt"

Always press F7 to build the solution, then press F5 run the launcher app and 
open a window; if you do not build first, your libraries may not update.

**DO NOT CHANGE ANY PROJECT SETTINGS** other than to add dependencies.

To prepare a package of your demo (e.g. redistributable), use the batch file 
"PACKAGE_VS.bat" in the root folder; just enter a name and it will copy all of 
the pertinent files to a new folder. Zip it up and you're golden!

	
-------------------------------------------------------------------------------

That's the gist of it... good luck, have fun, and learn lots!  :)
-Dan


-------------------------------------------------------------------------------
