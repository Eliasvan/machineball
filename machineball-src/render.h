// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#ifndef _RENDER_H_
#define _RENDER_H_

void render(void);

extern GLuint balltex;
extern GLuint shadowtex;
extern GLuint floortex;
extern GLuint walltex;
extern GLuint goaltex;
extern GLuint machinetex;
extern GLuint reflecttex;
extern GLuint poweruptex;
extern GLuint bennykramekwebtex;
extern GLuint weaponstex;
extern GLuint explosiontex;

class BodyModel
{
	private:
		GLuint dlist;

	public:
		void load(char *);
		void kill(void);
		void render(void);
};

extern BodyModel bodymodel, pushermodel;


#endif

