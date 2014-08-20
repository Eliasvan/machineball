// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#ifndef _TEXT_H_
#define _TEXT_H_

#include <allegro.h>
#include <alleggl.h>

class Text
{
	private:
		GLuint texture;
		GLuint list;

		int rows;
		int chars;
		

		int staterestore;

		int depth_test;
		int texture_2d;
		void setup(void);
		void restore(void);
	public:
		Text(void) { staterestore=0; }
		void init(void);
		void destroy(void);
		void print(int x, int y, char * s);
		void begin(void) { setup(); staterestore=1; };
		void end(void) { restore(); staterestore=0; };
};

extern Text text;

#endif

