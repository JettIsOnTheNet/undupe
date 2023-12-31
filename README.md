# undupe

I needed a simple command line tool to first pass sort image duplicates. So, I built undupe. It is a simple command line tool built in C++ for XCode 14.3, that will take a directory as input, hash all files to find binary duplicates, then take these duplicates and move them to an output diretory. This technique is a fast and efficient way to reduce duplicates and prune your dataset before similarity ranking the image duplicates on a deeper level, which will take much more processing power. I am sure this can be improved upon. Such as, this does not process buffer images in batches of n. It just grabs all the images and brute forces. I only needed to first pass prune on roughly 200k images in a single directory, and it was good enough for my use case to run in only a few minute. If you want to pull improvements, please do. I will verify and merge.

Works automatically with the following file extension:
* .jpg
* .png
* .gif
* .webp
* .bmp
* .jpeg
* .tiff
* .heif

To build, clone this repo.

Open XCode proj file or create a new XCode project and move main.cpp into it.

Build/run (Menu: Product/Build For/Running or CMD+SHFT+R).

Compiled binary file will be in the build directory (Menu: Product/Show Build Folder in Finder). 

Navigate to: ./Build/Products/Debug/). Move it to /usr/bin/ or to location of your choice that is in your path.

Then it can be run in terminal:

```
undupe -i <input_directory> -o <output_directory>
```

Example output:
```
$ undupe 
Usage: undupe -i <input directory> -o <output directory>

$ undupe -i input -o output
2849471691476935143: input/1.jpg input/copy.jpg
Move the duplicates to output directory? (y/n): y
Moved file: "output/copy.jpg"
```

Build/Runs on MacOS Ventura 13.4. I did not test on any other platforms. Probably can be built with gcc or g++ on Mac or on Linux etc easily enough.
