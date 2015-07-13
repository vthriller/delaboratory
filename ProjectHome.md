
---


## delaboratory is an unique image postprocessing application ##

[Philosophy](Philosophy.md) - design philosophy

[Screenshots](Screenshots.md) - screenshots of last stable version (0.8)


---


**2013.10.20 - prototype of new delaboratory**

Simple prototype of next generation delaboratory has been released. It uses **Qt** and it shows in which direction development is heading.

[Prototype20131020](Prototype20131020.md) - detailed description and link to files

(Prototype is **only for people interested in development status**, end-users should use stable version instead)

questions, bugs and other issues: delaboratory.project@gmail.com



---


**2012.10.08** - skin enhancement tutorial

KOVÁCS István wrote detailed article about using delaboratory 0.7 (last version with blend masks) for skin tone enhancement

http://tech.kovacs-telekes.org/content/using-delaboratory-cmyk-skin-tone-enhancement


---


delaboratory 0.8 features:

  * **floating point precision** (not 8-bit like Gimp, not 16-bit like most raw processors)
  * native support for **multiple colorspaces** (sRGB, ProPhoto RGB, LAB, CMYK, LCH, HSL, HSV, BW)
  * **non destructive editing** (adjustment layers)
  * **KISS** design
  * realtime preview
  * basic operations: CMYK curves, CMYK levels, RGB curves, RGB levels, white balance in LAB, saturation in LAB, LAB curves, BW curve, HSV equalizer, LCH equalizer, recover shadows/highlights, vignette, local contrast, LAB/BW/RGB/CMYK gradients
  * expert operations: levels, curves, equalizer, exposure, saturation, white balance, fill, tone, local contrast, auto\_dodge/auto\_burn, sharpen, vignette, gradient, gaussian blur, recover highlights, recover shadows, channel mixer, high pass, c2g, copy
  * RAW support via dcraw
  * up to 5 color samplers
  * histograms and color matrices
  * many blend modes (multiply, screen, overlay, inverted overlay, dodge, burn, add, grain extract, grain merge, sub, difference, darken, lighten), color/luminocity blending for RGB and CMYK

limitations:

  * delaboratory doesn't support any local modifications (...yet), like painting, clone tool, healing tool or true dodge/burn, you should perform such operations in other application like Gimp
  * delaboratory doesn't support color profiles at all, RAWs are loaded in ProPhoto RGB, but display works in sRGB and output will be sRGB, so if your style of photography requires wider colorspace than sRGB then delaboratory is not a tool for you
  * delaboratory uses lots of memory, 32-bit color means that each pixel in RGB uses 12 bytes, multiple layers means that each image is stored multiple times in memory, so 36-megapixel photo (like the one from D800) will be stored in multiple 432MB buffers, while 32-bit memory limit is enough for 12MP photos, it is recommended to use 64-bit system for large images

why use delaboratory?

  * you don't have to worry about quality loss during your postprocessing, it's perfectly fine to use for instance 10 different curves in different colorspaces, move between RGB, LAB and CMYK - in Gimp even one curve destroys the quality, because it's 8-bit color depth, in most RAW editors I've seen you can use 16-bit or 32-bit curve but only once, users of commercial software are afraid of converting between RGB/CMYK/LAB because they believe it means quality loss, that's why delaboratory uses 32-bit and adjustment layers

  * it's an utility made for experiments, to play with colors, to learn new techniques, if you multiply some simple actions (curves, mixer, vignette, etc) by number of colorspaces by number of blend modes, you will realize how many ways you can go, this is totally different approach than in RAW editors, when you can only set this and that

  * it's free and it will be free forever, it compiles and works on multiple operating systems - Linux, Windows, Mac, BSD


---


tips:
  * CMYK curves (and levels) are like RGB curves but much much better, it's best way to work on portraits
  * RAW files are loaded with reduced contrast to avoid clipping, you can change that by moving slider in source image layer (it's set to 0.9 by default), but you don't need it, you will maximize contrast with curves/levels anyway (and there is no quality loss, everything in delaboratory work on floats), you can also manually open RAW file in sRGB or with auto-brighten enabled
  * you can move vignette center and gradient position by clicking on image
  * you can auto set white balance by clicking on image (when editing wb layer)
  * always try to reduce opacity, experiment with blending modes
  * try exporting all layers, so you could choose best one later
  * you can edit any effect at any time, even when you put 10 different curves you can still fix the first one and the result will be updated in realtime
  * for glamour postprocessing - go to CMYK curves, choose with mouse light skin area then press "A" key or "G" key or "H" key for glamour skin presets, use "reset" when you mess it up
  * other keys for curve editor are "space" for adding new point in all channels, and "x" to remove last selected point
