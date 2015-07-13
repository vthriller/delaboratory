# Introduction #
Delaboratory 0.8 for Windows comes in two flavors - 32-bit and 64-bit. We should caution you that some of the new cameras like the Nikon D800 produce enormous images (the 14-bit uncompressed loss format NEF is 75Mb) and so you may have some trouble in the 32-bit world. However, in most cases you should be fine.


# Details #

You will notice that Delaboratory 0.8 now offers 4 methods to open a RAW file - ProPhoto RGB or sRGB, each with the option to auto-brighten the image.

![http://www.partha.com/images/screenshots/Delaboratory/screenshot-01-s.jpg](http://www.partha.com/images/screenshots/Delaboratory/screenshot-01-s.jpg)

You can work in different color spaces which has always been the strong point of Delaboratory. Here you see LAB color corrections being applied to an image.

![http://www.partha.com/images/screenshots/Delaboratory/screenshot-02-s.jpg](http://www.partha.com/images/screenshots/Delaboratory/screenshot-02-s.jpg)

The other window shows you how to increase the local contrast of an image, something similar to a pseudo-HDR tone-mapping.

Once you have done all your color corrections, it is time to do any remaining pixel level editing that you see fit.

In this version, Delaboratory has introduced an option to use any Windows photo editor that your heart desires. For example, if you like Paint.Net, then you can export from Delaboratory to that application

![http://www.partha.com/images/screenshots/Delaboratory/screenshot-03-s.jpg](http://www.partha.com/images/screenshots/Delaboratory/screenshot-03-s.jpg).

To do this, you need to define an environment variable called GR\_EDITOR in your Windows system. This variable must be set to the full path of the application's main executable.

Of course, if this is all too complicated (and I don't blame you if you think so), you can continue to use Gimp as your pixel editor.

![http://www.partha.com/images/screenshots/Delaboratory/screenshot-05-s.jpg](http://www.partha.com/images/screenshots/Delaboratory/screenshot-05-s.jpg)

If you have already installed Gimp from www.partha.com, then you don't need to do anything to "send to External Editor" from Delaboratory. For all other editors, including Gimp installation from Gimp.org, I am afraid you will have to set the environment variable to that applications main executable.