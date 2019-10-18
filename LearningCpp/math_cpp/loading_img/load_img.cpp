#include <iostream>
#include <cstdlib>

#define cimg_display  1

#include "CImg.h"

// using namespace std;
using namespace cimg_library;

void ex_save_image() {
  CImg<unsigned char> img(640,400,1,3);  // Define a 640x400 color image with 8 bits per color component.
  img.fill(0);                           // Set pixel values to 0 (color : black)
  unsigned char purple[] = { 255,0,255 };        // Define a purple color
  img.draw_text(100,100,"Hello World",purple); // Draw a purple "Hello world" at coordinates (100,100).
  img.save("test.png");
}

void show_image(){
  CImg<unsigned char> img(640,400,1,3);  // Define a 640x400 color image with 8 bits per color component.
  img.fill(0);                           // Set pixel values to 0 (color : black)
  unsigned char purple[] = { 255,0,255 };        // Define a purple color
  img.draw_text(100,100,"Hello World",purple); // Draw a purple "Hello world" at coordinates (100,100).
  CImgDisplay display(img, "Showing image");
  while (!display.is_closed()){
  	display.wait();
  	img.display(display);
  }
}

// void load_test_img() {
// 	cimg_library::CImg<unsigned char> img("00_016.png");
	// int w=img.width();
	// int h=img.height();
	// int c=img.spectrum();
	// cout << "Dimensions: " << w << "x" << h << " " << c << " channels" <<endl;
// }


// int main() {
//   CImg<unsigned char> image("00_016.png"), visu(500,400,1,3,0);
//   const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };
//   image.blur(2.5);
//   CImgDisplay main_disp(image,"Click a point"), draw_disp(visu,"Intensity profile");
//   while (!main_disp.is_closed() && !draw_disp.is_closed()) {
//     main_disp.wait();
//     if (main_disp.button() && main_disp.mouse_y()>=0) {
//       const int y = main_disp.mouse_y();
//       visu.fill(0).draw_graph(image.get_crop(0,y,0,0,image.width()-1,y,0,0),red,1,1,0,255,0);
//       visu.draw_graph(image.get_crop(0,y,0,1,image.width()-1,y,0,1),green,1,1,0,255,0);
//       visu.draw_graph(image.get_crop(0,y,0,2,image.width()-1,y,0,2),blue,1,1,0,255,0).display(draw_disp);
//       }
//     }
//   return 0;
// }

int main() {
  show_image();
  return 0;
}
