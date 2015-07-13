# Introduction #

This is an **early prototype** of new delaboratory.

If you want to edit your photos with delaboratory - use stable version. This prototype is **not ready in any way for an end user**.

It **lacks essential features** like curves, vignette or local contrast.
I implemented only **two layers** (think "effects" or "filters") just to show an interface.

This prototype was released for public to show Qt interface, to make sure Windows version works and to announce that project is not dead.

Code is **not in SVN** yet, I am not going to put it there until core will be finished, my initial plan was to show only **Windows binary**, but I decided to give you also **source code** in zip.

![http://delaboratory.googlecode.com/svn/trunk/screenshots/delaboratory_prototype_2013.10.jpg](http://delaboratory.googlecode.com/svn/trunk/screenshots/delaboratory_prototype_2013.10.jpg)

# features and user interface #

  * in the older releases it was possible to save a layer stack then load it later, now it will be posssible to **store multiple snapshots** for given image and also build **library of presets** to use with all images, initial release (prototype) contains window with snapshots (bottom left), try it

  * I don't want to support mutiple image management (like Lightroom library), because it's a huge task and delaboratory should be simple editor (KISS!), however, what I really like is saved state of each edited photo, it leads to some kind of library of snapshots, currently you can notice that after quit and restart you will see same set of snapshots and same loaded image

  * layer types are now in upper left window (prototype contains only two example layers), when you double click it - layer will be added to stack

  * layer controls are now stored in the right panel (not in separate windows), however I am not really sure what to do with curves yet, so it may change later

  * delaboratory processes scaled-down previews, just like before, but now it always uses full images, not crops, this leads to two effects in zoomed view:
    * it's possible to quickly move image with mouse
    * processing is slower (it will be fixed in the future, needs some tricks with threads - process visible rectangle first, then other rectangles later)

  * masks and local modifications are still open issues, the best solution I have seen is in Nik Software Suite (now owned by Google), but I also like the Lightroom way

  * blending multiple images is not available in prototype, but I will implement it to support textures (film grain, painted vignettes, dust/scratches, etc)

  * G'MIC support is not ready yet, expect it in first real release of new delaboratory

  * prototype opens RAW files via dcraw and 8-bit image files via Qt (16-bit TIFFs are not yet supported), export writes JPEG only, stable release will support reading/writing 16-bit TIFFs (just like each previous release)

  * there is no limit of layers on stack, but currently each layer will make processing slower, older versions solved this problem by keeping all processed images in the memory, now I want to avoid it (D800 produces 36 megapixel photos and delaboratory stores them in 32-bit quality) - this is another open issue in the delaboratory core

# programming details #

  * **delaboratory now uses Qt**
    * the main reason to use wxWidgets in the past was portability, however there were multiple problems with Windows version, mostly because I developed software under Linux, and Windows port was made only for releases
    * the obvious choice for graphics editor was GTK+, because it's made for GIMP, and it contains lots of cool features, it's simple and it's fast, I was trying to use it, and it worked pretty good, however, I was using Linux again, when I wanted to use GTK+3 in Windows I realized it's not easy at all (one need to build multiple libraries from source) and if it's not easy for me, it won't be easy for users, and nobody will be able to compile delaboratory for Windows
    * I use Linux on desktop since 1998, but I always avoided programming in Qt for two reasons
      * applications using Qt are usually slow and bloated
      * Qt uses moc preprocessor and I always thought it's "dirty"
    * in 2013 I decided to give Qt a chance, soon I realized that Qt is mature, stable, rich-featured, well documented and really portable
    * to develop with Qt in Windows, one need to download **single file**, which contains not just library but also compiler and IDE, the only problem is lack of static build, but I decided to skip this problem and just put dynamic libraries into binary archive (that's why Windows binaries will be so huge)
    * of course you don't need to download Qt to use delaboratory, in Windows you will get Qt libraries in the binary release, in Linux Qt is provided by your distro

  * **dcraw is now compiled-in**
    * author of dcraw recommends using it as separate process, but everyone compiles it in, for all previous releases of delaboratory I tried to use it as separate process, but I no longer see benefits of this solution, I see just problems, so I put dcraw.c into source code just like everyone else

  * project stays GPL
    * **delaboratory is a Free Software** and I have no plans to change that

# Download #

**unfortunately google code does no longer support project downloads and tells developers to upload files on google drive, which does not work very well (strange user interface and no statistics), so if I won't find a better solution I would have to move the project to another website**

Windows binary and source code (google drive):
https://drive.google.com/folderview?id=0B-0o_299rntmNlhzZ21kdm8zczg&usp=sharing

Instruction for Windows:
  * download huge binary from link above
  * unpack it
  * run delaboratory application (there is no installer)