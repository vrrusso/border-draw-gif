# Border Gif Drawer

![](https://github.com/vrrusso/border-draw-gif/blob/master/teste10.jpg)![](https://github.com/vrrusso/border-draw-gif/blob/master/borders.gif)


This is a simple toy project built upon a assignmet from my programing introduction classes. Given an image in PGM(text mode), it will use a naive flood and fill algorithm to segment the
image, so it will find the borders between the zones and produce a little gif animation drawing this borders.

## Usage
To segment an image you have to garantee that is on PGM format, and produce a little descriptor on how to process this image. This descriptor is a txt file containing the filename on 
it's first line, the number of zones n on the second line and n following lines,where each line has three integers, the first two being the (x,y) coordinates of the initial point and the third
being the criterion to determine if the next point lives in or out the especified zone.

To run the project is really easy, you just have to compile it with:
```C
make
```
 
 and then execute it:
 
 ```C
 make run IN=descriptor_filename
 ```

## Special Thanks
Example images, descriptors and assignment idea provided by Moacir Ponti.
Special thanks to Guilherme Amaral Hiromoto, who co-coded this little toy project
