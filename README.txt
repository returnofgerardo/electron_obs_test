# streamlabs-project
-Building-  
prerequisistes:  
    CMake 3.28 and a C++ compiler toolchain  
    Node.js  
    Internet connection  

steps:  
    Clone this repo using  
    'git clone --recurse-submodules <github-url>'  
    or do a normal clone and run  
    git submodule update --init --recursive  

    Follow obs build instructions for your platform https://github.com/obsproject/obs-studio/wiki/Install-Instructions  
    inside the mini-obs-module\libs\obs-studio directory. Build as Release  
    Run 'npm install' from project root folder  

-Running-  
Call 'npm start'  

Press the OBS Startup button to run obs_startup() in libobs  
The Reset Video button calls obs_reset_video(). This "succeeds", but the graphics thread it spawns fails to load a needed module 

